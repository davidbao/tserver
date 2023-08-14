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
#include "data/Dictionary.h"
#include "../Style.h"
#include <cassert>

using namespace Data;
using namespace Http;
using namespace Database;
using namespace System;

// for application.yml
#define TaskPrefix "summer.task."
#define SimDbPrefix TaskPrefix "database."

// for yml file.
#define MaxTaskCount 1000
#define TasksPrefix "tasks[%d]."
#define SchedulePrefix TasksPrefix "schedule."
#define ExecutionPrefix TasksPrefix "execution."

// for database.
#define TaskTableName "task"

class Execution : public IEvaluation<Execution>, public IEquatable<Execution>, public ICloneable<Execution> {
public:
    enum Result {
        Succeed = 0,
        FailedToStartProcess,
        NotFound,
        Timeout,
        FailedToExecuteSql,
    };

    Execution();

    explicit Execution(bool sync, const TimeSpan &timeout);

    bool equals(const Execution &other) const override;

    void evaluates(const Execution &other) override;

    virtual String type() const = 0;

    bool sync() const;

    const TimeSpan &timeout() const;

    virtual Result execute() = 0;

    virtual JsonNode toJsonNode() const = 0;

    virtual void removeFile();

    virtual bool checkFile(const String &md5) const;

    virtual String currentFile() const = 0;

    static String currentPath();

    String toString() const;

protected:
    bool _sync;
    TimeSpan _timeout;
};

class AppExecution : public Execution {
public:
    AppExecution() = default;

    explicit AppExecution(bool sync, const TimeSpan &timeout,
                          const String &app, const String &param);

    ~AppExecution() override = default;

    bool equals(const Execution &other) const override;

    void evaluates(const Execution &other) override;

    Execution *clone() const override;

    String type() const override;

    const String &app() const;

    const String &param() const;

    Result execute() override;

    JsonNode toJsonNode() const override;

protected:
    String currentFile() const override;

private:
    String _app;
    String _param;
};

class SqlExecution : public Execution {
public:
    SqlExecution() = default;

    explicit SqlExecution(bool sync, const TimeSpan &timeout,
                          const String &text, bool sql);

    ~SqlExecution() override = default;

    bool equals(const Execution &other) const override;

    void evaluates(const Execution &other) override;

    Execution *clone() const override;

    String type() const override;

    const String &sql() const;

    const String &fileName() const;

    bool isFile() const;

    Result execute() override;

    JsonNode toJsonNode() const override;

protected:
    String currentFile() const override;

private:
    String _sql;
    String _fileName;
};

class PythonExecution : public Execution {
public:
    PythonExecution() = default;

    explicit PythonExecution(bool sync, const TimeSpan &timeout,
                             const String &script);

    explicit PythonExecution(bool sync, const TimeSpan &timeout,
                             const String &fileName, const String &param);

    ~PythonExecution() override = default;

    bool equals(const Execution &other) const override;

    void evaluates(const Execution &other) override;

    Execution *clone() const override;

    String type() const override;

    const String &script() const;

    const String &fileName() const;

    const String &param() const;

    bool isFile() const;

    Result execute() override;

    JsonNode toJsonNode() const override;

protected:
    String currentFile() const override;

private:
    String _script;

    String _fileName;
    String _param;
};

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

    ~CycleSchedule() override = default;

    bool equals(const Schedule &other) const override;

    void evaluates(const Schedule &other) override;

    Schedule *clone() const override;

    String type() const override;

    const TimeSpan &interval() const;

    bool isTimeUp(const String &taskName) override;

    JsonNode toJsonNode() const override;

private:
    TimeSpan _interval;
};

class TimingSchedule : public Schedule {
public:
    TimingSchedule();

    TimingSchedule(std::initializer_list<KeyValuePair<String, String>> list);

    ~TimingSchedule() override = default;

    bool equals(const Schedule &other) const override;

    void evaluates(const Schedule &other) override;

    Schedule *clone() const override;

    String type() const override;

    const DateTime &time() const;

    const String &repeatType() const;

    const String &repeatValue() const;

    bool isTimeUp(const String &taskName) override;

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

    bool isTimeUp(const String &taskName, Types type);

public:
    static StringArray allRepeatTypes();

private:
    static bool isTimeUp(const String &taskName, const DateTime &now, const DateTime &time, const TimeSpan &deadZone);

    static bool parseWeeks(const String &value, Vector<DayOfWeek> &weeks);

    static bool parseMonths(const String &value, Vector<int> &months);

    static bool parseQuarters(const String &value, Vector<int> &quarters);

    static bool parseYears(const String &value, Vector<int> &years);

private:
    DateTime _time;
    String _repeatType;
    String _repeatValue;

    TimeSpan _minDeadZone;
    TimeSpan _secDeadZone;
};

class Crontab : public IEvaluation<Crontab>, public IEquatable<Crontab> {
public:
    String name;
    bool enable;

    Crontab();

    explicit Crontab(const String &name, Schedule *schedule, Execution *execution);

    Crontab(const Crontab &other);

    ~Crontab() override;

    bool equals(const Crontab &other) const override;

    void evaluates(const Crontab &other) override;

    Crontab &operator=(const Crontab &other);

    const Schedule *schedule() const;

    const Execution *execution() const;

    bool isTimeUp();

    bool execute();

    DataRow toDataRow(const DataTable &table) const;

    JsonNode toJsonNode() const;

    void removeFile();

    bool isValid() const;

    bool findName(const String &n) const;

    void updateYmlProperties(YmlNode::Properties &properties, int pos) const;

    static void removeYmlProperties(YmlNode::Properties &properties, int pos);

    String toInsertSqlStr(const String &prefix) const;

    String toReplaceSqlStr(const String &prefix) const;

public:
    static bool parse(const StringMap &request, Crontab &crontab);

    static bool parse(const YmlNode::Properties &properties, int pos, Crontab &crontab);

    static bool parse(const DataRow &row, Crontab &crontab);

    static String toSelectSqlStr(const String &prefix, const String &name);

    static String toSelectSqlStr(const String &prefix, int offset);

    static String toSelectSqlStr(const String &prefix, const SqlSelectFilter &filter);

    static String toCountSqlStr(const String &prefix, const SqlSelectFilter &filter);

    static String toDeleteSqlStr(const String &prefix, const String &taskName);

private:
    static bool parseSchedule(const String &str, Schedule *&schedule);

    static bool parseExecution(const String &str, Execution *&execution);

    static String getTableName(const String &prefix, const String &tableName);

private:
    Schedule *_schedule;
    Execution *_execution;
};

typedef List<Crontab> Crontabs;

class ITaskStorage {
public:
    ITaskStorage() = default;

    virtual ~ITaskStorage() = default;

    virtual bool load() = 0;

    virtual bool getTask(const String &name, Crontab &crontab) = 0;

    virtual bool getTask(int pos, Crontab &crontab) = 0;

    virtual bool getTasks(const SqlSelectFilter &filter, DataTable &table) = 0;

    virtual bool addTask(const StringMap &request, StringMap &response) = 0;

    virtual bool updateTask(const StringMap &request, StringMap &response) = 0;

    virtual bool removeTask(const StringMap &request, StringMap &response) = 0;
};

class ITaskCache : public IService {
public:
    ITaskCache() = default;

    virtual String getValue(const String &key) = 0;

    virtual bool setValue(const String &key, const String &value) = 0;
};

#endif //TSERVER_TASKCONTEXT_H
