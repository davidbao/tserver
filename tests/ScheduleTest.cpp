//
//  ScheduleTest.cpp
//  common
//
//  Created by baowei on 2023/8/21.
//  Copyright (c) 2023 com. All rights reserved.
//

#include "../src/tasks/Schedule.h"

// https://crontab.guru/

typedef bool (*check_callback)(const CronLine &);

struct CronScheduleInfo {
    char name[64];
    char timeStr[64];
    check_callback callback;
};

auto func1 = [](const CronLine &cronLine) {
    return true;
};
auto func2 = func1;
auto func3 = [](const CronLine &cronLine) {
    return true;
};
auto func4 = func3;
auto func5 = [](const CronLine &cronLine) {
    return true;
};
auto func6 = [](const CronLine &cronLine) {
    return true;
};
auto func7 = [](const CronLine &cronLine) {
    return true;
};
auto func8 = [](const CronLine &cronLine) {
    return true;
};
auto func9 = func8;
auto func10 = [](const CronLine &cronLine) {
    return true;
};
auto func11 = [](const CronLine &cronLine) {
    return true;
};
auto func12 = [](const CronLine &cronLine) {
    return true;
};
auto func13 = func12;
auto func14 = func11;
auto func15 = func12;
auto func16 = [](const CronLine &cronLine) {
    return true;
};
auto func17 = [](const CronLine &cronLine) {
    return true;
};
auto func18 = [](const CronLine &cronLine) {
    return true;
};
auto func19 = func17;
auto func20 = [](const CronLine &cronLine) {
    return true;
};
auto func21 = func20;
auto func22 = func20;
auto func23 = [](const CronLine &cronLine) {
    return true;
};
auto func24 = func23;
auto func25 = func23;
auto func26 = func23;
auto func27 = [](const CronLine &cronLine) {
    return true;
};
auto func28 = func27;
auto func29 = [](const CronLine &cronLine) {
    return true;
};
auto func30 = [](const CronLine &cronLine) {
    return true;
};
auto func31 = func30;
auto func32 = [](const CronLine &cronLine) {
    return true;
};
auto func33 = [](const CronLine &cronLine) {
    return true;
};
auto func34 = [](const CronLine &cronLine) {
    return true;
};
auto func35 = func34;

auto func36 = [](const CronLine &cronLine) {
    return true;
};
auto func37 = func36;
auto func38 = func36;
auto func39 = func36;
auto func40 = [](const CronLine &cronLine) {
    return true;
};
auto func41 = [](const CronLine &cronLine) {
    return true;
};
auto func42 = [](const CronLine &cronLine) {
    return true;
};
auto func43 = [](const CronLine &cronLine) {
    return true;
};
auto func44 = func36;
auto func45 = func36;
auto func46 = func36;
auto func47 = [](const CronLine &cronLine) {
    return true;
};
auto func48 = [](const CronLine &cronLine) {
    return true;
};
auto func49 = [](const CronLine &cronLine) {
    return true;
};
auto func50 = [](const CronLine &cronLine) {
    return true;
};
auto func51 = [](const CronLine &cronLine) {
    return true;
};
auto func52 = [](const CronLine &cronLine) {
    return true;
};
auto func53 = func52;
auto func54 = [](const CronLine &cronLine) {
    return true;
};
auto func55 = [](const CronLine &cronLine) {
    return true;
};
auto func56 = func55;
auto func57 = func55;
auto func58 = [](const CronLine &cronLine) {
    return true;
};
auto func59 = func58;
auto func60 = [](const CronLine &cronLine) {
    return true;
};
auto func61 = func60;
auto func62 = func60;
auto func63 = func60;
auto func64 = [](const CronLine &cronLine) {
    return true;
};
auto func65 = func64;
auto func66 = func64;
auto func67 = [](const CronLine &cronLine) {
    return true;
};
auto func68 = [](const CronLine &cronLine) {
    return true;
};
auto func69 = [](const CronLine &cronLine) {
    return true;
};
auto func70 = [](const CronLine &cronLine) {
    return true;
};

//static const int ParseLength = 70;
static const CronScheduleInfo items[] = {
        {"every minute",             "* * * * *",      func1},
        {"every 1 minute",           "* * * * *",      func2},
        {"every 2 minutes",          "*/2 * * * *",    func3},
        {"every even minute",        "*/2 * * * *",    func4},
        {"every uneven minute",      "1-59/2 * * * *", func5},
        {"every 3 minutes",          "*/3 * * * *",    func6},
        {"every 4 minutes",          "*/4 * * * *",    func7},
        {"every 5 minutes",          "*/5 * * * *",    func8},
        {"every five minutes",       "*/5 * * * *",    func9},
        {"every 6 minutes",          "*/6 * * * *",    func10},
        {"every 10 minutes",         "*/10 * * * *",   func11},
        {"every 15 minutes",         "*/15 * * * *",   func12},
        {"every fifteen minutes",    "*/15 * * * *",   func13},
        {"every ten minutes",        "*/10 * * * *",   func14},
        {"every quarter hour",       "*/15 * * * *",   func15},
        {"every 20 minutes",         "*/20 * * * *",   func16},
        {"every 30 minutes",         "*/30 * * * *",   func17},
        {"every hour at 30 minutes", "30 * * * *",     func18},
        {"every half hour",          "*/30 * * * *",   func19},
        {"every 60 minutes",         "0 * * * *",      func20},
        {"every hour",               "0 * * * *",      func21},
        {"every 1 hour",             "0 * * * *",      func22},
        {"every 2 hours",            "0 */2 * * *",    func23},
        {"every two hours",          "0 */2 * * *",    func24},
        {"every even hour",          "0 */2 * * *",    func25},
        {"every other hour",         "0 */2 * * *",    func26},
        {"every 3 hours",            "0 */3 * * *",    func27},
        {"every three hours",        "0 */3 * * *",    func28},
        {"every 4 hours",            "0 */4 * * *",    func29},
        {"every 6 hours",            "0 */6 * * *",    func30},
        {"every six hours",          "0 */6 * * *",    func31},
        {"every 8 hours",            "0 */8 * * *",    func32},
        {"every 12 hours",           "0 */12 * * *",   func33},
        {"hour range",               "0 9-17 * * *",   func34},
        {"between certain hours",    "0 9-17 * * *",   func35},

        {"every day",                "0 0 * * *",      func36},
        {"daily",                    "0 0 * * *",      func37},
        {"once a day",               "0 0 * * *",      func38},
        {"every night",              "0 0 * * *",      func39},
        {"every day at 1am",         "0 1 * * *",      func40},
        {"every day at 2am",         "0 2 * * *",      func41},
        {"every day 8am",            "0 8 * * *",      func42},
        {"every morning",            "0 9 * * *",      func43},
        {"every midnight",           "0 0 * * *",      func44},
        {"every day at midnight",    "0 0 * * *",      func45},
        {"every night at midnight",  "0 0 * * *",      func46},
        {"every sunday",             "0 0 * * SUN",    func47},
        {"every monday",             "0 0 * * MON",    func48},
        {"every tuesday",            "0 0 * * TUE",    func49},
        {"every wednesday",          "0 0 * * WED",    func50},
        {"every thursday",           "0 0 * * THU",    func51},
        {"every friday",             "0 0 * * FRI",    func52},
        {"every friday at midnight", "0 0 * * FRI",    func53},
        {"every saturday",           "0 0 * * SAT",    func54},
        {"every weekday",            "0 0 * * 1-5",    func55},
        {"weekdays only",            "0 0 * * 1-5",    func56},
        {"monday to friday",         "0 0 * * 1-5",    func57},
        {"every weekend",            "0 0 * * 6,0",    func58},
        {"weekends only",            "0 0 * * 6,0",    func59},
        {"every 7 days",             "0 0 * * 0",      func60},
        {"every week",               "0 0 * * 0",      func61},
        {"weekly",                   "0 0 * * 0",      func62},
        {"once a week",              "0 0 * * 0",      func63},
        {"every month",              "0 0 1 * *",      func64},
        {"monthly",                  "0 0 1 * *",      func65},
        {"once a month",             "0 0 1 * *",      func66},
        {"every other month",        "0 0 1 */2 *",    func67},
        {"every quarter",            "0 0 1 */3 *",    func68},
        {"every 6 months",           "0 0 1 */6 *",    func69},
        {"every year",               "0 0 1 1 *",      func70},
};

bool testCronSchedule_parse() {
    {
        for (const auto &item: items) {
            CronSchedule schedule;
            if (!CronSchedule::parse(item.timeStr, schedule)) {
                printf("Failed to parse, name: %s, time str: %s\n", item.name, item.timeStr);
                return false;
            }
            if (item.callback != nullptr) {
                const CronLine &cronLine = schedule.cronLine();
                if (!item.callback(cronLine)) {
                    printf("Failed to parse, name: %s, time str: %s\n", item.name, item.timeStr);
                    return false;
                }
            }
        }
    }

    {
        static const char *str = "-1 * * * *";
        CronSchedule schedule;
        if (CronSchedule::parse(str, schedule)) {
            printf("Failed to parse, time str: %s\n", str);
            return false;
        }
    }
    {
        static const char *str = "0 * * * *";
        CronSchedule schedule;
        if (!CronSchedule::parse(str, schedule)) {
            printf("Failed to parse, time str: %s\n", str);
            return false;
        }
    }
    {
        static const char *str = "59 * * * *";
        CronSchedule schedule;
        if (!CronSchedule::parse(str, schedule)) {
            printf("Failed to parse, time str: %s\n", str);
            return false;
        }
    }
    {
        static const char *str = "60 * * * *";
        CronSchedule schedule;
        if (CronSchedule::parse(str, schedule)) {
            printf("Failed to parse, time str: %s\n", str);
            return false;
        }
    }

    {
        static const char *str = "* -1 * * *";
        CronSchedule schedule;
        if (CronSchedule::parse(str, schedule)) {
            printf("Failed to parse, time str: %s\n", str);
            return false;
        }
    }
    {
        static const char *str = "* 0 * * *";
        CronSchedule schedule;
        if (!CronSchedule::parse(str, schedule)) {
            printf("Failed to parse, time str: %s\n", str);
            return false;
        }
    }
    {
        static const char *str = "* 23 * * *";
        CronSchedule schedule;
        if (!CronSchedule::parse(str, schedule)) {
            printf("Failed to parse, time str: %s\n", str);
            return false;
        }
    }
    {
        static const char *str = "* 24 * * *";
        CronSchedule schedule;
        if (CronSchedule::parse(str, schedule)) {
            printf("Failed to parse, time str: %s\n", str);
            return false;
        }
    }

    {
        static const char *str = "* * 0 * *";
        CronSchedule schedule;
        if (CronSchedule::parse(str, schedule)) {
            printf("Failed to parse, time str: %s\n", str);
            return false;
        }
    }
    {
        static const char *str = "* * 1 * *";
        CronSchedule schedule;
        if (!CronSchedule::parse(str, schedule)) {
            printf("Failed to parse, time str: %s\n", str);
            return false;
        }
    }
    {
        static const char *str = "* * 31 * *";
        CronSchedule schedule;
        if (!CronSchedule::parse(str, schedule)) {
            printf("Failed to parse, time str: %s\n", str);
            return false;
        }
    }
    {
        static const char *str = "* * 32 * *";
        CronSchedule schedule;
        if (CronSchedule::parse(str, schedule)) {
            printf("Failed to parse, time str: %s\n", str);
            return false;
        }
    }

    {
        static const char *str = "* * * 0 *";
        CronSchedule schedule;
        if (CronSchedule::parse(str, schedule)) {
            printf("Failed to parse, time str: %s\n", str);
            return false;
        }
    }
    {
        static const char *str = "* * * 1 *";
        CronSchedule schedule;
        if (!CronSchedule::parse(str, schedule)) {
            printf("Failed to parse, time str: %s\n", str);
            return false;
        }
    }
    {
        static const char *str = "* * * 12 *";
        CronSchedule schedule;
        if (!CronSchedule::parse(str, schedule)) {
            printf("Failed to parse, time str: %s\n", str);
            return false;
        }
    }
    {
        static const char *str = "* * * 13 *";
        CronSchedule schedule;
        if (CronSchedule::parse(str, schedule)) {
            printf("Failed to parse, time str: %s\n", str);
            return false;
        }
    }

    {
        static const char *str = "* * * * -1";
        CronSchedule schedule;
        if (CronSchedule::parse(str, schedule)) {
            printf("Failed to parse, time str: %s\n", str);
            return false;
        }
    }
    {
        static const char *str = "* * * * 0";
        CronSchedule schedule;
        if (!CronSchedule::parse(str, schedule)) {
            printf("Failed to parse, time str: %s\n", str);
            return false;
        }
    }
    {
        static const char *str = "* * * * 6";
        CronSchedule schedule;
        if (!CronSchedule::parse(str, schedule)) {
            printf("Failed to parse, time str: %s\n", str);
            return false;
        }
    }
    {
        static const char *str = "* * * * 7";
        CronSchedule schedule;
        if (CronSchedule::parse(str, schedule)) {
            printf("Failed to parse, time str: %s\n", str);
            return false;
        }
    }

    {
        static const char *weeks[] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
        for (const auto &week: weeks) {
            String str = String::format("* * * * %s", week);
            CronSchedule schedule;
            if (!CronSchedule::parse(str, schedule)) {
                printf("Failed to parse, time str: %s\n", str.c_str());
                return false;
            }
        }
    }

    {
        static const char *years[] = {"JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV",
                                      "DEC"};
        for (const auto &year: years) {
            String str = String::format("* * * %s *", year);
            CronSchedule schedule;
            if (!CronSchedule::parse(str, schedule)) {
                printf("Failed to parse, time str: %s\n", str.c_str());
                return false;
            }
        }
    }

    {
        static const char *str = "* 1,3,6,2,5,23,7 * * *";
        CronSchedule schedule;
        if (!CronSchedule::parse(str, schedule)) {
            printf("Failed to parse, time str: %s\n", str);
            return false;
        }
    }

    return true;
}

bool testCronSchedule_toString() {
    {
        for (const auto &item: items) {
            CronSchedule schedule;
            if (!CronSchedule::parse(item.timeStr, schedule)) {
                return false;
            }

            String field = schedule.field();
            if (field != item.timeStr) {
                CronSchedule schedule2;
                if (!CronSchedule::parse(field, schedule2)) {
                    return false;
                }
                if (schedule != schedule2) {
                    printf("Failed to toString, name: %s, time str: %s, to str: %s\n",
                           item.name, item.timeStr, field.c_str());
                    return false;
                }
            }
        }
    }

    return true;
}

static const int GetNextCount = 4;
struct GetNextPara {
    const char *current;
    const char *experts[GetNextCount];
};

bool testCronSchedule_getNext() {
    auto func = [](const char *field, const GetNextPara &para) {
        CronSchedule schedule;
        if (!CronSchedule::parse(field, schedule)) {
            return false;
        }

        DateTime currentTime;
        DateTime::parse(para.current, currentTime);
        Vector<DateTime> times = schedule.getNextTimes(currentTime, GetNextCount);
        if (times.count() != GetNextCount) {
            printf("Can not retrieve 4 times.\n");
            return false;
        }
        for (int i = 0; i < GetNextCount; ++i) {
            const char *expert = para.experts[i];
            const DateTime &time = times[i];
            if (expert != nullptr) {
                if (time.toString() != expert) {
                    printf("Failed to getNext'%d', field: %s, current: %s, actual: %s, expert: %s\n",
                           i, field, para.current, time.toString().c_str(), expert);
                    return false;
                }
            }
        }
        return true;
    };

    // minute any value.
    if (!func("* * * * *", {"2023-08-22 14:07:12",
                            "2023-08-22 14:08:00", "2023-08-22 14:09:00",
                            "2023-08-22 14:10:00", "2023-08-22 14:11:00"})) {
        return false;
    }
    if (!func("* * * * *", {"2023-08-22 14:08:00",
                            "2023-08-22 14:09:00", "2023-08-22 14:10:00",
                            "2023-08-22 14:11:00", "2023-08-22 14:12:00"})) {
        return false;
    }
    if (!func("*/1 * * * *", {"2023-08-22 14:07:12",
                              "2023-08-22 14:08:00", "2023-08-22 14:09:00",
                              "2023-08-22 14:10:00", "2023-08-22 14:11:00"})) {
        return false;
    }
    if (!func("*/2 * * * *", {"2023-08-22 14:07:12",
                              "2023-08-22 14:08:00", "2023-08-22 14:10:00",
                              "2023-08-22 14:12:00", "2023-08-22 14:14:00"})) {
        return false;
    }
    if (!func("*/2 * * * *", {"2023-08-22 14:58:00",
                              "2023-08-22 15:00:00", "2023-08-22 15:02:00",
                              "2023-08-22 15:04:00", "2023-08-22 15:06:00"})) {
        return false;
    }
    if (!func("*/2 * * * *", {"2023-08-22 14:08:12",
                              "2023-08-22 14:10:00", "2023-08-22 14:12:00",
                              "2023-08-22 14:14:00", "2023-08-22 14:16:00"})) {
        return false;
    }
    if (!func("*/2 * * * *", {"2023-08-22 14:07:00",
                              "2023-08-22 14:08:00", "2023-08-22 14:10:00",
                              "2023-08-22 14:12:00", "2023-08-22 14:14:00"})) {
        return false;
    }
    if (!func("*/3 * * * *", {"2023-08-22 14:08:12",
                              "2023-08-22 14:09:00", "2023-08-22 14:12:00",
                              "2023-08-22 14:15:00", "2023-08-22 14:18:00"})) {
        return false;
    }
    if (!func("*/5 * * * *", {"2023-08-22 14:08:12",
                              "2023-08-22 14:10:00", "2023-08-22 14:15:00",
                              "2023-08-22 14:20:00", "2023-08-22 14:25:00"})) {
        return false;
    }
    if (!func("*/20 * * * *", {"2023-08-22 14:08:12",
                               "2023-08-22 14:20:00", "2023-08-22 14:40:00",
                               "2023-08-22 15:00:00", "2023-08-22 15:20:00"})) {
        return false;
    }
    if (!func("*/59 * * * *", {"2023-08-22 14:59:00",
                               "2023-08-22 15:00:00", "2023-08-22 15:59:00",
                               "2023-08-22 16:00:00", "2023-08-22 16:59:00"})) {
        return false;
    }
    if (!func("*/59 * * * *", {"2023-08-22 14:07:12",
                               "2023-08-22 14:59:00", "2023-08-22 15:00:00",
                               "2023-08-22 15:59:00", "2023-08-22 16:00:00"})) {
        return false;
    }
    if (!func("*/60 * * * *", {"2023-08-22 14:07:12",
                               "2023-08-22 15:00:00", "2023-08-22 16:00:00",
                               "2023-08-22 17:00:00", "2023-08-22 18:00:00"})) {
        return false;
    }
    if (!func("*/60 * * * *", {"2023-08-22 14:00:00",
                               "2023-08-22 15:00:00", "2023-08-22 16:00:00",
                               "2023-08-22 17:00:00", "2023-08-22 18:00:00"})) {
        return false;
    }
    if (!func("*/66 * * * *", {"2023-08-22 14:07:12",
                               "2023-08-22 15:00:00", "2023-08-22 16:00:00",
                               "2023-08-22 17:00:00", "2023-08-22 18:00:00"})) {
        return false;
    }

    // minute single value.
    if (!func("0 * * * *", {"2023-08-22 14:07:12",
                            "2023-08-22 15:00:00", "2023-08-22 16:00:00",
                            "2023-08-22 17:00:00", "2023-08-22 18:00:00"})) {
        return false;
    }
    if (!func("1 * * * *", {"2023-08-22 14:07:12",
                            "2023-08-22 15:01:00", "2023-08-22 16:01:00",
                            "2023-08-22 17:01:00", "2023-08-22 18:01:00"})) {
        return false;
    }
    if (!func("1 * * * *", {"2023-08-22 15:01:00",
                            "2023-08-22 16:01:00", "2023-08-22 17:01:00",
                            "2023-08-22 18:01:00", "2023-08-22 19:01:00"})) {
        return false;
    }
    if (!func("33 * * * *", {"2023-08-22 14:07:12",
                             "2023-08-22 14:33:00", "2023-08-22 15:33:00",
                             "2023-08-22 16:33:00", "2023-08-22 17:33:00"})) {
        return false;
    }
    if (!func("59 * * * *", {"2023-08-22 14:07:12",
                             "2023-08-22 14:59:00", "2023-08-22 15:59:00",
                             "2023-08-22 16:59:00", "2023-08-22 17:59:00"})) {
        return false;
    }

    // minute range values.
    if (!func("1-3 * * * *", {"2023-08-22 14:07:12",
                              "2023-08-22 15:01:00", "2023-08-22 15:02:00",
                              "2023-08-22 15:03:00", "2023-08-22 16:01:00"})) {
        return false;
    }
    if (!func("8-10 * * * *", {"2023-08-22 14:07:12",
                               "2023-08-22 14:08:00", "2023-08-22 14:09:00",
                               "2023-08-22 14:10:00", "2023-08-22 15:08:00"})) {
        return false;
    }
    if (!func("8-10 * * * *", {"2023-08-22 14:09:12",
                               "2023-08-22 14:10:00", "2023-08-22 15:08:00",
                               "2023-08-22 15:09:00", "2023-08-22 15:10:00"})) {
        return false;
    }
    if (!func("1-3/2 * * * *", {"2023-08-22 14:07:12",
                                "2023-08-22 15:01:00", "2023-08-22 15:03:00",
                                "2023-08-22 16:01:00", "2023-08-22 16:03:00"})) {
        return false;
    }
    if (!func("8-10/2 * * * *", {"2023-08-22 14:07:12",
                                 "2023-08-22 14:08:00", "2023-08-22 14:10:00",
                                 "2023-08-22 15:08:00", "2023-08-22 15:10:00"})) {
        return false;
    }

    // minute list values.
    if (!func("1,2,3 * * * *", {"2023-08-22 14:07:12",
                                "2023-08-22 15:01:00", "2023-08-22 15:02:00",
                                "2023-08-22 15:03:00", "2023-08-22 16:01:00"})) {
        return false;
    }
    if (!func("1,2,3 * * * *", {"2023-08-22 14:00:05",
                                "2023-08-22 14:01:00", "2023-08-22 14:02:00",
                                "2023-08-22 14:03:00", "2023-08-22 15:01:00"})) {
        return false;
    }
    if (!func("1,2,3 * * * *", {"2023-08-22 14:01:05",
                                "2023-08-22 14:02:00", "2023-08-22 14:03:00",
                                "2023-08-22 15:01:00", "2023-08-22 15:02:00"})) {
        return false;
    }

    if (!func("1,3,2 * * * *", {"2023-08-22 14:07:12",
                                "2023-08-22 15:01:00", "2023-08-22 15:02:00",
                                "2023-08-22 15:03:00", "2023-08-22 16:01:00"})) {
        return false;
    }
    if (!func("1,3,2 * * * *", {"2023-08-22 14:00:05",
                                "2023-08-22 14:01:00", "2023-08-22 14:02:00",
                                "2023-08-22 14:03:00", "2023-08-22 15:01:00"})) {
        return false;
    }
    if (!func("1,3,2 * * * *", {"2023-08-22 14:01:05",
                                "2023-08-22 14:02:00", "2023-08-22 14:03:00",
                                "2023-08-22 15:01:00", "2023-08-22 15:02:00"})) {
        return false;
    }

    // hour any values.
    if (!func("* */1 * * *", {"2023-08-22 14:07:12",
                              "2023-08-22 14:08:00", "2023-08-22 14:09:00",
                              "2023-08-22 14:10:00", "2023-08-22 14:11:00"})) {
        return false;
    }
    if (!func("* */2 * * *", {"2023-08-22 14:07:12",
                              "2023-08-22 14:08:00", "2023-08-22 14:09:00",
                              "2023-08-22 14:10:00", "2023-08-22 14:11:00"})) {
        return false;
    }
    if (!func("* */2 * * *", {"2023-08-22 14:00:00",
                              "2023-08-22 14:01:00", "2023-08-22 14:02:00",
                              "2023-08-22 14:03:00", "2023-08-22 14:04:00"})) {
        return false;
    }
    if (!func("* */2 * * *", {"2023-08-22 15:07:12",
                              "2023-08-22 16:00:00", "2023-08-22 16:01:00",
                              "2023-08-22 16:02:00", "2023-08-22 16:03:00"})) {
        return false;
    }
    if (!func("* */2 * * *", {"2023-08-22 15:00:00",
                              "2023-08-22 16:00:00", "2023-08-22 16:01:00",
                              "2023-08-22 16:02:00", "2023-08-22 16:03:00"})) {
        return false;
    }
    if (!func("* */2 * * *", {"2023-08-22 15:59:00",
                              "2023-08-22 16:00:00", "2023-08-22 16:01:00",
                              "2023-08-22 16:02:00", "2023-08-22 16:03:00"})) {
        return false;
    }
    if (!func("* */5 * * *", {"2023-08-22 14:08:12",
                              "2023-08-22 15:00:00", "2023-08-22 15:01:00",
                              "2023-08-22 15:02:00", "2023-08-22 15:03:00"})) {
        return false;
    }
    if (!func("* */3 * * *", {"2023-08-22 14:08:12",
                              "2023-08-22 15:00:00", "2023-08-22 15:01:00",
                              "2023-08-22 15:02:00", "2023-08-22 15:03:00"})) {
        return false;
    }
    if (!func("* */23 * * *", {"2023-08-22 14:07:12",
                               "2023-08-22 23:00:00", "2023-08-22 23:01:00",
                               "2023-08-22 23:02:00", "2023-08-22 23:03:00"})) {
        return false;
    }
    if (!func("* */24 * * *", {"2023-08-23 00:00:00",
                               "2023-08-23 00:01:00", "2023-08-23 00:02:00",
                               "2023-08-23 00:03:00", "2023-08-23 00:04:00"})) {
        return false;
    }
    if (!func("* */24 * * *", {"2023-08-22 14:07:12",
                               "2023-08-23 00:00:00", "2023-08-23 00:01:00",
                               "2023-08-23 00:02:00", "2023-08-23 00:03:00"})) {
        return false;
    }
    if (!func("* */30 * * *", {"2023-08-22 14:07:12",
                               "2023-08-23 00:00:00", "2023-08-23 00:01:00",
                               "2023-08-23 00:02:00", "2023-08-23 00:03:00"})) {
        return false;
    }

    // hour single value.
    if (!func("* 0 * * *", {"2023-08-22 14:07:12",
                            "2023-08-23 00:00:00", "2023-08-23 00:01:00",
                            "2023-08-23 00:02:00", "2023-08-23 00:03:00"})) {
        return false;
    }
    if (!func("* 1 * * *", {"2023-08-22 14:07:12",
                            "2023-08-23 01:00:00", "2023-08-23 01:01:00",
                            "2023-08-23 01:02:00", "2023-08-23 01:03:00"})) {
        return false;
    }
    if (!func("* 1 * * *", {"2023-08-22 15:01:00",
                            "2023-08-23 01:00:00", "2023-08-23 01:01:00",
                            "2023-08-23 01:02:00", "2023-08-23 01:03:00"})) {
        return false;
    }
    if (!func("* 15 * * *", {"2023-08-22 14:07:12",
                             "2023-08-22 15:00:00", "2023-08-22 15:01:00",
                             "2023-08-22 15:02:00", "2023-08-22 15:03:00"})) {
        return false;
    }
    if (!func("* 23 * * *", {"2023-08-22 14:07:12",
                             "2023-08-22 23:00:00", "2023-08-22 23:01:00",
                             "2023-08-22 23:02:00", "2023-08-22 23:03:00"})) {
        return false;
    }

    // hour range values.
    if (!func("* 1-3 * * *", {"2023-08-22 14:07:12",
                              "2023-08-23 01:00:00", "2023-08-23 01:01:00",
                              "2023-08-23 01:02:00", "2023-08-23 01:03:00"})) {
        return false;
    }
    if (!func("* 15-20 * * *", {"2023-08-22 14:07:12",
                                "2023-08-22 15:00:00", "2023-08-22 15:01:00",
                                "2023-08-22 15:02:00", "2023-08-22 15:03:00"})) {
        return false;
    }
    if (!func("* 13-20 * * *", {"2023-08-22 14:07:12",
                                "2023-08-22 14:08:00", "2023-08-22 14:09:00",
                                "2023-08-22 14:10:00", "2023-08-22 14:11:00"})) {
        return false;
    }
    if (!func("* 1-3/2 * * *", {"2023-08-22 14:07:12",
                                "2023-08-23 01:00:00", "2023-08-23 01:01:00",
                                "2023-08-23 01:02:00", "2023-08-23 01:03:00"})) {
        return false;
    }
    if (!func("* 15-20/2 * * *", {"2023-08-22 14:07:12",
                                  "2023-08-22 15:00:00", "2023-08-22 15:01:00",
                                  "2023-08-22 15:02:00", "2023-08-22 15:03:00"})) {
        return false;
    }
    if (!func("* 13-20/2 * * *", {"2023-08-22 13:57:12",
                                  "2023-08-22 13:58:00", "2023-08-22 13:59:00",
                                  "2023-08-22 15:00:00", "2023-08-22 15:01:00"})) {
        return false;
    }
    if (!func("* 13-20/2 * * *", {"2023-08-22 14:07:12",
                                  "2023-08-22 15:00:00", "2023-08-22 15:01:00",
                                  "2023-08-22 15:02:00", "2023-08-22 15:03:00"})) {
        return false;
    }

    // hour list values.
    if (!func("* 1,2,3 * * *", {"2023-08-22 01:00:00",
                                "2023-08-22 01:01:00", "2023-08-22 01:02:00",
                                "2023-08-22 01:03:00", "2023-08-22 01:04:00"})) {
        return false;
    }
    if (!func("* 1,2,3 * * *", {"2023-08-22 14:07:12",
                                "2023-08-23 01:00:00", "2023-08-23 01:01:00",
                                "2023-08-23 01:02:00", "2023-08-23 01:03:00"})) {
        return false;
    }
    if (!func("* 1,3,7 * * *", {"2023-08-22 14:00:05",
                                "2023-08-23 01:00:00", "2023-08-23 01:01:00",
                                "2023-08-23 01:02:00", "2023-08-23 01:03:00"})) {
        return false;
    }
    if (!func("* 6,9,18 * * *", {"2023-08-22 04:00:05",
                                 "2023-08-22 06:00:00", "2023-08-22 06:01:00",
                                 "2023-08-22 06:02:00", "2023-08-22 06:03:00"})) {
        return false;
    }

    if (!func("* 1,3,2 * * *", {"2023-08-22 01:00:00",
                                "2023-08-22 01:01:00", "2023-08-22 01:02:00",
                                "2023-08-22 01:03:00", "2023-08-22 01:04:00"})) {
        return false;
    }
    if (!func("* 1,3,2 * * *", {"2023-08-22 14:07:12",
                                "2023-08-23 01:00:00", "2023-08-23 01:01:00",
                                "2023-08-23 01:02:00", "2023-08-23 01:03:00"})) {
        return false;
    }
    if (!func("* 1,7,3 * * *", {"2023-08-22 14:00:05",
                                "2023-08-23 01:00:00", "2023-08-23 01:01:00",
                                "2023-08-23 01:02:00", "2023-08-23 01:03:00"})) {
        return false;
    }
    if (!func("* 6,18,9 * * *", {"2023-08-22 04:00:05",
                                 "2023-08-22 06:00:00", "2023-08-22 06:01:00",
                                 "2023-08-22 06:02:00", "2023-08-22 06:03:00"})) {
        return false;
    }

    // day any values.
    if (!func("* * */1 * *", {"2023-08-22 14:07:12",
                              "2023-08-22 14:08:00", "2023-08-22 14:09:00",
                              "2023-08-22 14:10:00", "2023-08-22 14:11:00"})) {
        return false;
    }
    if (!func("* * */2 * *", {"2023-08-22 14:07:12",
                              "2023-08-23 00:00:00", "2023-08-23 00:01:00",
                              "2023-08-23 00:02:00", "2023-08-23 00:03:00"})) {
        return false;
    }
    if (!func("* * */2 * *", {"2023-08-23 15:07:12",
                              "2023-08-23 15:08:00", "2023-08-23 15:09:00",
                              "2023-08-23 15:10:00", "2023-08-23 15:11:00"})) {
        return false;
    }
    if (!func("* * */5 * *", {"2023-08-28 10:48:12",
                              "2023-08-31 00:00:00", "2023-08-31 00:01:00",
                              "2023-08-31 00:02:00", "2023-08-31 00:03:00"})) {
        return false;
    }
    if (!func("* * */3 * *", {"2023-08-28 10:48:12",
                              "2023-08-28 10:49:00", "2023-08-28 10:50:00",
                              "2023-08-28 10:51:00", "2023-08-28 10:52:00"})) {
        return false;
    }
    if (!func("* * */30 * *", {"2023-08-28 10:48:12",
                               "2023-08-31 00:00:00", "2023-08-31 00:01:00",
                               "2023-08-31 00:02:00", "2023-08-31 00:03:00"})) {
        return false;
    }
    if (!func("* * */31 * *", {"2023-08-28 10:48:12",
                               "2023-09-01 00:00:00", "2023-09-01 00:01:00",
                               "2023-09-01 00:02:00", "2023-09-01 00:03:00"})) {
        return false;
    }
    if (!func("* * */32 * *", {"2023-08-28 10:48:12",
                               "2023-09-01 00:00:00", "2023-09-01 00:01:00",
                               "2023-09-01 00:02:00", "2023-09-01 00:03:00"})) {
        return false;
    }
    if (!func("* * */32 * *", {"2023-08-31 10:48:12",
                               "2023-09-01 00:00:00", "2023-09-01 00:01:00",
                               "2023-09-01 00:02:00", "2023-09-01 00:03:00"})) {
        return false;
    }

    // day single value.
    if (!func("* * 1 * *", {"2023-08-22 14:07:12",
                            "2023-09-01 00:00:00", "2023-09-01 00:01:00",
                            "2023-09-01 00:02:00", "2023-09-01 00:03:00"})) {
        return false;
    }
    if (!func("* * 2 * *", {"2023-08-22 14:07:12",
                            "2023-09-02 00:00:00", "2023-09-02 00:01:00",
                            "2023-09-02 00:02:00", "2023-09-02 00:03:00"})) {
        return false;
    }
    if (!func("* * 2 * *", {"2023-08-22 15:01:00",
                            "2023-09-02 00:00:00", "2023-09-02 00:01:00",
                            "2023-09-02 00:02:00", "2023-09-02 00:03:00"})) {
        return false;
    }
    if (!func("* * 15 * *", {"2023-08-22 14:07:12",
                             "2023-09-15 00:00:00", "2023-09-15 00:01:00",
                             "2023-09-15 00:02:00", "2023-09-15 00:03:00"})) {
        return false;
    }
    if (!func("* * 31 * *", {"2023-08-22 14:07:12",
                             "2023-08-31 00:00:00", "2023-08-31 00:01:00",
                             "2023-08-31 00:02:00", "2023-08-31 00:03:00"})) {
        return false;
    }

    // day range values.
    if (!func("* * 1-3 * *", {"2023-08-22 14:07:12",
                              "2023-09-01 00:00:00", "2023-09-01 00:01:00",
                              "2023-09-01 00:02:00", "2023-09-01 00:03:00"})) {
        return false;
    }
    if (!func("* * 15-20 * *", {"2023-08-22 14:07:12",
                                "2023-09-15 00:00:00", "2023-09-15 00:01:00",
                                "2023-09-15 00:02:00", "2023-09-15 00:03:00"})) {
        return false;
    }
    if (!func("* * 13-28 * *", {"2023-08-22 14:07:12",
                                "2023-08-22 14:08:00", "2023-08-22 14:09:00",
                                "2023-08-22 14:10:00", "2023-08-22 14:11:00"})) {
        return false;
    }
    if (!func("* * 1-3/2 * *", {"2023-08-22 14:07:12",
                                "2023-09-01 00:00:00", "2023-09-01 00:01:00",
                                "2023-09-01 00:02:00", "2023-09-01 00:03:00"})) {
        return false;
    }
    if (!func("* * 15-20/2 * *", {"2023-08-22 14:07:12",
                                  "2023-09-15 00:00:00", "2023-09-15 00:01:00",
                                  "2023-09-15 00:02:00", "2023-09-15 00:03:00"})) {
        return false;
    }
    if (!func("* * 13-28/2 * *", {"2023-08-22 14:07:12",
                                  "2023-08-23 00:00:00", "2023-08-23 00:01:00",
                                  "2023-08-23 00:02:00", "2023-08-23 00:03:00"})) {
        return false;
    }

    // day list values.
    if (!func("* * 1,2,3 * *", {"2023-08-22 01:00:00",
                                "2023-09-01 00:00:00", "2023-09-01 00:01:00",
                                "2023-09-01 00:02:00", "2023-09-01 00:03:00"})) {
        return false;
    }
    if (!func("* * 1,2,3 * *", {"2023-08-22 14:07:12",
                                "2023-09-01 00:00:00", "2023-09-01 00:01:00",
                                "2023-09-01 00:02:00", "2023-09-01 00:03:00"})) {
        return false;
    }
    if (!func("* * 1,3,7 * *", {"2023-08-22 14:00:05",
                                "2023-09-01 00:00:00", "2023-09-01 00:01:00",
                                "2023-09-01 00:02:00", "2023-09-01 00:03:00"})) {
        return false;
    }
    if (!func("* * 6,9,12 * *", {"2023-08-22 04:00:05",
                                 "2023-09-06 00:00:00", "2023-09-06 00:01:00",
                                 "2023-09-06 00:02:00", "2023-09-06 00:03:00"})) {
        return false;
    }

    if (!func("* * 1,3,2 * *", {"2023-08-22 01:00:00",
                                "2023-09-01 00:00:00", "2023-09-01 00:01:00",
                                "2023-09-01 00:02:00", "2023-09-01 00:03:00"})) {
        return false;
    }
    if (!func("* * 1,3,2 * *", {"2023-08-22 14:07:12",
                                "2023-09-01 00:00:00", "2023-09-01 00:01:00",
                                "2023-09-01 00:02:00", "2023-09-01 00:03:00"})) {
        return false;
    }
    if (!func("* * 1,7,3 * *", {"2023-08-22 14:00:05",
                                "2023-09-01 00:00:00", "2023-09-01 00:01:00",
                                "2023-09-01 00:02:00", "2023-09-01 00:03:00"})) {
        return false;
    }
    if (!func("* * 6,12,9 * *", {"2023-08-22 04:00:05",
                                 "2023-09-06 00:00:00", "2023-09-06 00:01:00",
                                 "2023-09-06 00:02:00", "2023-09-06 00:03:00"})) {
        return false;
    }

    // month any values.
    if (!func("* * * */1 *", {"2023-08-22 14:07:12",
                              "2023-08-22 14:08:00", "2023-08-22 14:09:00",
                              "2023-08-22 14:10:00", "2023-08-22 14:11:00"})) {
        return false;
    }
    if (!func("* * * */2 *", {"2023-08-22 14:07:12",
                              "2023-09-01 00:00:00", "2023-09-01 00:01:00",
                              "2023-09-01 00:02:00", "2023-09-01 00:03:00"})) {
        return false;
    }
    if (!func("* * * */2 *", {"2023-08-23 15:07:12",
                              "2023-09-01 00:00:00", "2023-09-01 00:01:00",
                              "2023-09-01 00:02:00", "2023-09-01 00:03:00"})) {
        return false;
    }
    if (!func("* * * */5 *", {"2023-08-28 10:48:12",
                              "2023-11-01 00:00:00", "2023-11-01 00:01:00",
                              "2023-11-01 00:02:00", "2023-11-01 00:03:00"})) {
        return false;
    }
    if (!func("* * * */3 *", {"2023-08-28 10:48:12",
                              "2023-10-01 00:00:00", "2023-10-01 00:01:00",
                              "2023-10-01 00:02:00", "2023-10-01 00:03:00"})) {
        return false;
    }
    if (!func("* * * */12 *", {"2023-08-28 10:48:12",
                               "2024-01-01 00:00:00", "2024-01-01 00:01:00",
                               "2024-01-01 00:02:00", "2024-01-01 00:03:00"})) {
        return false;
    }
    if (!func("* * * */13 *", {"2023-08-28 10:48:12",
                               "2024-01-01 00:00:00", "2024-01-01 00:01:00",
                               "2024-01-01 00:02:00", "2024-01-01 00:03:00"})) {
        return false;
    }

    // month single value.
    if (!func("* * * 1 *", {"2023-08-22 14:07:12",
                            "2024-01-01 00:00:00", "2024-01-01 00:01:00",
                            "2024-01-01 00:02:00", "2024-01-01 00:03:00"})) {
        return false;
    }
    if (!func("* * * 2 *", {"2023-08-22 14:07:12",
                            "2024-02-01 00:00:00", "2024-02-01 00:01:00",
                            "2024-02-01 00:02:00", "2024-02-01 00:03:00"})) {
        return false;
    }
    if (!func("* * * 2 *", {"2023-08-22 15:01:00",
                            "2024-02-01 00:00:00", "2024-02-01 00:01:00",
                            "2024-02-01 00:02:00", "2024-02-01 00:03:00"})) {
        return false;
    }
    if (!func("* * * 5 *", {"2023-08-22 14:07:12",
                            "2024-05-01 00:00:00", "2024-05-01 00:01:00",
                            "2024-05-01 00:02:00", "2024-05-01 00:03:00"})) {
        return false;
    }
    if (!func("* * * 12 *", {"2023-08-22 14:07:12",
                             "2023-12-01 00:00:00", "2023-12-01 00:01:00",
                             "2023-12-01 00:02:00", "2023-12-01 00:03:00"})) {
        return false;
    }

    // month range values.
    if (!func("* * * 1-3 *", {"2023-08-22 14:07:12",
                              "2024-01-01 00:00:00", "2024-01-01 00:01:00",
                              "2024-01-01 00:02:00", "2024-01-01 00:03:00"})) {
        return false;
    }
    if (!func("* * * 5-12 *", {"2023-08-22 14:07:12",
                               "2023-08-22 14:08:00", "2023-08-22 14:09:00",
                               "2023-08-22 14:10:00", "2023-08-22 14:11:00"})) {
        return false;
    }
    if (!func("* * * 9-11 *", {"2023-08-22 14:07:12",
                               "2023-09-01 00:00:00", "2023-09-01 00:01:00",
                               "2023-09-01 00:02:00", "2023-09-01 00:03:00"})) {
        return false;
    }
    if (!func("* * * 1-3/2 *", {"2023-08-22 14:07:12",
                                "2024-01-01 00:00:00", "2024-01-01 00:01:00",
                                "2024-01-01 00:02:00", "2024-01-01 00:03:00"})) {
        return false;
    }
    if (!func("* * * 5-12/2 *", {"2023-08-22 14:07:12",
                                 "2023-09-01 00:00:00", "2023-09-01 00:01:00",
                                 "2023-09-01 00:02:00", "2023-09-01 00:03:00"})) {
        return false;
    }
    if (!func("* * * 9-11/2 *", {"2023-08-22 14:07:12",
                                 "2023-09-01 00:00:00", "2023-09-01 00:01:00",
                                 "2023-09-01 00:02:00", "2023-09-01 00:03:00"})) {
        return false;
    }

    // month list values.
    if (!func("* * * 1,2,3 *", {"2023-08-22 01:00:00",
                                "2024-01-01 00:00:00", "2024-01-01 00:01:00",
                                "2024-01-01 00:02:00", "2024-01-01 00:03:00"})) {
        return false;
    }
    if (!func("* * * 1,2,3 *", {"2023-08-22 14:07:12",
                                "2024-01-01 00:00:00", "2024-01-01 00:01:00",
                                "2024-01-01 00:02:00", "2024-01-01 00:03:00"})) {
        return false;
    }
    if (!func("* * * 1,3,7 *", {"2023-08-22 14:00:05",
                                "2024-01-01 00:00:00", "2024-01-01 00:01:00",
                                "2024-01-01 00:02:00", "2024-01-01 00:03:00"})) {
        return false;
    }
    if (!func("* * * 6,9,12 *", {"2023-08-22 04:00:05",
                                 "2023-09-01 00:00:00", "2023-09-01 00:01:00",
                                 "2023-09-01 00:02:00", "2023-09-01 00:03:00"})) {
        return false;
    }
    if (!func("* * * 9,10,12 *", {"2023-08-22 04:00:05",
                                  "2023-09-01 00:00:00", "2023-09-01 00:01:00",
                                  "2023-09-01 00:02:00", "2023-09-01 00:03:00"})) {
        return false;
    }

    if (!func("* * * 1,3,2 *", {"2023-08-22 01:00:00",
                                "2024-01-01 00:00:00", "2024-01-01 00:01:00",
                                "2024-01-01 00:02:00", "2024-01-01 00:03:00"})) {
        return false;
    }
    if (!func("* * * 1,3,2 *", {"2023-08-22 14:07:12",
                                "2024-01-01 00:00:00", "2024-01-01 00:01:00",
                                "2024-01-01 00:02:00", "2024-01-01 00:03:00"})) {
        return false;
    }
    if (!func("* * * 1,7,3 *", {"2023-08-22 14:00:05",
                                "2024-01-01 00:00:00", "2024-01-01 00:01:00",
                                "2024-01-01 00:02:00", "2024-01-01 00:03:00"})) {
        return false;
    }
    if (!func("* * * 6,12,9 *", {"2023-08-22 04:00:05",
                                 "2023-09-01 00:00:00", "2023-09-01 00:01:00",
                                 "2023-09-01 00:02:00", "2023-09-01 00:03:00"})) {
        return false;
    }
    if (!func("* * * 9,12,10 *", {"2023-08-22 04:00:05",
                                  "2023-09-01 00:00:00", "2023-09-01 00:01:00",
                                  "2023-09-01 00:02:00", "2023-09-01 00:03:00"})) {
        return false;
    }

    // week any values.
    if (!func("* * * * */1", {"2023-08-29 14:07:12",
                              "2023-08-29 14:08:00", "2023-08-29 14:09:00",
                              "2023-08-29 14:10:00", "2023-08-29 14:11:00"})) {
        return false;
    }
    if (!func("* * * * */2", {"2023-08-29 14:07:12",
                              "2023-08-29 14:08:00", "2023-08-29 14:09:00",
                              "2023-08-29 14:10:00", "2023-08-29 14:11:00"})) {
        return false;
    }
    if (!func("* * * * */5", {"2023-08-29 10:48:12",
                              "2023-09-01 00:00:00", "2023-09-01 00:01:00",
                              "2023-09-01 00:02:00", "2023-09-01 00:03:00"})) {
        return false;
    }
    if (!func("* * * * */3", {"2023-08-29 10:48:12",
                              "2023-08-30 00:00:00", "2023-08-30 00:01:00",
                              "2023-08-30 00:02:00", "2023-08-30 00:03:00"})) {
        return false;
    }
    if (!func("* * * * */6", {"2023-08-29 10:48:12",
                              "2023-09-02 00:00:00", "2023-09-02 00:01:00",
                              "2023-09-02 00:02:00", "2023-09-02 00:03:00"})) {
        return false;
    }
    if (!func("* * * * */7", {"2023-08-29 10:48:12",
                              "2023-09-03 00:00:00", "2023-09-03 00:01:00",
                              "2023-09-03 00:02:00", "2023-09-03 00:03:00"})) {
        return false;
    }
    if (!func("* * * * */13", {"2023-08-29 10:48:12",
                               "2023-09-03 00:00:00", "2023-09-03 00:01:00",
                               "2023-09-03 00:02:00", "2023-09-03 00:03:00"})) {
        return false;
    }

    // week single value.
    if (!func("* * * * 1", {"2023-08-29 14:07:12",
                            "2023-09-04 00:00:00", "2023-09-04 00:01:00",
                            "2023-09-04 00:02:00", "2023-09-04 00:03:00"})) {
        return false;
    }
    if (!func("* * * * 2", {"2023-08-29 14:07:12",
                            "2023-08-29 14:08:00", "2023-08-29 14:09:00",
                            "2023-08-29 14:10:00", "2023-08-29 14:11:00"})) {
        return false;
    }
    if (!func("* * * * 2", {"2023-08-30 09:07:12",
                            "2023-09-05 00:00:00", "2023-09-05 00:01:00",
                            "2023-09-05 00:02:00", "2023-09-05 00:03:00"})) {
        return false;
    }
    if (!func("* * * * 5", {"2023-08-29 14:07:12",
                            "2023-09-01 00:00:00", "2023-09-01 00:01:00",
                            "2023-09-01 00:02:00", "2023-09-01 00:03:00"})) {
        return false;
    }
    if (!func("* * * * 6", {"2023-08-29 14:07:12",
                            "2023-09-02 00:00:00", "2023-09-02 00:01:00",
                            "2023-09-02 00:02:00", "2023-09-02 00:03:00"})) {
        return false;
    }

    // week range values.
    if (!func("* * * * 1-3", {"2023-08-29 14:07:12",
                              "2023-08-29 14:08:00", "2023-08-29 14:09:00",
                              "2023-08-29 14:10:00", "2023-08-29 14:11:00"})) {
        return false;
    }
    if (!func("* * * * 1-1", {"2023-08-29 14:07:12",
                              "2023-09-04 00:00:00", "2023-09-04 00:01:00",
                              "2023-09-04 00:02:00", "2023-09-04 00:03:00"})) {
        return false;
    }
    if (!func("* * * * 4-6", {"2023-08-29 14:07:12",
                              "2023-08-31 00:00:00", "2023-08-31 00:01:00",
                              "2023-08-31 00:02:00", "2023-08-31 00:03:00"})) {
        return false;
    }
    if (!func("* * * * 1-1/2", {"2023-08-29 14:07:12",
                                "2023-09-04 00:00:00", "2023-09-04 00:01:00",
                                "2023-09-04 00:02:00", "2023-09-04 00:03:00"})) {
        return false;
    }
    if (!func("* * * * 1-3/2", {"2023-08-29 14:07:12",
                                "2023-08-30 00:00:00", "2023-08-30 00:01:00",
                                "2023-08-30 00:02:00", "2023-08-30 00:03:00"})) {
        return false;
    }
    if (!func("* * * * 4-6/2", {"2023-08-29 14:07:12",
                                "2023-08-31 00:00:00", "2023-08-31 00:01:00",
                                "2023-08-31 00:02:00", "2023-08-31 00:03:00"})) {
        return false;
    }

    // week list values.
    if (!func("* * * * 1,2,3", {"2023-08-29 18:04:30",
                                "2023-08-29 18:05:00", "2023-08-29 18:06:00",
                                "2023-08-29 18:07:00", "2023-08-29 18:08:00"})) {
        return false;
    }
    if (!func("* * * * 1,3,6", {"2023-08-29 14:00:05",
                                "2023-08-30 00:00:00", "2023-08-30 00:01:00",
                                "2023-08-30 00:02:00", "2023-08-30 00:03:00"})) {
        return false;
    }
    if (!func("* * * * 3,4,6", {"2023-08-29 04:00:05",
                                "2023-08-30 00:00:00", "2023-08-30 00:01:00",
                                "2023-08-30 00:02:00", "2023-08-30 00:03:00"})) {
        return false;
    }
    if (!func("* * * * 0,1", {"2023-08-29 04:00:05",
                              "2023-09-03 00:00:00", "2023-09-03 00:01:00",
                              "2023-09-03 00:02:00", "2023-09-03 00:03:00"})) {
        return false;
    }

    if (!func("* * * * 3,1,2", {"2023-08-29 18:04:30",
                                "2023-08-29 18:05:00", "2023-08-29 18:06:00",
                                "2023-08-29 18:07:00", "2023-08-29 18:08:00"})) {
        return false;
    }
    if (!func("* * * * 1,6,3", {"2023-08-29 14:00:05",
                                "2023-08-30 00:00:00", "2023-08-30 00:01:00",
                                "2023-08-30 00:02:00", "2023-08-30 00:03:00"})) {
        return false;
    }
    if (!func("* * * * 3,6,4", {"2023-08-29 04:00:05",
                                "2023-08-30 00:00:00", "2023-08-30 00:01:00",
                                "2023-08-30 00:02:00", "2023-08-30 00:03:00"})) {
        return false;
    }
    if (!func("* * * * 1,0", {"2023-08-29 04:00:05",
                              "2023-09-03 00:00:00", "2023-09-03 00:01:00",
                              "2023-09-03 00:02:00", "2023-09-03 00:03:00"})) {
        return false;
    }

    // multi groups.
    if (!func("30 6 * * *", {"2023-08-29 04:00:05",
                             "2023-08-29 06:30:00", "2023-08-30 06:30:00",
                             "2023-08-31 06:30:00", "2023-09-01 06:30:00"})) {
        return false;
    }
    if (!func("30 6 * * *", {"2023-08-29 18:15:05",
                             "2023-08-30 06:30:00", "2023-08-31 06:30:00",
                             "2023-09-01 06:30:00", "2023-09-02 06:30:00"})) {
        return false;
    }
    if (!func("30 6 5 * *", {"2023-08-29 18:15:05",
                             "2023-09-05 06:30:00", "2023-10-05 06:30:00",
                             "2023-11-05 06:30:00", "2023-12-05 06:30:00"})) {
        return false;
    }
    if (!func("30 6 30 * *", {"2023-08-29 18:15:05",
                              "2023-08-30 06:30:00", "2023-09-30 06:30:00",
                              "2023-10-30 06:30:00", "2023-11-30 06:30:00"})) {
        return false;
    }
    if (!func("30 6 30 1 *", {"2023-08-30 08:26:05",
                              "2024-01-30 06:30:00", "2025-01-30 06:30:00",
                              "2026-01-30 06:30:00", "2027-01-30 06:30:00"})) {
        return false;
    }
    if (!func("30 6 * 1 2", {"2023-08-30 08:26:05",
                             "2024-01-02 06:30:00", "2024-01-09 06:30:00",
                             "2024-01-16 06:30:00", "2024-01-23 06:30:00"})) {
        return false;
    }
    if (!func("30 6 30 * *", {"2023-08-30 08:26:05",
                              "2023-09-30 06:30:00", "2023-10-30 06:30:00",
                              "2023-11-30 06:30:00", "2023-12-30 06:30:00"})) {
        return false;
    }
    if (!func("30 6 30 * *", {"2023-12-30 06:30:00",
                              "2024-01-30 06:30:00", "2024-03-30 06:30:00",
                              "2024-04-30 06:30:00", "2024-05-30 06:30:00"})) {
        return false;
    }
    if (!func("30 6 30 1 2", {"2023-08-30 08:26:05",
                              "2024-01-02 06:30:00", "2024-01-09 06:30:00",
                              "2024-01-16 06:30:00", "2024-01-23 06:30:00"})) {
        return false;
    }
    if (!func("30 6 31 * *", {"2023-08-30 08:26:05",
                              "2023-08-31 06:30:00", "2023-10-31 06:30:00",
                              "2023-12-31 06:30:00", "2024-01-31 06:30:00"})) {
        return false;
    }

    {
        static const char *str = "30 6 31 * *";
        CronSchedule schedule;
        if (!CronSchedule::parse(str, schedule)) {
            printf("Failed to parse, time str: %s\n", str);
            return false;
        }
        DateTime current;
        DateTime::parse("2023-08-30 08:26:05", current);
        DateTime expect = schedule.getNextTime(current);
        if (expect.toString() != "2023-08-31 06:30:00") {
            return false;
        }
    }

    return true;
}

bool testCronSchedule_specialTimes() {
    typedef struct SpecialEntry {
        const char *name;
        const char *tokens;
    } SpecialEntry;
    static const SpecialEntry SpecAry[] = {
            "@yearly",      "0 0 1 1 *",
            "@annually",    "0 0 1 1 *",
            "@monthly",     "0 0 1 * *",
            "@weekly",      "0 0 * * 0",
            "@daily",       "0 0 * * *",
            "@midnight",    "0 0 * * *",
            "@hourly",      "0 * * * *",
            "",             ""
    };

    const SpecialEntry *e = SpecAry;
    for (;;) {
        if (!e->tokens[0])
            break;

        CronSchedule schedule;
        if (!CronSchedule::parse(e->name, schedule)) {
            return false;
        }
        if (!schedule.field().equals(e->tokens)) {
            return false;
        }
        e++;
    }

    return true;
}

int main() {
//    CronSchedule schedule;
//    CronSchedule::parse("* * */2 * *", schedule);
//    DateTime time = schedule.getNextTime();
//    printf("time: %s\n", time.toString().c_str());
//    return -1;
    if (!testCronSchedule_parse()) {
        return 1;
    }
    if (!testCronSchedule_toString()) {
        return 2;
    }
    if (!testCronSchedule_getNext()) {
        return 3;
    }
    if (!testCronSchedule_specialTimes()) {
        return 4;
    }

    return 0;
}
