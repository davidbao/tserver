//
//  TaskContext.h
//  tserver
//
//  Created by baowei on 2022/12/17.
//  Copyright (c) 2022 com. All rights reserved.
//

#ifndef TSERVER_TASKCONTEXT_H
#define TSERVER_TASKCONTEXT_H

#include "data/PList.h"
#include "json/JsonNode.h"
#include "http/HttpContent.h"
#include "database/DataTable.h"
#include "database/SqlSelectFilter.h"
#include "system/ServiceFactory.h"
#include "../Style.h"

using namespace Data;
using namespace Http;
using namespace Database;
using namespace System;

class Execution {
public:
    Execution() = default;

    virtual ~Execution() = default;

    virtual bool execute() = 0;

    virtual JsonNode toJsonNode() const = 0;
};

class AppExecution : public Execution {
public:
    explicit AppExecution(const String &path, const String &app, const String &param);

    ~AppExecution() override = default;

    bool execute() override;

    JsonNode toJsonNode() const override;

private:
    String _app;
    String _param;

    String _path;
};

class SqlExecution : public Execution {
public:
    explicit SqlExecution(const String &text, bool sql);

    ~SqlExecution() override = default;

    bool execute() override;

    JsonNode toJsonNode() const override;

private:
    String _sql;
    String _fileName;
};

class PythonExecution : public Execution {
public:
    explicit PythonExecution(const String &script);

    explicit PythonExecution(const String &fileName, const String &param);

    ~PythonExecution() override = default;

    bool execute() override;

    JsonNode toJsonNode() const override;

private:
    String _script;

    String _fileName;
    String _param;
};

class Schedule {
public:
    Schedule() = default;

    virtual ~Schedule() = default;

    virtual bool isTimeUp() = 0;

    virtual JsonNode toJsonNode() const = 0;
};

class CycleSchedule : public Schedule {
public:
    explicit CycleSchedule(const TimeSpan &interval);

    ~CycleSchedule() override = default;

    bool isTimeUp() override;

    JsonNode toJsonNode() const override;

private:
    TimeSpan _interval;

    uint32_t _tick;
};

class TimingSchedule : public Schedule {
public:
    TimingSchedule(const DateTime &time, const String &repeatType, const String &repeatValue);

    ~TimingSchedule() override = default;

    bool isTimeUp() override;

    JsonNode toJsonNode() const override;

public:
    static StringArray allRepeatTypes();

private:
    static bool parseWeeks(const String &value, Vector<DayOfWeek> &weeks);

    static bool parseMonths(const String &value, Vector<int> &months);

    static bool parseQuarters(const String &value, Vector<int> &quarters);

    static bool parseYears(const String &value, Vector<int> &years);

private:
    DateTime _time;
    String _repeatType;
    String _repeatValue;

    uint32_t _tickSeconds;
    uint32_t _tickMinutes;
};

class Task {
public:
    explicit Task(const String &name, Schedule *schedule, Execution *execution);

    ~Task();

    const String &name() const;

    bool isTimeUp();

    bool execute();

    DataRow toDataRow(const DataTable &table) const;

    JsonNode toJsonNode() const;

private:
    String _name;
    Schedule *_schedule;
    Execution *_execution;
};

typedef PList<Task> Tasks;

#endif //TSERVER_TASKCONTEXT_H
