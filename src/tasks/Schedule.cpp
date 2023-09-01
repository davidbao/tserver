//
//  Schedule.cpp
//  tserver
//
//  Created by baowei on 2023/8/21.
//  Copyright (c) 2023 com. All rights reserved.
//

#include "Schedule.h"
#include "TaskContext.h"
#include "thread/TickTimeout.h"
#include "system/Environment.h"
#include "diag/Trace.h"

using namespace Diag;

#ifdef WIN32
#define strcasecmp _stricmp
#define strncasecmp _strnicmp
#endif

String Schedule::toString() const {
    return toJsonNode().toString();
}

CycleSchedule::CycleSchedule(const TimeSpan &interval) : _interval(interval) {
}

CycleSchedule::CycleSchedule(const CycleSchedule &other) {
    CycleSchedule::evaluates(other);
}

CycleSchedule &CycleSchedule::operator=(const CycleSchedule &other) {
    if (this != &other) {
        CycleSchedule::evaluates(other);
    }
    return *this;
}

bool CycleSchedule::equals(const Schedule &other) const {
    auto p = dynamic_cast<const CycleSchedule *>(&other);
    assert(p);
    return _interval == p->_interval;
}

void CycleSchedule::evaluates(const Schedule &other) {
    auto p = dynamic_cast<const CycleSchedule *>(&other);
    assert(p);
    _interval = p->_interval;
}

Schedule *CycleSchedule::clone() const {
    auto p = new CycleSchedule();
    p->evaluates(*this);
    return p;
}

String CycleSchedule::type() const {
    return "cycle";
}

const TimeSpan &CycleSchedule::interval() const {
    return _interval;
}

bool CycleSchedule::isTimeUp(const String &taskName) {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto tc = factory->getService<ITaskCache>();
    if (tc == nullptr) {
        return false;
    }
    const String key = String::format("%s.cycle.ticks", taskName.c_str());
    String tickStr = tc->getValue(key);
    uint64_t tick = 0;
    UInt64::parse(tickStr, tick);

    if ((int64_t) (Environment::getTickCount() - tick) >= (int64_t) _interval.totalMilliseconds()) {
        tc->setValue(key, UInt64(Environment::getTickCount()).toString());
        return true;
    }
    return false;
}

JsonNode CycleSchedule::toJsonNode() const {
    JsonNode node;
    node.add(JsonNode("type", "cycle"));
    node.add(JsonNode("interval", _interval));
    return node;
}

CronSchedule::CronSchedule(const CronSchedule &other) {
    CronSchedule::evaluates(other);
}

CronSchedule &CronSchedule::operator=(const CronSchedule &other) {
    if (this != &other) {
        CronSchedule::evaluates(other);
    }
    return *this;
}

bool CronSchedule::equals(const Schedule &other) const {
    auto p = dynamic_cast<const CronSchedule *>(&other);
    assert(p);
    return _field == p->_field && memcmp(&_line, &p->_line, sizeof(CronLine)) == 0;
}

void CronSchedule::evaluates(const Schedule &other) {
    auto p = dynamic_cast<const CronSchedule *>(&other);
    assert(p);
    _field = p->_field;
    memcpy(&_line, &p->_line, sizeof(CronLine));
}

Schedule *CronSchedule::clone() const {
    auto p = new CronSchedule();
    p->evaluates(*this);
    return p;
}

String CronSchedule::type() const {
    return "cron";
}

bool CronSchedule::isTimeUp(const String &taskName) {
    if (_field.isNullOrEmpty()) {
        return false;
    }

    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto tc = factory->getService<ITaskCache>();
    if (tc == nullptr) {
        return false;
    }

    auto getCurrentSeconds = []() {
        return Environment::getTickCount() / 1000;
    };

    const String key = String::format("%s.cron.seconds", taskName.c_str());
    String tickStr = tc->getValue(key);
    uint64_t t1 = 0;
    if (!UInt64::parse(tickStr, t1)) {
        t1 = getCurrentSeconds();
        tc->setValue(key, UInt64(t1).toString());
    }

    static const int sleepTime = 60;
    /* Synchronize to 1 minute, minimum 1 second */
    int timeout = sleepTime - (int) (getCurrentSeconds() % sleepTime) + 1;

    if (getCurrentSeconds() - t1 >= timeout) {
        uint64_t t2 = getCurrentSeconds();
        tc->setValue(key, UInt64(t2).toString());

        if (t2 - t1 > 0) {
            for (uint64_t t = t1 - t1 % 60; t <= t2; t += 60) {
                if (t <= t1)
                    continue;

                if (isTimeUp()) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool CronSchedule::isTimeUp(const DateTime &current) const {
    if (_line.cl_Mins[current.minute()]
        && _line.cl_Hrs[current.hour()]
        && (_line.cl_Days[current.day() - 1] || _line.cl_Dow[current.dayOfWeek()])
        && _line.cl_Mons[current.month() - 1]) {
        return true;
    }
    return false;
}

JsonNode CronSchedule::toJsonNode() const {
    JsonNode node;
    node.add(JsonNode("type", "cron"));
    node.add(JsonNode("field", _field));
    return node;
}

const CronLine &CronSchedule::cronLine() const {
    return _line;
}

const String &CronSchedule::field() const {
    return _field;
}

bool CronSchedule::parse(const String &str, CronSchedule &schedule) {
    if (str.isNullOrEmpty()) {
        return false;
    }

    // https://crontab.guru/ and https://tool.lu/crontab/

    //   *    *    *    *    *
    //   -    -    -    -    -
    //   |    |    |    |    |
    //   |    |    |    |    +----- day of week (0 - 7) (Sunday=0 or 7) OR sun,mon,tue,wed,thu,fri,sat
    //   |    |    |    +---------- month (1 - 12) OR jan,feb,mar,apr ...
    //   |    |    +--------------- day of month (1 - 31)
    //   |    +-------------------- hour (0 - 23)
    //   +------------------------- minute (0 - 59)

    // @yearly       Run once a year:  "0 0 1 1 *"
    // @annually     Same as @yearly:  "0 0 1 1 *"
    // @monthly      Run once a month: "0 0 1 * *"
    // @weekly       Run once a week:  "0 0 * * 0"
    // @daily        Run once a day:   "0 0 * * *"
    // @midnight     Same as @daily:   "0 0 * * *"
    // @hourly       Run once an hour: "0 * * * *"

    static const char DowAry[] =
       "sun""mon""tue""wed""thu""fri""sat"
        /* "Sun""Mon""Tue""Wed""Thu""Fri""Sat" */
            ;

    static const char MonAry[] =
       "jan""feb""mar""apr""may""jun""jul""aug""sep""oct""nov""dec"
        /* "Jan""Feb""Mar""Apr""May""Jun""Jul""Aug""Sep""Oct""Nov""Dec" */
            ;

    auto fixDayDow = [](CronLine *line) {
        unsigned i;
        int weekUsed = 0;
        int daysUsed = 0;

        for (i = 0; i < sizeof(line->cl_Dow); ++i) {
            if (line->cl_Dow[i] == 0) {
                weekUsed = 1;
                break;
            }
        }
        for (i = 0; i < sizeof(line->cl_Days); ++i) {
            if (line->cl_Days[i] == 0) {
                daysUsed = 1;
                break;
            }
        }
        if (weekUsed != daysUsed) {
            if (weekUsed)
                memset(line->cl_Days, 0, sizeof(line->cl_Days));
            else /* daysUsed */
                memset(line->cl_Dow, 0, sizeof(line->cl_Dow));
        }
    };

    String field;
    if (str[0] == '@') {
        /*
         * "@daily"
         */
        typedef struct SpecialEntry {
            const char *name;
            const char *tokens;
        } SpecialEntry;
        static const SpecialEntry SpecAry[] = {
                /*              hour  day   month weekday */
                { "yearly",     "0 0 1 1 *" },
                { "annually",   "0 0 1 1 *" },
                { "monthly",    "0 0 1 * *" },
                { "weekly",     "0 0 * * 0" },
                { "daily",      "0 0 * * *" },
                { "midnight",   "0 0 * * *" },
                { "hourly",     "0 * * * *" },
                { "",           ""          },
        };
        const SpecialEntry *e = SpecAry;
        String token = str.trim('@');
        for (;;) {
            if (String::equals(token, e->name)) {
                if (e->tokens[0]) {
                    field = e->tokens;
                    break;
                } else {
                    schedule._field = str;
                    return true;
                }
            }
            if (!e->tokens[0])
                break;
            e++;
        }
    } else {
        field = str;
    }

    StringArray tokens;
    StringArray::parse(field, tokens, ' ');
    if (tokens.count() == FieldCount) {
        CronLine line;
        if (!parseField(line.cl_Mins, 60, 0, nullptr, tokens[0])) {
            return false;
        }
        if (!parseField(line.cl_Hrs, 24, 0, nullptr, tokens[1])) {
            return false;
        }
        if (!parseField(line.cl_Days, 31, -1, nullptr, tokens[2])) {
            return false;
        }
        if (!parseField(line.cl_Mons, 12, -1, MonAry, tokens[3])) {
            return false;
        }
        if (!parseField(line.cl_Dow, 7, 0, DowAry, tokens[4])) {
            return false;
        }
        /*
         * fix days and dow - if one is not "*" and the other
         * is "*", the other is set to 0, and vise-versa
         */
        fixDayDow(&line);

        schedule._field = field;
        memcpy(&schedule._line, &line, sizeof(line));

        return true;
    }
    return false;
}

DateTime CronSchedule::getNextTime(const DateTime &current) const {
    Vector<DateTime> times = getNextTimes(current, 1);
    return times.count() == 1 ? times[0] : DateTime::MinValue;
}

Vector<DateTime> CronSchedule::getNextTimes(const DateTime &current, int count) const {
    Vector<DateTime> times;
    count = count > 0 ? count : 1;
    count = count < 100 ? count : 100;
    DateTime time = DateTime(current.year(), current.month(), current.day(), current.hour(), current.minute());
    while(true) {
        time = time.addMinutes(1);
        if (isTimeUp(time)) {
            times.add(time);
            if (times.count() >= count) {
                break;
            }
        }
    }
    return times;
}

bool CronSchedule::parseField(char *ary, int modValue, int off, const char *names, const char *ptr) {
    /* 'names' is a pointer to a set of 3-char abbreviations */
    const char *base = ptr;
    int n1 = -1;
    int n2 = -1;

    // this can't happen due to config_read()
    /*if (base == NULL)
        return;*/

    while (true) {
        int skip = 0;

        /* Handle numeric digit or symbol or '*' */
        if (*ptr == '*') {
            n1 = 0;  /* everything will be filled */
            n2 = modValue - 1;
            skip = 1;
            ++ptr;
        } else if (isdigit(*ptr)) {
            char *endp;
            if (n1 < 0) {
                n1 = (int) strtol(ptr, &endp, 10) + off;
            } else {
                n2 = (int) strtol(ptr, &endp, 10) + off;
            }
            ptr = endp; /* gcc likes temp var for &endp */
            skip = 1;
        } else if (names) {
            int i;

            for (i = 0; names[i]; i += 3) {
                /* was using strncmp before... */
                if (strncasecmp(ptr, &names[i], 3) == 0) {
                    ptr += 3;
                    if (n1 < 0) {
                        n1 = i / 3;
                    } else {
                        n2 = i / 3;
                    }
                    skip = 1;
                    break;
                }
            }
        }

        /* handle optional range '-' */
        if (skip == 0) {
            goto err;
        }
        if (*ptr == '-' && n2 < 0) {
            ++ptr;
            continue;
        }

        /*
         * collapse single-value ranges, handle skipmark, and fill
         * in the character array appropriately.
         */
        if (n2 < 0) {
            n2 = n1;
        }
        if (*ptr == '/') {
            char *endp;
            skip = (int) strtol(ptr + 1, &endp, 10);
            ptr = endp; /* gcc likes temp var for &endp */
        }

        /*
         * fill array, using a failsafe is the easiest way to prevent
         * an endless loop
         */
        {
            int s0 = 1;
            int failsafe = 1024;

            --n1;
            do {
                n1 = (n1 + 1) % modValue;
                if (n1 < 0)
                    goto err;

                if (--s0 == 0) {
                    ary[n1 % modValue] = 1;
                    s0 = skip;
                }
                if (--failsafe == 0) {
                    goto err;
                }
            } while (n1 != n2);
        }
        if (*ptr != ',') {
            break;
        }
        ++ptr;
        n1 = -1;
        n2 = -1;
    }

    if (*ptr) {
err:
        Trace::debug(String::format("parse error at %s", base));
        return false;
    }
    return true;
}
