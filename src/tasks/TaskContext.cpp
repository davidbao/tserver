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
#include "thread/Task.h"
#include "system/Application.h"
#include "crypto/Md5Provider.h"
#include "TaskContext.h"
#include "TaskDbService.h"

using namespace Diag;
using namespace Crypto;

Execution::Execution(bool sync, const TimeSpan &timeout) : _sync(sync), _timeout(timeout) {
}

bool Execution::sync() const {
    return _sync;
}

const TimeSpan &Execution::timeout() const {
    return _timeout;
}

void Execution::removeFile() {
    String fileName = currentFile();
    if (File::exists(fileName)) {
        File::deleteFile(fileName);
    }
}

bool Execution::checkFile(const String &md5) const {
    String fileName = currentFile();
    if (File::exists(fileName)) {
        String actual;
        if (Md5Provider::computeFileHash(fileName, actual)) {
            return String::equals(actual, md5, true);
        }
    }
    return false;
}

String Execution::currentPath() const {
    Application *app = Application::instance();
    assert(app);
    return app->rootPath();
}

String Execution::toString() const {
    return toJsonNode().toString();
}

AppExecution::AppExecution(bool sync, const TimeSpan &timeout,
                           const String &app, const String &param) :
        Execution(sync, timeout), _app(app), _param(param) {
}

String AppExecution::type() const {
    return "app";
}

const String &AppExecution::app() const {
    return _app;
}

const String &AppExecution::param() const {
    return _param;
}

bool AppExecution::execute() {
    String fileName = currentFile();
    if (File::exists(fileName)) {
        bool result;
        if (_sync) {
            Process process;
            process.setRedirectStdout(true);
            process.setWaitingTimeout(_timeout);
            result = Process::start(fileName, _param, &process);
            Trace::info(process.stdoutStr());
        } else {
            result = Process::start(fileName, _param);
        }
        return result;
    }
    return false;
}

JsonNode AppExecution::toJsonNode() const {
    JsonNode node;
    node.add(JsonNode("type", "app"));
    JsonNode syncNode("sync");
    syncNode.add(JsonNode("enable", _sync));
    if (_sync) {
        syncNode.add(JsonNode("timeout", _timeout));
    }
    node.add(syncNode);
    node.add(JsonNode("app", _app));
    node.add(JsonNode("param", _param));
    return node;
}

String AppExecution::currentFile() const {
    String fileName;
    if (!Path::isPathRooted(_app)) {
        fileName = Path::combine(currentPath(), _app);
    } else {
        fileName = _app;
    }
    return fileName;
}

SqlExecution::SqlExecution(bool sync, const TimeSpan &timeout, const String &text, bool sql) :
        Execution(sync, timeout) {
    if (sql) {
        _sql = text;
    } else {
        _fileName = text;
    }
}

String SqlExecution::type() const {
    return "sql";
}

const String &SqlExecution::sql() const {
    return _sql;
}

const String &SqlExecution::fileName() const {
    return _fileName;
}

bool SqlExecution::isFile() const {
    return !_fileName.isNullOrEmpty();
}

bool SqlExecution::execute() {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto ss = factory->getService<TaskDbService>();
    assert(ss);

    if (!_sql.isNullOrEmpty()) {
        if (_sync) {
            return ss->executeSql(_sql);
        } else {
            Task::run(&TaskDbService::executeSql, ss, _sql);
            return true;
        }
    } else {
        String fileName;
        if (!Path::isPathRooted(_fileName)) {
            fileName = Path::combine(currentPath(), _fileName);
        } else {
            fileName = _fileName;
        }
        if (File::exists(fileName)) {
            String sql = File::readAllText(fileName);
            if (_sync) {
                return ss->executeSql(sql);
            } else {
                Task::run(&TaskDbService::executeSql, ss, sql);
                return true;
            }
        } else {
            Trace::error(String::format("Can not find the execution sql file'%s'!", fileName.c_str()));
        }
        return false;
    }
}

JsonNode SqlExecution::toJsonNode() const {
    JsonNode node;
    node.add(JsonNode("type", "sql"));
    JsonNode syncNode("sync");
    syncNode.add(JsonNode("enable", _sync));
    if (_sync) {
        syncNode.add(JsonNode("timeout", _timeout));
    }
    node.add(syncNode);
    if (!_sql.isNullOrEmpty()) {
        node.add(JsonNode("sql", _sql));
    }
    if (!_fileName.isNullOrEmpty()) {
        node.add(JsonNode("file", _fileName));
    }
    return node;
}

String SqlExecution::currentFile() const {
    String fileName;
    if (!Path::isPathRooted(_fileName)) {
        fileName = Path::combine(currentPath(), _fileName);
    } else {
        fileName = _fileName;
    }
    return fileName;
}

PythonExecution::PythonExecution(bool sync, const TimeSpan &timeout, const String &script) :
        Execution(sync, timeout), _script(script) {
}

PythonExecution::PythonExecution(bool sync, const TimeSpan &timeout, const String &fileName, const String &param) :
        Execution(sync, timeout), _fileName(fileName), _param(param) {
}

String PythonExecution::type() const {
    return "python";
}

const String &PythonExecution::script() const {
    return _script;
}

const String &PythonExecution::fileName() const {
    return _fileName;
}

const String &PythonExecution::param() const {
    return _param;
}

bool PythonExecution::isFile() const {
    return !_fileName.isNullOrEmpty();
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
            fileName = Path::combine(currentPath(), _fileName);
        } else {
            fileName = _fileName;
        }
    }
    if (File::exists(fileName)) {
        bool result;
        String param = !_param.isNullOrEmpty() ?
                       String::format("%s %s", fileName.c_str(), _param.c_str()) :
                       fileName;
        static const char *PythonApp = "python";
        if (_sync) {
            Process process;
            process.setRedirectStdout(true);
            process.setWaitingTimeout(_timeout);
            result = Process::start(PythonApp, param, &process);
            Trace::info(process.stdoutStr());
        } else {
            result = Process::start(PythonApp, param);
        }
        if (!_script.isNullOrEmpty()) {
            File::deleteFile(fileName);
        }
        return result;
    } else {
        Trace::error(String::format("Can not find the execution python file'%s'!", fileName.c_str()));
    }
    return false;
}

JsonNode PythonExecution::toJsonNode() const {
    JsonNode node;
    node.add(JsonNode("type", "python"));
    JsonNode syncNode("sync");
    syncNode.add(JsonNode("enable", _sync));
    if (_sync) {
        syncNode.add(JsonNode("timeout", _timeout));
    }
    node.add(syncNode);
    if (!_script.isNullOrEmpty()) {
        node.add(JsonNode("script", _script));
    }
    if (!_fileName.isNullOrEmpty()) {
        node.add(JsonNode("file", _fileName));
        node.add(JsonNode("param", _param));
    }
    return node;
}

String PythonExecution::currentFile() const {
    String fileName;
    if (!Path::isPathRooted(_fileName)) {
        fileName = Path::combine(currentPath(), _fileName);
    } else {
        fileName = _fileName;
    }
    return fileName;
}

String Schedule::toString() const {
    return toJsonNode().toString();
}

CycleSchedule::CycleSchedule(const TimeSpan &interval) : _interval(interval), _tick(0) {
}

String CycleSchedule::type() const {
    return "cycle";
}

const TimeSpan &CycleSchedule::interval() const {
    return _interval;
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
    node.add(JsonNode("type", "cycle"));
    node.add(JsonNode("interval", _interval));
    return node;
}

const TimeSpan TimingSchedule::MinDeadZone = TimeSpan::fromMinutes(5);
const TimeSpan TimingSchedule::SecDeadZone = TimeSpan::fromSeconds(5);

TimingSchedule::TimingSchedule(const DateTime &time, const String &repeatType, const String &repeatValue) :
        _time(time), _repeatType(repeatType), _repeatValue(repeatValue),
        _tickSeconds(0), _tickMinutes(0), _lastTimeUpTick(0) {
}

String TimingSchedule::type() const {
    return "timing";
}

const DateTime &TimingSchedule::time() const {
    return _time;
}

const String &TimingSchedule::repeatType() const {
    return _repeatType;
}

const String &TimingSchedule::repeatValue() const {
    return _repeatValue;
}

bool TimingSchedule::isTimeUp() {
    {
        if (TickTimeout::isTimeout(_tickSeconds, TimeSpan::fromMilliseconds(501))) {
            _tickSeconds = TickTimeout::getCurrentTickCount();

            if (String::equals(_repeatType, "minute", true)) {
                return isTimeUp(Types::S);
            } else if (String::equals(_repeatType, "hour", true)) {
                return isTimeUp(Types::MS);
            }
        }
    }

    {
        if (TickTimeout::isTimeout(_tickMinutes, TimeSpan::fromSeconds(31))) {
            _tickMinutes = TickTimeout::getCurrentTickCount();

            if (String::equals(_repeatType, "day", true)) {
                return isTimeUp(Types::HM);
            } else if (String::equals(_repeatType, "week", true)) {
                Vector<DayOfWeek> weeks;
                if (parseWeeks(_repeatValue, weeks)) {
                    DateTime now = DateTime::now();
                    if (weeks.contains(now.dayOfWeek())) {
                        return isTimeUp(Types::HM);
                    }
                }
            } else if (String::equals(_repeatType, "month", true)) {
                Vector<int> months;
                if (parseMonths(_repeatValue, months)) {
                    DateTime now = DateTime::now();
                    if (months.contains(now.month())) {
                        return isTimeUp(Types::DHM);
                    }
                }
            } else if (String::equals(_repeatType, "quarter", true)) {
                Vector<int> quarters;
                if (parseQuarters(_repeatValue, quarters)) {
                    DateTime now = DateTime::now();
                    if (quarters.contains(now.month() / 4 + 1)) {
                        return isTimeUp(Types::DHM);
                    }
                }
            } else if (String::equals(_repeatType, "year", true)) {
                Vector<int> years;
                if (parseYears(_repeatValue, years)) {
                    DateTime now = DateTime::now();
                    if (years.contains(now.year())) {
                        return isTimeUp(Types::MDHM);
                    }
                }
            }
        }
    }
    return false;
}

bool TimingSchedule::isTimeUp(Types type) {
    DateTime now = DateTime::now();
    if (type == Types::S) {
        DateTime time = DateTime(now.year(), now.month(), now.day(),
                                 now.hour(), now.minute(), _time.second(), 0, now.kind());
        return isTimeUp(now, time, SecDeadZone);
    } else if (type == Types::MS) {
        DateTime time = DateTime(now.year(), now.month(), now.day(),
                                 now.hour(), _time.minute(), _time.second(), 0, now.kind());
        return isTimeUp(now, time, MinDeadZone);
    } else if (type == Types::HM) {
        DateTime time = DateTime(now.year(), now.month(), now.day(),
                                 _time.hour(), _time.minute(), 0, 0, now.kind());
        return isTimeUp(now, time, MinDeadZone);
    } else if (type == Types::DHM) {
        DateTime time = DateTime(now.year(), now.month(), _time.day(),
                                 _time.hour(), _time.minute(), 0, 0, now.kind());
        return isTimeUp(now, time, MinDeadZone);
    } else if (type == Types::MDHM) {
        DateTime time = DateTime(now.year(), _time.month(), _time.day(),
                                 _time.hour(), _time.minute(), 0, 0, now.kind());
        return isTimeUp(now, time, MinDeadZone);
    }
    return false;
}

bool TimingSchedule::isTimeUp(const DateTime &now, const DateTime &time, const TimeSpan &deadZone) {
    TimeSpan diff = now - time;
//    Debug::writeFormatLine("now: %s, time: %s, diff: %s",
//                           now.toString().c_str(),
//                           time.toString().c_str(),
//                           diff.toString().c_str());
    if (diff >= TimeSpan::Zero && diff <= deadZone &&
        TickTimeout::isTimeout(_lastTimeUpTick, deadZone)) {
        _lastTimeUpTick = TickTimeout::getCurrentTickCount();
        return true;
    }
    return false;
}

JsonNode TimingSchedule::toJsonNode() const {
    JsonNode node;
    node.add(JsonNode("type", "timing"));
    node.add(JsonNode("time", toTimeStr()));
    JsonNode repeat("repeat");
    repeat.add(JsonNode("type", _repeatType));
    repeat.add(JsonNode("value", _repeatValue));
    node.add(repeat);
    return node;
}

String TimingSchedule::toTimeStr() const {
    if (String::equals(_repeatType, "minute", true)) {
        return _time.toString("HH:mm:ss");
    } else if (String::equals(_repeatType, "hour", true)) {
        return _time.toString("HH:mm:ss");
    } else if (String::equals(_repeatType, "day", true)) {
        return _time.toString("HH:mm:ss");
    } else if (String::equals(_repeatType, "week", true)) {
        return _time.toString("HH:mm:ss");
    } else if (String::equals(_repeatType, "month", true)) {
        return _time.toString("HH:mm:ss");
    } else if (String::equals(_repeatType, "quarter", true)) {
        return _time.toString("dd.HH:mm:ss");
    } else if (String::equals(_repeatType, "year", true)) {
        return _time.toString("yyy-MM-dd HH:mm:ss");
    } else {
        return _time.toString();
    }
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
                start >= 1 && start <= 9999 && end >= 1 && end <= 9999 &&
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
                year >= 1 && year <= 9999) {
                years.add(year);
            }
        }
        return years.count() > 0;
    }
    return false;
}

Crontab::Crontab(const String &name, Schedule *schedule, Execution *execution) :
        _name(name), _schedule(schedule), _execution(execution) {
}

Crontab::~Crontab() {
    delete _schedule;
    delete _execution;
}

const String &Crontab::name() const {
    return _name;
}

const Schedule *Crontab::schedule() const {
    return _schedule;
}

const Execution *Crontab::execution() const {
    return _execution;
}

bool Crontab::isTimeUp() {
    return isValid() && _schedule->isTimeUp();
}

bool Crontab::execute() {
    if (_execution != nullptr) {
        Trace::verb(String::format("Start to execute a task'%s'.", _name.c_str()));
        bool result = _execution->execute();
        if (!result) {
            Trace::error(String::format("Can not execute a task'%s'!", _name.c_str()));
        } else {
            Trace::verb(String::format("Execute a task'%s' successfully.", _name.c_str()));
        }
        return result;
    } else {
        Trace::error(String::format("Can not find execution in task'%s'!", _name.c_str()));
        return false;
    }
}

DataRow Crontab::toDataRow(const DataTable &table) const {
    if (isValid()) {
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

JsonNode Crontab::toJsonNode() const {
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

void Crontab::removeFile() {
    if (_execution != nullptr) {
        _execution->removeFile();
    }
}

bool Crontab::isValid() const {
    return _schedule != nullptr && _execution != nullptr;
}

bool Crontab::parseSchedule(const String &str, Schedule *&schedule) {
    JsonNode scheduleNode;
    if (JsonNode::parse(str, scheduleNode)) {
        String type = scheduleNode.getAttribute("type");
        if (type == "cycle") {
            TimeSpan interval;
            scheduleNode.getAttribute("interval", interval);
            if (interval != TimeSpan::Zero) {
                schedule = new CycleSchedule(interval);
                return true;
            } else {
                // Cycle interval is invalid.
            }
        } else if (type == "timing") {
            DateTime time;
            scheduleNode.getAttribute("time", time);
            String repeatType, repeatValue;
            JsonNode repeatNode = scheduleNode["repeat"];
            repeatNode.getAttribute("type", repeatType);
            repeatNode.getAttribute("value", repeatValue);
            if (!TimingSchedule::allRepeatTypes().contains(repeatType, true)) {
                // Repeat type is invalid.
            }
            schedule = new TimingSchedule(time, repeatType, repeatValue);
            return true;
        } else {
            // Can not find schedule type.
        }
    }
    return false;
}

bool Crontab::parseExecution(const String &str, Execution *&execution) {
    JsonNode executionNode;
    if (JsonNode::parse(str, executionNode)) {
        String type = executionNode.getAttribute("type");
        JsonNode syncNode = executionNode["sync"];
        bool sync = true;
        syncNode.getAttribute("enable", sync);
        TimeSpan timeout = TimeSpan::fromSeconds(10);
        syncNode.getAttribute("timeout", timeout);
        if (type == "app") {
            String app, param;
            executionNode.getAttribute("app", app);
            executionNode.getAttribute("param", param);
            execution = new AppExecution(sync, timeout, app, param);
            return true;
        } else if (type == "sql") {
            String sql, fileName;
            executionNode.getAttribute("sql", sql);
            executionNode.getAttribute("file", fileName);
            execution = !sql.isNullOrEmpty() ?
                        new SqlExecution(sync, timeout, sql, true) :
                        new SqlExecution(sync, timeout, fileName, false);
            return true;
        } else if (type == "python") {
            String script, fileName, param;
            executionNode.getAttribute("script", script);
            executionNode.getAttribute("file", fileName);
            executionNode.getAttribute("param", param);
            execution = !script.isNullOrEmpty() ?
                        new PythonExecution(sync, timeout, script) :
                        new PythonExecution(sync, timeout, fileName, param);
            return true;
        } else {
            // Can not find execution type.
        }
    }
    return false;
}
