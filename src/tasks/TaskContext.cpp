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
#include "diag/Process.h"
#include "thread/TickTimeout.h"
#include "system/Application.h"
#include "TaskContext.h"
#include "TaskDbProvider.h"
#include "TaskD5kProvider.h"
#include "TaskDbService.h"

using namespace Diag;

TaskAction::TaskAction() = default;

TaskAction::TaskAction(const String &name) : name(name) {
}

TaskAction::TaskAction(const TaskAction &other) {
    TaskAction::evaluates(other);
}

bool TaskAction::equals(const TaskAction &other) const {
    return this->name == other.name && this->style == other.style;
}

void TaskAction::evaluates(const TaskAction &other) {
    this->name = other.name;
    this->style = other.style;
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
           this->registerStr == other.registerStr &&
           this->style == other.style;
}

void Table::evaluates(const Table &other) {
    this->name = other.name;
    this->columns = other.columns;
    this->registerStr = other.registerStr;
    this->style = other.style;
}

const Variable Variable::Empty;

Variable::Variable() = default;

Variable::Variable(const String &name) : name(name) {
}

Variable::Variable(const Variable &other) {
    Variable::evaluates(other);
}

bool Variable::equals(const Variable &other) const {
    return this->name == other.name && this->value == other.value;
}

void Variable::evaluates(const Variable &other) {
    this->name = other.name;
    this->value = other.value;
}

bool Variable::isEmpty() const {
    return name.isNullOrEmpty();
}

bool Variable::updateValues() {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto ps = factory->getService<ITaskProviders>();
    assert(ps);

    ITaskProvider *provider = ps->getProvider(value["datasource"]);
    if (provider != nullptr) {
        return provider->getValue(value, _values);
    } else {
        String str = value.toString();
        double minValue, maxValue, step;
        if (parseDoubleStyle(str, minValue, maxValue, step)) {
            for (double v = minValue; v < maxValue; v += step) {
                _values.add(Variant(v));
            }
            return true;
        }

        StringArray texts;
        StringArray::parse(str, texts, ';');
        if (texts.count() > 0) {
            auto values = new Values();
            for (size_t j = 0; j < texts.count(); j++) {
                _values.add(Variant(texts[j].trim(' ')));
            }
            return true;
        }

        _values.add(Variant(value.toString()));
    }
    return false;
}

void Variable::clearValues() {
    _values.clear();
}

const Variable::Values &Variable::values() const {
    return _values;
}

bool Variable::parseDoubleStyle(const String &style, double &minValue, double &maxValue, double &step) {
    if (style.isNullOrEmpty()) {
        return false;
    }

    bool result = true;
    Style s;
    if (Style::parse(style, s)) {
        String rangeStr = String::trim(s["range"], '\'', '"', ' ');
        StringArray ranges;
        StringArray::parse(rangeStr, ranges, '-');
        if (ranges.count() == 2) {
            if (!Double::parse(ranges[0], minValue)) {
                result = false;
            }
            if (!Double::parse(ranges[1], maxValue)) {
                result = false;
            }
        } else {
            result = false;
        }

        String stepStr = String::trim(s["step"], '\'', '"', ' ');
        if (!Double::parse(stepStr, step)) {
            result = false;
        }
    } else {
        result = false;
    }
    return result;
}

bool InnerExecution::execute() {
    updateVars();

    clearVars();

    return false;
}

void InnerExecution::updateVars() {
    for (size_t i = 0; i < vars.count(); ++i) {
        Variable &var = vars[i];
        var.updateValues();
    }
}

void InnerExecution::clearVars() {
    for (size_t i = 0; i < vars.count(); ++i) {
        Variable &var = vars[i];
        var.clearValues();
    }
}

AppExecution::AppExecution(const String &path, const String &app, const String &param) :
        _path(path), _app(app), _param(param) {
}

bool AppExecution::execute() {
    String appFileName = Path::combine(_path, _app, _app);
    if (File::exists(appFileName)) {
        Process process;
        process.setRedirectStdout(true);
        process.setWaitingTimeout(3000);
        bool result = Process::start(appFileName, _param, &process);
        Trace::info(process.stdoutStr());
        return result;
    }
    return false;
}

SqlExecution::SqlExecution(const String &text, bool sql) {
    if (sql) {
        _sql = text;
    } else {
        _fileName = text;
    }
}

bool SqlExecution::execute() {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto ss = factory->getService<TaskDbService>();
    assert(ss);

    if (!_sql.isNullOrEmpty()) {
        return ss->executeSql(_sql);
    } else {
        String fileName;
        if (!Path::isPathRooted(_fileName)) {
            const String appPath = Path::getAppPath();
            fileName = Path::combine(appPath, _fileName);
            if (!File::exists(fileName)) {
                Application *app = Application::instance();
                assert(app);
                fileName = Path::combine(app->rootPath(), _fileName);
            }
        } else {
            fileName = _fileName;
        }
        if (File::exists(fileName)) {
            String sql = File::readAllText(fileName);
            return !sql.isNullOrEmpty() && ss->executeSql(sql);
        }
        return false;
    }
}

PythonExecution::PythonExecution(const String &text, bool script) {
    if (script) {
        _script = text;
    } else {
        _fileName = text;
    }
}

bool PythonExecution::execute() {
    String fileName;
    if (!_script.isNullOrEmpty()) {
        fileName = Path::getTempFileName("tserver");
        FileStream fs(fileName, FileMode::FileCreate, FileAccess::FileWrite);
        fs.writeText(_script);
        fs.flush();
    } else {
        if (!Path::isPathRooted(_fileName)) {
            const String appPath = Path::getAppPath();
            fileName = Path::combine(appPath, _fileName);
            if (!File::exists(fileName)) {
                Application *app = Application::instance();
                assert(app);
                fileName = Path::combine(app->rootPath(), _fileName);
            }
        } else {
            fileName = _fileName;
        }
    }
    if (File::exists(fileName)) {
        Process process;
        process.setRedirectStdout(true);
        process.setWaitingTimeout(3000);
        bool result = Process::start("python", fileName, &process);
        Trace::info(process.stdoutStr());
        if (!_script.isNullOrEmpty()) {
            File::deleteFile(fileName);
        }
        return result;
    }
    return false;
}

CycleSchedule::CycleSchedule(const TimeSpan &interval) : _interval(interval), _tick(0) {
}

bool CycleSchedule::isTimeUp() {
    if (TickTimeout::isTimeout(_tick, _interval)) {
        _tick = TickTimeout::getCurrentTickCount();
        return true;
    }
    return false;
}

TimingSchedule::TimingSchedule(const DateTime &time, const String &_repeatType, const String &_repeatValue) :
        _time(time), _repeatType(_repeatType), _repeatValue(_repeatValue), _tickSeconds(0), _tickMinutes(0) {
}

bool TimingSchedule::isTimeUp() {
    {
        if (TickTimeout::isTimeout(_tickSeconds, TimeSpan::fromMilliseconds(999))) {
            _tickSeconds = TickTimeout::getCurrentTickCount();

            if (String::equals(_repeatType, "minute", true)) {
                DateTime now = DateTime::now();
                if (_time.second() == now.second()) {
                    return true;
                }
            } else if (String::equals(_repeatType, "hour", true)) {
                DateTime now = DateTime::now();
                if (_time.minute() == now.minute() && _time.second() == now.second()) {
                    return true;
                }
            }
        }
    }

    {
        if (TickTimeout::isTimeout(_tickMinutes, TimeSpan::fromMinutes(1))) {
            _tickMinutes = TickTimeout::getCurrentTickCount();

            if (String::equals(_repeatType, "day", true)) {
                DateTime now = DateTime::now();
                if (_time.hour() == now.hour() && _time.minute() == now.minute()) {
                    return true;
                }
            } else if (String::equals(_repeatType, "week", true)) {
                Vector<DayOfWeek> weeks;
                if (parseWeeks(_repeatValue, weeks)) {
                    DateTime now = DateTime::now();
                    if (weeks.contains(now.dayOfWeek()) &&
                        _time.hour() == now.hour() && _time.minute() == now.minute()) {
                        return true;
                    }
                }
            } else if (String::equals(_repeatType, "month", true)) {
                Vector<int> months;
                if (parseMonths(_repeatValue, months)) {
                    DateTime now = DateTime::now();
                    if (months.contains(now.month()) &&
                        _time.day() == now.day() && _time.hour() == now.hour() && _time.minute() == now.minute()) {
                        return true;
                    }
                }
            } else if (String::equals(_repeatType, "quarter", true)) {
                Vector<int> quarters;
                if (parseQuarters(_repeatValue, quarters)) {
                    DateTime now = DateTime::now();
                    if (quarters.contains(now.month() / 4 + 1) &&
                        _time.day() == now.day() && _time.hour() == now.hour() && _time.minute() == now.minute()) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}


StringArray TimingSchedule::allRepeatTypes() {
    return {"minute", "hour", "day", "week", "month", "quarter", "year"};
}

bool TimingSchedule::parseWeeks(const String &value, Vector<DayOfWeek> &weeks) {
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

bool TimingSchedule::parseMonths(const String &value, Vector<int> &months) {
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

bool TimingSchedule::parseQuarters(const String &value, Vector<int> &quarters) {
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

Task::Task(const String &name, Schedule *schedule, Execution *execution) :
        _name(name), _schedule(schedule), _execution(execution) {
}

Task::~Task() {
    delete _schedule;
    delete _execution;
}

const String &Task::name() const {
    return _name;
}

bool Task::isTimeUp() {
    return _schedule != nullptr && _schedule->isTimeUp();
}

bool Task::execute() {
    return _execution != nullptr && _execution->execute();
//    if (appExists()) {
//        return startApp();
//    } else {
//        ServiceFactory *factory = ServiceFactory::instance();
//        assert(factory);
//        auto te = factory->getService<ITaskExecution>();
//        assert(te);
//        return te->execute(this);
//    }
}

DataRow Task::toDataRow(const DataTable &table) const {
    return DataRow();
}

JsonNode Task::toJsonNode() const {
    return JsonNode();
}

//
//CycleTask::CycleTask(const String &path) : Task(path), _tick(0) {
//}
//
//CycleTask::~CycleTask() = default;
//
//bool CycleTask::isTimeUp() {
//    if (TickTimeout::isTimeout(_tick, interval)) {
//        _tick = TickTimeout::getCurrentTickCount();
//        return true;
//    }
//    return false;
//}
//
//DataRow CycleTask::toDataRow(const DataTable &table) const {
//    return DataRow({
//                           DataCell(table.columns()["name"], name),
//                           DataCell(table.columns()["app"], app),
//                           DataCell(table.columns()["param"], param),
//                           DataCell(table.columns()["interval"], interval),
//                   });
//}
//
//String CycleTask::type() const {
//    return "cycle";
//}
//
//JsonNode CycleTask::toJsonNode() const {
//    JsonNode node;
//    node.add(JsonNode("name", name));
//    node.add(JsonNode("type", type()));
//    node.add(JsonNode("app", app));
//    node.add(JsonNode("param", param));
//    node.add(JsonNode("interval", interval));
//    return node;
//}
//
//TimeTask::TimeTask(const String &path) : Task(path), _tickSeconds(0), _tickMinutes(0) {
//}
//
//TimeTask::~TimeTask() = default;
//
//bool TimeTask::isTimeUp() {
//    {
//        if (TickTimeout::isTimeout(_tickSeconds, TimeSpan::fromMilliseconds(999))) {
//            _tickSeconds = TickTimeout::getCurrentTickCount();
//
//            if (String::equals(_repeatType, "minute", true)) {
//                DateTime now = DateTime::now();
//                if (_time.second() == now.second()) {
//                    return true;
//                }
//            } else if (String::equals(_repeatType, "hour", true)) {
//                DateTime now = DateTime::now();
//                if (_time.minute() == now.minute() && _time.second() == now.second()) {
//                    return true;
//                }
//            }
//        }
//    }
//
//    {
//        if (TickTimeout::isTimeout(_tickMinutes, TimeSpan::fromMinutes(1))) {
//            _tickMinutes = TickTimeout::getCurrentTickCount();
//
//            if (String::equals(_repeatType, "day", true)) {
//                DateTime now = DateTime::now();
//                if (_time.hour() == now.hour() && _time.minute() == now.minute()) {
//                    return true;
//                }
//            } else if (String::equals(_repeatType, "week", true)) {
//                Vector <DayOfWeek> weeks;
//                if (parseWeeks(_repeatValue, weeks)) {
//                    DateTime now = DateTime::now();
//                    if (weeks.contains(now.dayOfWeek()) &&
//                        _time.hour() == now.hour() && _time.minute() == now.minute()) {
//                        return true;
//                    }
//                }
//            } else if (String::equals(_repeatType, "month", true)) {
//                Vector<int> months;
//                if (parseMonths(_repeatValue, months)) {
//                    DateTime now = DateTime::now();
//                    if (months.contains(now.month()) &&
//                        _time.day() == now.day() && _time.hour() == now.hour() && _time.minute() == now.minute()) {
//                        return true;
//                    }
//                }
//            } else if (String::equals(_repeatType, "quarter", true)) {
//                Vector<int> quarters;
//                if (parseQuarters(_repeatValue, quarters)) {
//                    DateTime now = DateTime::now();
//                    if (quarters.contains(now.month() / 4 + 1) &&
//                        _time.day() == now.day() && _time.hour() == now.hour() && _time.minute() == now.minute()) {
//                        return true;
//                    }
//                }
//            }
//        }
//    }
//
//    return false;
//}
//
//DataRow TimeTask::toDataRow(const DataTable &table) const {
//    return DataRow({
//                           DataCell(table.columns()["name"], name),
//                           DataCell(table.columns()["app"], app),
//                           DataCell(table.columns()["param"], param),
//                           DataCell(table.columns()["time"], time),
//                           DataCell(table.columns()["_repeatType"], _repeatType),
//                           DataCell(table.columns()["_repeatValue"], _repeatValue),
//                   });
//}
//
//String TimeTask::type() const {
//    return "time";
//}
//
//JsonNode TimeTask::toJsonNode() const {
//    JsonNode node;
//    node.add(JsonNode("name", name));
//    node.add(JsonNode("type", type()));
//    node.add(JsonNode("app", app));
//    node.add(JsonNode("param", param));
//    node.add(JsonNode("time", time));
//    node.add(JsonNode("_repeatType", _repeatType));
//    node.add(JsonNode("_repeatValue", _repeatValue));
//    return node;
//}
//
//StringArray TimeTask::allRepeatTypes() {
//    return {"minute", "hour", "day", "week", "month", "quarter", "year"};
//}
//
//bool TimeTask::parseWeeks(const String &value, Vector <DayOfWeek> &weeks) {
//    if (value.find('-') > 0) {
//        StringArray texts;
//        StringArray::parse(value, texts, '-');
//        if (texts.count() == 2) {
//            int start, end;
//            if (Int32::parse(texts[0], start) &&
//                Int32::parse(texts[1], end) &&
//                start >= DayOfWeek::Sunday && start <= DayOfWeek::Saturday &&
//                end >= DayOfWeek::Sunday && end <= DayOfWeek::Saturday &&
//                end >= start) {
//                for (int week = start; week <= end; ++week) {
//                    weeks.add((DayOfWeek) week);
//                }
//                return true;
//            }
//        }
//    } else {
//        StringArray texts;
//        StringArray::parse(value, texts, ',');
//        for (size_t i = 0; i < texts.count(); ++i) {
//            int week;
//            if (Int32::parse(texts[i], week) &&
//                week >= DayOfWeek::Sunday && week <= DayOfWeek::Saturday) {
//                weeks.add((DayOfWeek) week);
//            }
//        }
//        return weeks.count() > 0;
//    }
//    return false;
//}
//
//bool TimeTask::parseMonths(const String &value, Vector<int> &months) {
//    if (value.find('-') > 0) {
//        StringArray texts;
//        StringArray::parse(value, texts, '-');
//        if (texts.count() == 2) {
//            int start, end;
//            if (Int32::parse(texts[0], start) &&
//                Int32::parse(texts[1], end) &&
//                start >= 1 && start <= 12 && end >= 1 && end <= 12 &&
//                end >= start) {
//                for (int month = start; month <= end; ++month) {
//                    months.add(month);
//                }
//                return true;
//            }
//        }
//    } else {
//        StringArray texts;
//        StringArray::parse(value, texts, ',');
//        for (size_t i = 0; i < texts.count(); ++i) {
//            int month;
//            if (Int32::parse(texts[i], month) &&
//                month >= 1 && month <= 12) {
//                months.add(month);
//            }
//        }
//        return months.count() > 0;
//    }
//    return false;
//}
//
//bool TimeTask::parseQuarters(const String &value, Vector<int> &quarters) {
//    if (value.find('-') > 0) {
//        StringArray texts;
//        StringArray::parse(value, texts, '-');
//        if (texts.count() == 2) {
//            int start, end;
//            if (Int32::parse(texts[0], start) &&
//                Int32::parse(texts[1], end) &&
//                start >= 1 && start <= 4 && end >= 1 && end <= 4 &&
//                end >= start) {
//                for (int month = start; month <= end; ++month) {
//                    quarters.add(month);
//                }
//                return true;
//            }
//        }
//    } else {
//        StringArray texts;
//        StringArray::parse(value, texts, ',');
//        for (size_t i = 0; i < texts.count(); ++i) {
//            int month;
//            if (Int32::parse(texts[i], month) &&
//                month >= 1 && month <= 4) {
//                quarters.add(month);
//            }
//        }
//        return quarters.count() > 0;
//    }
//    return false;
//}

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
