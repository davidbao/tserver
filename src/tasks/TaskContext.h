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

class Action : public IEvaluation<Action>, public IEquatable<Action> {
public:
    String name;

    Action();

    explicit Action(const String &name);

    Action(const Action &other);

    bool equals(const Action &other) const override;

    void evaluates(const Action &other) override;
};

typedef List<Action> Actions;

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
    String datasource;
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

class Task {
public:
    String name;
    String app;
    String param;
    Actions actions;
    Tables tables;

    explicit Task(const String &path);

    virtual ~Task();

    virtual bool isTimeUp() = 0;

    virtual DataRow toDataRow(const DataTable &table) const = 0;

    virtual String type() const = 0;

    virtual JsonNode toJsonNode() const = 0;

    bool execute();

private:
    bool startApp();

    bool appExists() const;

private:
    String _path;
};

class CycleTask : public Task {
public:
    TimeSpan interval;

    explicit CycleTask(const String &path);

    ~CycleTask() override;

    bool isTimeUp() override;

    DataRow toDataRow(const DataTable &table) const override;

    String type() const override;

    JsonNode toJsonNode() const override;

private:
    uint32_t _tick;
};

class TimeTask : public Task {
public:
    DateTime time;
    String repeatType;
    String repeatValue;

    explicit TimeTask(const String &path);

    ~TimeTask() override;

    bool isTimeUp() override;

    DataRow toDataRow(const DataTable &table) const override;

    String type() const override;

    JsonNode toJsonNode() const override;

public:
    static StringArray allRepeatTypes();

private:
    static bool parseWeeks(const String &value, Vector<DayOfWeek> &weeks);

    static bool parseMonths(const String &value, Vector<int> &months);

    static bool parseQuarters(const String &value, Vector<int> &quarters);
};

typedef PList<Task> Tasks;

class DataSource;

class ITaskProvider : public IService {
public:
    explicit ITaskProvider(const DataSource *ds);

    ~ITaskProvider() override = default;

    virtual bool getValue(const Table &table, DataTable &t) = 0;

protected:
    const DataSource *_dataSource;
};

class ITaskExecution : public IService {
public:
    explicit ITaskExecution() = default;

    ~ITaskExecution() override = default;

    virtual bool execute(const Task *task) = 0;
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
