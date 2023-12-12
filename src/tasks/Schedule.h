//
//  Schedule.h
//  tserver
//
//  Created by baowei on 2023/8/21.
//  Copyright (c) 2023 com. All rights reserved.
//

#ifndef TSERVER_SCHEDULE_H
#define TSERVER_SCHEDULE_H

#include "data/PList.h"
#include "json/JsonNode.h"
#include "http/HttpContent.h"
#include "database/DataTable.h"
#include "database/SqlSelectFilter.h"
#include "system/ServiceFactory.h"
#include "data/Dictionary.h"
#include "../Style.h"
#include <cassert>
#include "Execution.h"

using namespace Data;
using namespace Http;
using namespace Database;
using namespace System;

class Schedule : public IEvaluation<Schedule>, public IEquatable<Schedule>, public ICloneable<Schedule> {
public:
    Schedule() = default;

    virtual String type() const = 0;

    virtual bool isTimeUp(const String &taskName) = 0;

    virtual JsonNode toJsonNode() const = 0;

    String toString() const;
};

class CycleSchedule : public Schedule {
public:
    CycleSchedule() = default;

    explicit CycleSchedule(const TimeSpan &interval);

    CycleSchedule(const CycleSchedule &other);

    CycleSchedule &operator=(const CycleSchedule &other);

    ~CycleSchedule() override = default;

    bool equals(const Schedule &other) const override;

    void evaluates(const Schedule &other) override;

    Schedule *clone() const override;

    String type() const override;

    const TimeSpan &interval() const;

    bool isTimeUp(const String &taskName) override;

    JsonNode toJsonNode() const override;

protected:
    TimeSpan _interval;
};

struct CronLine {
    /* ordered by size, not in natural order. makes code smaller: */
    char cl_Dow[7];                 /* 0-6, beginning sunday */
    char cl_Mons[12];               /* 0-11 */
    char cl_Hrs[24];                /* 0-23 */
    char cl_Days[31];               /* 0-30 */
    char cl_Mins[60];               /* 0-59 */

    CronLine() : cl_Dow{0}, cl_Mons{0}, cl_Hrs{0}, cl_Days{0}, cl_Mins{0} {
//        memset(this, 0, sizeof(CronLine));
    }
};

class CronSchedule : public Schedule {
public:
    CronSchedule() = default;

    CronSchedule(const CronSchedule &other);

    CronSchedule &operator=(const CronSchedule &other);

    ~CronSchedule() override = default;

    bool equals(const Schedule &other) const override;

    void evaluates(const Schedule &other) override;

    Schedule *clone() const override;

    String type() const override;

    bool isTimeUp(const String &taskName) override;

    JsonNode toJsonNode() const override;

    const CronLine &cronLine() const;

    const String &field() const;

    DateTime getNextTime(const DateTime &current = DateTime::now()) const;

    Vector<DateTime> getNextTimes(const DateTime &current, int count) const;

    bool isTimeUp(const DateTime &current = DateTime::now()) const;

public:
    static bool parse(const String &str, CronSchedule& schedule);

private:
    static bool parseField(char *ary, int modValue, int off, const char *names, const char *ptr);

private:
    String _field;
    CronLine _line;

    static const int FieldCount = 5;
};

class CountSchedule : public CycleSchedule {
public:
    explicit CountSchedule();

    CountSchedule(const TimeSpan &interval, int count = 1);

    CountSchedule(const CountSchedule &other);

    CountSchedule &operator=(const CountSchedule &other);

    ~CountSchedule() override = default;

    bool equals(const Schedule &other) const override;

    void evaluates(const Schedule &other) override;

    Schedule *clone() const override;

    String type() const override;

    int count() const;

    bool isTimeUp(const String &taskName) override;

    JsonNode toJsonNode() const override;

private:
    int _count;
};

#endif //TSERVER_SCHEDULE_H
