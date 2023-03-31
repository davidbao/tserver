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
    explicit Execution(bool sync, const TimeSpan &timeout);

    virtual ~Execution() = default;

    virtual String type() const = 0;

    bool sync() const;

    const TimeSpan &timeout() const;

    virtual bool execute() = 0;

    virtual JsonNode toJsonNode() const = 0;

    virtual void removeFile();

    virtual bool checkFile(const String &md5) const;

    virtual String currentFile() const = 0;

    String currentPath() const;

    String toString() const;

protected:
    bool _sync;
    TimeSpan _timeout;
};

class AppExecution : public Execution {
public:
    explicit AppExecution(bool sync, const TimeSpan &timeout,
                          const String &app, const String &param);

    ~AppExecution() override = default;

    String type() const override;

    const String &app() const;

    const String &param() const;

    bool execute() override;

    JsonNode toJsonNode() const override;

protected:
    String currentFile() const override;

private:
    String _app;
    String _param;
};

class SqlExecution : public Execution {
public:
    explicit SqlExecution(bool sync, const TimeSpan &timeout, const String &text, bool sql);

    ~SqlExecution() override = default;

    String type() const override;

    const String &sql() const;

    const String &fileName() const;

    bool isFile() const;

    bool execute() override;

    JsonNode toJsonNode() const override;

protected:
    String currentFile() const override;

private:
    String _sql;
    String _fileName;
};

class PythonExecution : public Execution {
public:
    explicit PythonExecution(bool sync, const TimeSpan &timeout, const String &script);

    explicit PythonExecution(bool sync, const TimeSpan &timeout, const String &fileName, const String &param);

    ~PythonExecution() override = default;

    String type() const override;

    const String &script() const;

    const String &fileName() const;

    const String &param() const;

    bool isFile() const;

    bool execute() override;

    JsonNode toJsonNode() const override;

protected:
    String currentFile() const override;

private:
    String _script;

    String _fileName;
    String _param;
};

class Schedule {
public:
    Schedule() = default;

    virtual ~Schedule() = default;

    virtual String type() const = 0;

    virtual bool isTimeUp() = 0;

    virtual JsonNode toJsonNode() const = 0;

    String toString() const;
};

class CycleSchedule : public Schedule {
public:
    explicit CycleSchedule(const TimeSpan &interval);

    ~CycleSchedule() override = default;

    String type() const override;

    const TimeSpan &interval() const;

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

    String type() const override;

    const DateTime &time() const;

    const String &repeatType() const;

    const String &repeatValue() const;

    bool isTimeUp() override;

    JsonNode toJsonNode() const override;

    String toTimeStr() const;

private:
    enum Types {
        S = 0,      // second
        MS = 1,     // minute + second
        HM = 2,     // hour + minute
        DHM = 3,    // day + hour + minute
        MDHM = 4    // month + day + hour + minute
    };

    bool isTimeUp(Types type);

    bool isTimeUp(const DateTime &now, const DateTime &time, const TimeSpan &deadZone);

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

    uint32_t _lastTimeUpTick;

private:
    static const TimeSpan MinDeadZone;
    static const TimeSpan SecDeadZone;
};

class Crontab {
public:
    explicit Crontab(const String &name, Schedule *schedule, Execution *execution);

    ~Crontab();

    const String &name() const;

    const Schedule *schedule() const;

    const Execution *execution() const;

    bool isTimeUp();

    bool execute();

    DataRow toDataRow(const DataTable &table) const;

    JsonNode toJsonNode() const;

    void removeFile();

    bool isValid() const;

public:
    static bool parseSchedule(const String &str, Schedule *&schedule);

    static bool parseExecution(const String &str, Execution *&execution);

private:
    String _name;
    Schedule *_schedule;
    Execution *_execution;
};

typedef PList<Crontab> Crontabs;

#endif //TSERVER_TASKCONTEXT_H
