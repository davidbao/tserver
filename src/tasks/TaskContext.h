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

class TaskAction : public IEvaluation<TaskAction>, public IEquatable<TaskAction> {
public:
    String name;
    Style style;

    TaskAction();

    explicit TaskAction(const String &name);

    TaskAction(const TaskAction &other);

    bool equals(const TaskAction &other) const override;

    void evaluates(const TaskAction &other) override;
};

typedef List<TaskAction> TaskActions;

class Table;

class Column : public IEvaluation<Column>, public IEquatable<Column> {
public:
    String name;
    String registerStr;
    Style style;
    String type;
    bool pkey;

    Column();

    explicit Column(const String &name);

    Column(const Column &other);

    bool equals(const Column &other) const override;

    void evaluates(const Column &other) override;
};

class Columns : public List<Column> {
public:
    Columns() = default;

    bool getColumn(const String &name, Column &column) const;

    DataColumns toColumns() const;
};

class Table : public IEvaluation<Table>, public IEquatable<Table> {
public:
    String name;
    String registerStr;
    Style style;
    Columns columns;
    DataTable value;

    Table();

    explicit Table(const String &name);

    Table(const Table &table);

    bool equals(const Table &other) const override;

    void evaluates(const Table &other) override;
};

typedef List<Table> Tables;

class Execution {
public:
    Execution() = default;

    virtual ~Execution() = default;

    virtual bool execute() = 0;
};

class Variable : public IEvaluation<Variable>, public IEquatable<Variable> {
public:
    typedef Vector<Variant> Values;

    String name;
    Style value;

    Variable();

    explicit Variable(const String &name);

    Variable(const Variable &other);

    bool equals(const Variable &other) const override;

    void evaluates(const Variable &other) override;

    bool isEmpty() const;

    bool updateValues();

    void clearValues();

    const Values &values() const;

private:
    static bool parseDoubleStyle(const String &style, double &minValue, double &maxValue, double &step);

public:
    static const Variable Empty;

private:
    Values _values;
};

typedef List<Variable> Variables;

class InnerExecution : public Execution {
public:
    Variables vars;
    TaskActions actions;
    Tables tables;

    InnerExecution() = default;

    ~InnerExecution() override = default;

    bool execute() override;

private:
    void updateVars();

    void clearVars();

private:

};

class AppExecution : public Execution {
public:
    explicit AppExecution(const String &path, const String &app, const String &param);

    ~AppExecution() override = default;

    bool execute() override;

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

private:
    String _sql;
    String _fileName;
};

class PythonExecution : public Execution {
public:
    explicit PythonExecution(const String &text, bool script);

    ~PythonExecution() override = default;

    bool execute() override;

private:
    String _script;
    String _fileName;
};

class Schedule {
public:
    Schedule() = default;

    virtual ~Schedule() = default;

    virtual bool isTimeUp() = 0;
};

class CycleSchedule : public Schedule {
public:
    explicit CycleSchedule(const TimeSpan &interval);

    ~CycleSchedule() override = default;

    bool isTimeUp() override;

private:
    TimeSpan _interval;

    uint32_t _tick;
};

class TimingSchedule : public Schedule {
public:
    TimingSchedule(const DateTime &time, const String &repeatType, const String &repeatValue);

    ~TimingSchedule() override = default;

    bool isTimeUp() override;

public:
    static StringArray allRepeatTypes();

private:
    static bool parseWeeks(const String &value, Vector<DayOfWeek> &weeks);

    static bool parseMonths(const String &value, Vector<int> &months);

    static bool parseQuarters(const String &value, Vector<int> &quarters);

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

//class CycleTask : public Task {
//public:
//    TimeSpan interval;
//
//    explicit CycleTask(const String &path);
//
//    ~CycleTask() override;
//
//    bool isTimeUp() override;
//
//    DataRow toDataRow(const DataTable &table) const override;
//
//    String type() const override;
//
//    JsonNode toJsonNode() const override;
//
//private:
//    uint32_t _tick;
//};
//
//class TimeTask : public Task {
//public:
//    DateTime time;
//    String repeatType;
//    String repeatValue;
//
//    explicit TimeTask(const String &path);
//
//    ~TimeTask() override;
//
//    bool isTimeUp() override;
//
//    DataRow toDataRow(const DataTable &table) const override;
//
//    String type() const override;
//
//    JsonNode toJsonNode() const override;
//
//public:
//    static StringArray allRepeatTypes();
//
//private:
//    static bool parseWeeks(const String &value, Vector<DayOfWeek> &weeks);
//
//    static bool parseMonths(const String &value, Vector<int> &months);
//
//    static bool parseQuarters(const String &value, Vector<int> &quarters);
//
//private:
//    uint32_t _tickSeconds;
//    uint32_t _tickMinutes;
//};

typedef PList<Task> Tasks;

class DataSource;

class ITaskProvider : public IService {
public:
    explicit ITaskProvider(const DataSource *ds);

    ~ITaskProvider() override = default;

    virtual bool getValue(const Column &column, DbValue &value) = 0;

    virtual bool getValue(const Table &table, DataTable &dataTable) = 0;

    virtual bool getValue(const Style &value, Variable::Values &values) = 0;

protected:
    const DataSource *_dataSource;
};

class ITaskProviders : public IService {
public:
    explicit ITaskProviders() = default;

    ~ITaskProviders() override = default;

    virtual ITaskProvider *getProvider(const String &dsName) const = 0;
};

class DataSource {
public:
    String name;

    explicit DataSource(const String &name);

    virtual ~DataSource();

    virtual void createProvider() = 0;

    ITaskProvider *provider() const;

protected:
    ITaskProvider *_provider;
};

class DbSource : public DataSource {
public:
    String userName;
    String password;
    Url url;

    explicit DbSource(const String &name);

    void createProvider() override;
};

class D5kSource : public DataSource {
public:
    int appNo;

    explicit D5kSource(const String &name);

    void createProvider() override;

public:
    static bool parseTableStyle(const Style &style, int &tableNo, int &contextNo);

    static bool parseColumnStyle(const Style &style, String &type, String &sql);
};

typedef PList<DataSource> DataSources;

#endif //TSERVER_TASKCONTEXT_H
