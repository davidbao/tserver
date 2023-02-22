//
//  TaskContext.cpp
//  tserver
//
//  Created by baowei on 2022/12/17.
//  Copyright (c) 2022 com. All rights reserved.
//

#include "IO/Path.h"
#include "IO/File.h"
#include "IO/Directory.h"
#include "diag/Trace.h"
#include "thread/Process.h"
#include "thread/TickTimeout.h"
#include "TaskContext.h"
#include "TaskDbProvider.h"
#include "TaskD5kProvider.h"

using namespace Diag;

Action::Action() = default;

Action::Action(const String &name) : name(name) {
}

Action::Action(const Action &other) {
    Action::evaluates(other);
}

bool Action::equals(const Action &other) const {
    return this->name == other.name;
}

void Action::evaluates(const Action &other) {
    this->name = other.name;
}

Column::Column() : pkey(false) {
}

Column::Column(const String &name) : pkey(false), name(name) {
}

Column::Column(const Column &other) : pkey(false) {
    Column::evaluates(other);
}

bool Column::equals(const Column &other) const {
    return this->name == other.name &&
           this->registerStr == other.registerStr &&
           this->style == other.style &&
           this->type == other.type &&
           this->pkey == other.pkey;
}

void Column::evaluates(const Column &other) {
    this->name = other.name;
    this->registerStr = other.registerStr;
    this->style = other.style;
    this->type = other.type;
    this->pkey = other.pkey;
}

bool Columns::getColumn(const String &name, Column &column) const {
    for (size_t i = 0; i < count(); ++i) {
        const Column &c = at(i);
        String n = c.style["name"];
        if (n.isNullOrEmpty()) {
            n = c.name;
        }
        if (n == name) {
            column = c;
            return true;
        }
    }
    return false;
}

DataColumns Columns::toColumns() const {
    DataColumns columns;
    for (size_t i = 0; i < count(); ++i) {
        const Column &column = at(i);
        DbValue::Type type = DbValue::fromTypeStr(column.type);
        if (type != DbValue::Null) {
            columns.add(DataColumn(column.name, type, column.pkey));
        }
    }
    return columns;
}

Table::Table() = default;

Table::Table(const String &name) : name(name) {
}

Table::Table(const Table &Table) {
    Table::evaluates(Table);
}

bool Table::equals(const Table &other) const {
    return this->name == other.name &&
           this->columns == other.columns &&
           this->datasource == other.datasource &&
           this->style == other.style;
}

void Table::evaluates(const Table &other) {
    this->name = other.name;
    this->columns = other.columns;
    this->datasource = other.datasource;
    this->style = other.style;
}

Task::Task(const String &path) : _path(path) {
}

Task::~Task() = default;

bool Task::execute() {
    if (appExists()) {
        return startApp();
    } else {
        ServiceFactory *factory = ServiceFactory::instance();
        assert(factory);
        auto te = factory->getService<ITaskExecution>();
        assert(te);
        return te->execute(this);
    }
}

bool Task::startApp() {
    String appFileName = Path::combine(_path, app, app);
    if (File::exists(appFileName)) {
        Process process;
        process.setRedirectStdout(true);
        process.setWaitingTimeout(3000);
        Process::start(appFileName, param, &process);
        Trace::info(process.stdoutStr());
        return true;
    }
    return false;
}

bool Task::appExists() const {
    if (app.isNullOrEmpty()) {
        return false;
    }
    String appFileName = Path::combine(_path, app, app);
    return File::exists(appFileName);
}

CycleTask::CycleTask(const String &path) : Task(path), _tick(0) {
}

CycleTask::~CycleTask() = default;

bool CycleTask::isTimeUp() {
    if (TickTimeout::isTimeout(_tick, interval)) {
        _tick = TickTimeout::getCurrentTickCount();
        return true;
    }
    return false;
}

DataRow CycleTask::toDataRow(const DataTable &table) const {
    return DataRow({
                           DataCell(table.columns()["name"], name),
                           DataCell(table.columns()["app"], app),
                           DataCell(table.columns()["param"], param),
                           DataCell(table.columns()["interval"], interval),
                   });
}

String CycleTask::type() const {
    return "cycle";
}

JsonNode CycleTask::toJsonNode() const {
    JsonNode node;
    node.add(JsonNode("name", name));
    node.add(JsonNode("type", type()));
    node.add(JsonNode("app", app));
    node.add(JsonNode("param", param));
    node.add(JsonNode("interval", interval));
    return node;
}

TimeTask::TimeTask(const String &path) : Task(path) {
}

TimeTask::~TimeTask() = default;

bool TimeTask::isTimeUp() {
    static uint32_t current = 0;
    if (TickTimeout::isTimeout(current, TimeSpan::fromMinutes(1))) {
        current = TickTimeout::getCurrentTickCount();

        if (String::equals(repeatType, "week", true)) {
            Vector<DayOfWeek> weeks;
            if (parseWeeks(repeatValue, weeks)) {
                DateTime now = DateTime::now();
                if (weeks.contains(now.dayOfWeek()) &&
                    time.hour() == now.hour() && time.minute() == now.minute()) {
                    return true;
                }
            }
        } else if (String::equals(repeatType, "day", true)) {
            DateTime now = DateTime::now();
            if (time.hour() == now.hour() && time.minute() == now.minute()) {
                return true;
            }
        } else if (String::equals(repeatType, "month", true)) {
            Vector<int> months;
            if (parseMonths(repeatValue, months)) {
                DateTime now = DateTime::now();
                if (months.contains(now.month()) &&
                    time.day() == now.day() && time.hour() == now.hour() && time.minute() == now.minute()) {
                    return true;
                }
            }
        } else if (String::equals(repeatType, "quarter", true)) {
            Vector<int> quarters;
            if (parseQuarters(repeatValue, quarters)) {
                DateTime now = DateTime::now();
                if (quarters.contains(now.month() / 4 + 1) &&
                    time.day() == now.day() && time.hour() == now.hour() && time.minute() == now.minute()) {
                    return true;
                }
            }
        }
    }
    return false;
}

DataRow TimeTask::toDataRow(const DataTable &table) const {
    return DataRow({
                           DataCell(table.columns()["name"], name),
                           DataCell(table.columns()["app"], app),
                           DataCell(table.columns()["param"], param),
                           DataCell(table.columns()["time"], time),
                           DataCell(table.columns()["repeatType"], repeatType),
                           DataCell(table.columns()["repeatValue"], repeatValue),
                   });
}

String TimeTask::type() const {
    return "time";
}

JsonNode TimeTask::toJsonNode() const {
    JsonNode node;
    node.add(JsonNode("name", name));
    node.add(JsonNode("type", type()));
    node.add(JsonNode("app", app));
    node.add(JsonNode("param", param));
    node.add(JsonNode("time", time));
    node.add(JsonNode("repeatType", repeatType));
    node.add(JsonNode("repeatValue", repeatValue));
    return node;
}

StringArray TimeTask::allRepeatTypes() {
    return {"week", "day", "month", "quarter"};
}

bool TimeTask::parseWeeks(const String &value, Vector<DayOfWeek> &weeks) {
    if (value.find('-') > 0) {
        StringArray texts;
        StringArray::parse(value, texts, '-');
        if (texts.count() == 2) {
            int start, end;
            if (Int32::parse(texts[0], start) &&
                Int32::parse(texts[1], end) &&
                start >= DayOfWeek::Sunday && start <= DayOfWeek::Saturday &&
                end >= DayOfWeek::Sunday && end <= DayOfWeek::Saturday &&
                end >= start) {
                for (int week = start; week <= end; ++week) {
                    weeks.add((DayOfWeek) week);
                }
                return true;
            }
        }
    } else {
        StringArray texts;
        StringArray::parse(value, texts, ',');
        for (size_t i = 0; i < texts.count(); ++i) {
            int week;
            if (Int32::parse(texts[i], week) &&
                week >= DayOfWeek::Sunday && week <= DayOfWeek::Saturday) {
                weeks.add((DayOfWeek) week);
            }
        }
        return weeks.count() > 0;
    }
    return false;
}

bool TimeTask::parseMonths(const String &value, Vector<int> &months) {
    if (value.find('-') > 0) {
        StringArray texts;
        StringArray::parse(value, texts, '-');
        if (texts.count() == 2) {
            int start, end;
            if (Int32::parse(texts[0], start) &&
                Int32::parse(texts[1], end) &&
                start >= 1 && start <= 12 && end >= 1 && end <= 12 &&
                end >= start) {
                for (int month = start; month <= end; ++month) {
                    months.add(month);
                }
                return true;
            }
        }
    } else {
        StringArray texts;
        StringArray::parse(value, texts, ',');
        for (size_t i = 0; i < texts.count(); ++i) {
            int month;
            if (Int32::parse(texts[i], month) &&
                month >= 1 && month <= 12) {
                months.add(month);
            }
        }
        return months.count() > 0;
    }
    return false;
}

bool TimeTask::parseQuarters(const String &value, Vector<int> &quarters) {
    if (value.find('-') > 0) {
        StringArray texts;
        StringArray::parse(value, texts, '-');
        if (texts.count() == 2) {
            int start, end;
            if (Int32::parse(texts[0], start) &&
                Int32::parse(texts[1], end) &&
                start >= 1 && start <= 4 && end >= 1 && end <= 4 &&
                end >= start) {
                for (int month = start; month <= end; ++month) {
                    quarters.add(month);
                }
                return true;
            }
        }
    } else {
        StringArray texts;
        StringArray::parse(value, texts, ',');
        for (size_t i = 0; i < texts.count(); ++i) {
            int month;
            if (Int32::parse(texts[i], month) &&
                month >= 1 && month <= 4) {
                quarters.add(month);
            }
        }
        return quarters.count() > 0;
    }
    return false;
}

ITaskProvider::ITaskProvider(const DataSource *ds) : _dataSource(ds) {
}

DataSource::DataSource(const String &name) : name(name), _provider(nullptr) {
}

DataSource::~DataSource() {
    if (_provider != nullptr) {
        delete _provider;
        _provider = nullptr;
    }
}

ITaskProvider *DataSource::provider() const {
    return _provider;
}

DbSource::DbSource(const String &name) : DataSource(name) {
}

void DbSource::createProvider() {
    _provider = new TaskDbProvider(this);
}

D5kSource::D5kSource(const String &name) : DataSource(name), appNo(0) {
}

void D5kSource::createProvider() {
    _provider = new TaskD5kProvider(this);
}

bool D5kSource::parseTableStyle(const Style &style, int &tableNo, int &contextNo) {
    if (style.isEmpty()) {
        return false;
    }

    String tableNoStr = style["tableNo"];
    if (!Int32::parse(tableNoStr, tableNo)) {
        return false;
    }
    String contextNoStr = style["contextNo"];
    if (!Int32::parse(contextNoStr, contextNo)) {
        return false;
    }
    return true;
}

bool D5kSource::parseColumnStyle(const Style &style, String &type, String &sql) {
    if (style.isEmpty()) {
        return false;
    }

    type = style["type"];
    if (type.isNullOrEmpty()) {
        return false;
    }
    sql = style["sql"];
    if (sql.isNullOrEmpty()) {
        return false;
    }
    return true;
}
