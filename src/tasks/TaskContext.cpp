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
#include "TaskDbService.h"

using namespace Diag;

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

JsonNode AppExecution::toJsonNode() const {
    JsonNode node;
    node.add(JsonNode("app", _app));
    node.add(JsonNode("param", _param));
    return node;
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

JsonNode SqlExecution::toJsonNode() const {
    JsonNode node;
    node.add(JsonNode("sql", _sql));
    node.add(JsonNode("fileName", _fileName));
    return node;
}

PythonExecution::PythonExecution(const String &script) : _script(script) {
}

PythonExecution::PythonExecution(const String &fileName, const String &param) : _fileName(fileName), _param(param) {
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
        String param = !_param.isNullOrEmpty() ?
                String::format("%s %s", fileName.c_str(), _param.c_str()) :
                fileName;
        bool result = Process::start("python", param, &process);
        Trace::info(process.stdoutStr());
        if (!_script.isNullOrEmpty()) {
            File::deleteFile(fileName);
        }
        return result;
    }
    return false;
}

JsonNode PythonExecution::toJsonNode() const {
    JsonNode node;
    node.add(JsonNode("script", _script));
    node.add(JsonNode("fileName", _fileName));
    return node;
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

JsonNode CycleSchedule::toJsonNode() const {
    JsonNode node;
    node.add(JsonNode("interval", _interval));
    return node;
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
            } else if (String::equals(_repeatType, "year", true)) {
                Vector<int> years;
                if (parseYears(_repeatValue, years)) {
                    DateTime now = DateTime::now();
                    if (years.contains(now.year()) &&
                        _time.month() == now.month() && _time.day() == now.day() &&
                        _time.hour() == now.hour() && _time.minute() == now.minute()) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

JsonNode TimingSchedule::toJsonNode() const {
    JsonNode node;
    node.add(JsonNode("time", _time));
    node.add(JsonNode("name", _repeatType));
    node.add(JsonNode("repeatValue", _repeatValue));
    return node;
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

bool TimingSchedule::parseYears(const String &value, Vector<int> &years) {
    if (value.find('-') > 0) {
        StringArray texts;
        StringArray::parse(value, texts, '-');
        if (texts.count() == 2) {
            int start, end;
            if (Int32::parse(texts[0], start) &&
                Int32::parse(texts[1], end) &&
                start >= 0 && start <= 9999 && end >= 0 && end <= 9999 &&
                end >= start) {
                for (int year = start; year <= end; ++year) {
                    years.add(year);
                }
                return true;
            }
        }
    } else {
        StringArray texts;
        StringArray::parse(value, texts, ',');
        for (size_t i = 0; i < texts.count(); ++i) {
            int year;
            if (Int32::parse(texts[i], year) &&
                year >= 0 && year <= 9999) {
                years.add(year);
            }
        }
        return years.count() > 0;
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
    if (_execution != nullptr) {
        bool result = _execution->execute();
        if (!result) {
            Trace::error(String::format("Can not execute a task'%s'!", _name.c_str()));
        } else {
            Trace::info(String::format("Execute a task'%s' successfully.", _name.c_str()));
        }
        return result;
    } else {
        Trace::error(String::format("Can not find execution in task'%s'!", _name.c_str()));
        return false;
    }
}

DataRow Task::toDataRow(const DataTable &table) const {
    if (_schedule != nullptr && _execution != nullptr) {
        return DataRow({
                               DataCell(table.columns()["name"], _name),
                               DataCell(table.columns()["schedule"], _schedule->toJsonNode().toString()),
                               DataCell(table.columns()["execution"], _execution->toJsonNode().toString()),
                       });
    } else {
        return DataRow({
                               DataCell(table.columns()["name"], _name),
                       });
    }
}

JsonNode Task::toJsonNode() const {
    JsonNode node;
    node.add(JsonNode("name", _name));
    if (_schedule != nullptr) {
        node.add(JsonNode("schedule", _schedule->toJsonNode()));
    }
    if (_execution != nullptr) {
        node.add(JsonNode("execution", _execution->toJsonNode()));
    }
    return node;
}
