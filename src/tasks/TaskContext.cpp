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

Execution::Execution() : _sync(true), _timeout(TimeSpan::Zero) {
}

Execution::Execution(bool sync, const TimeSpan &timeout) : _sync(sync), _timeout(timeout) {
}

bool Execution::equals(const Execution &other) const {
    return _sync == other._sync && _timeout == other._timeout;
}

void Execution::evaluates(const Execution &other) {
    _sync = other._sync;
    _timeout = other._timeout;
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
        Md5Provider md5p;
        if (md5p.computeFileHash(fileName, actual)) {
            return String::equals(actual, md5, true);
        }
    }
    return false;
}

String Execution::currentPath() {
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

bool AppExecution::equals(const Execution &other) const {
    if (!Execution::equals(other)) {
        return false;
    }
    auto p = dynamic_cast<const AppExecution *>(&other);
    assert(p);
    return _app == p->_app && _param == p->_param;
}

void AppExecution::evaluates(const Execution &other) {
    Execution::evaluates(other);
    auto p = dynamic_cast<const AppExecution *>(&other);
    assert(p);
    _app = p->_app;
    _param = p->_param;
}

Execution *AppExecution::clone() const {
    auto p = new AppExecution();
    p->evaluates(*this);
    return p;
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

bool SqlExecution::equals(const Execution &other) const {
    if (!Execution::equals(other)) {
        return false;
    }
    auto p = dynamic_cast<const SqlExecution *>(&other);
    assert(p);
    return _sql == p->_sql && _fileName == p->_fileName;
}

void SqlExecution::evaluates(const Execution &other) {
    Execution::evaluates(other);
    auto p = dynamic_cast<const SqlExecution *>(&other);
    assert(p);
    _sql = p->_sql;
    _fileName = p->_fileName;
}

Execution *SqlExecution::clone() const {
    auto p = new SqlExecution();
    p->evaluates(*this);
    return p;
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

bool PythonExecution::equals(const Execution &other) const {
    if (!Execution::equals(other)) {
        return false;
    }
    auto p = dynamic_cast<const PythonExecution *>(&other);
    assert(p);
    return _script == p->_script && _fileName == p->_fileName && _param == p->_param;
}

void PythonExecution::evaluates(const Execution &other) {
    Execution::evaluates(other);
    auto p = dynamic_cast<const PythonExecution *>(&other);
    assert(p);
    _script = p->_script;
    _fileName = p->_fileName;
    _param = p->_param;
}

Execution *PythonExecution::clone() const {
    auto p = new PythonExecution();
    p->evaluates(*this);
    return p;
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

CycleSchedule::CycleSchedule(const TimeSpan &interval) : _interval(interval) {
}

bool CycleSchedule::equals(const Schedule &other) const {
    auto p = dynamic_cast<const CycleSchedule *>(&other);
    assert(p);
    return _interval == p->_interval;
}

void CycleSchedule::evaluates(const Schedule &other) {
    auto p = dynamic_cast<const CycleSchedule *>(&other);
    assert(p);
    _interval = p->_interval;
}

Schedule *CycleSchedule::clone() const {
    auto p = new CycleSchedule();
    p->evaluates(*this);
    return p;
}

String CycleSchedule::type() const {
    return "cycle";
}

const TimeSpan &CycleSchedule::interval() const {
    return _interval;
}

bool CycleSchedule::isTimeUp(const String &taskName) {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto tc = factory->getService<ITaskCache>();
    if (tc == nullptr) {
        return false;
    }
    const String key = String::format("%s.tick", taskName.c_str());
    String tickStr = tc->getValue(key);
    uint32_t tick = 0;
    UInt32::parse(tickStr, tick);

    if (TickTimeout::isTimeout(tick, _interval)) {
        tc->setValue(key, UInt32(TickTimeout::getCurrentTickCount()).toString());
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
        _time(time), _repeatType(repeatType), _repeatValue(repeatValue) {
}

bool TimingSchedule::equals(const Schedule &other) const {
    auto p = dynamic_cast<const TimingSchedule *>(&other);
    assert(p);
    return _time == p->_time && _repeatType == p->_repeatType && _repeatValue == p->_repeatValue;
}

void TimingSchedule::evaluates(const Schedule &other) {
    auto p = dynamic_cast<const TimingSchedule *>(&other);
    assert(p);
    _time = p->_time;
    _repeatType = p->_repeatType;
    _repeatValue = p->_repeatValue;
}

Schedule *TimingSchedule::clone() const {
    auto p = new TimingSchedule();
    p->evaluates(*this);
    return p;
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

bool TimingSchedule::isTimeUp(const String &taskName) {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto tc = factory->getService<ITaskCache>();
    if (tc == nullptr) {
        return false;
    }

    {
        const String key = String::format("%s.tickSeconds", taskName.c_str());
        String tickStr = tc->getValue(key);
        uint32_t tick = 0;
        UInt32::parse(tickStr, tick);
        if (TickTimeout::isTimeout(tick, TimeSpan::fromMilliseconds(501))) {
            tc->setValue(key, UInt32(TickTimeout::getCurrentTickCount()).toString());

            if (String::equals(_repeatType, "minute", true)) {
                return isTimeUp(taskName, Types::S);
            } else if (String::equals(_repeatType, "hour", true)) {
                return isTimeUp(taskName, Types::MS);
            }
        }
    }

    {
        const String key = String::format("%s.tickMinutes", taskName.c_str());
        String tickStr = tc->getValue(key);
        uint32_t tick = 0;
        UInt32::parse(tickStr, tick);
        if (TickTimeout::isTimeout(tick, TimeSpan::fromSeconds(31))) {
            tc->setValue(key, UInt32(TickTimeout::getCurrentTickCount()).toString());

            if (String::equals(_repeatType, "day", true)) {
                return isTimeUp(taskName, Types::HM);
            } else if (String::equals(_repeatType, "week", true)) {
                Vector<DayOfWeek> weeks;
                if (parseWeeks(_repeatValue, weeks)) {
                    DateTime now = DateTime::now();
                    if (weeks.contains(now.dayOfWeek())) {
                        return isTimeUp(taskName, Types::HM);
                    }
                }
            } else if (String::equals(_repeatType, "month", true)) {
                Vector<int> months;
                if (parseMonths(_repeatValue, months)) {
                    DateTime now = DateTime::now();
                    if (months.contains(now.month())) {
                        return isTimeUp(taskName, Types::DHM);
                    }
                }
            } else if (String::equals(_repeatType, "quarter", true)) {
                Vector<int> quarters;
                if (parseQuarters(_repeatValue, quarters)) {
                    DateTime now = DateTime::now();
                    if (quarters.contains(now.month() / 4 + 1)) {
                        return isTimeUp(taskName, Types::DHM);
                    }
                }
            } else if (String::equals(_repeatType, "year", true)) {
                Vector<int> years;
                if (parseYears(_repeatValue, years)) {
                    DateTime now = DateTime::now();
                    if (years.contains(now.year())) {
                        return isTimeUp(taskName, Types::MDHM);
                    }
                }
            }
        }
    }
    return false;
}

bool TimingSchedule::isTimeUp(const String &taskName, Types type) {
    DateTime now = DateTime::now();
    if (type == Types::S) {
        DateTime time = DateTime(now.year(), now.month(), now.day(),
                                 now.hour(), now.minute(), _time.second(), 0, now.kind());
        return isTimeUp(taskName, now, time, SecDeadZone);
    } else if (type == Types::MS) {
        DateTime time = DateTime(now.year(), now.month(), now.day(),
                                 now.hour(), _time.minute(), _time.second(), 0, now.kind());
        return isTimeUp(taskName, now, time, MinDeadZone);
    } else if (type == Types::HM) {
        DateTime time = DateTime(now.year(), now.month(), now.day(),
                                 _time.hour(), _time.minute(), 0, 0, now.kind());
        return isTimeUp(taskName, now, time, MinDeadZone);
    } else if (type == Types::DHM) {
        DateTime time = DateTime(now.year(), now.month(), _time.day(),
                                 _time.hour(), _time.minute(), 0, 0, now.kind());
        return isTimeUp(taskName, now, time, MinDeadZone);
    } else if (type == Types::MDHM) {
        DateTime time = DateTime(now.year(), _time.month(), _time.day(),
                                 _time.hour(), _time.minute(), 0, 0, now.kind());
        return isTimeUp(taskName, now, time, MinDeadZone);
    }
    return false;
}

bool TimingSchedule::isTimeUp(const String &taskName,
                              const DateTime &now, const DateTime &time, const TimeSpan &deadZone) {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto tc = factory->getService<ITaskCache>();
    if (tc == nullptr) {
        return false;
    }

    TimeSpan diff = now - time;
//    Debug::writeFormatLine("now: %s, time: %s, diff: %s",
//                           now.toString().c_str(),
//                           time.toString().c_str(),
//                           diff.toString().c_str());
    const String key = String::format("%s.lastTimeUpTick", taskName.c_str());
    String tickStr = tc->getValue(key);
    uint32_t tick = 0;
    UInt32::parse(tickStr, tick);
    if (diff >= TimeSpan::Zero && diff <= deadZone &&
        TickTimeout::isTimeout(tick, deadZone)) {
        tc->setValue(key, UInt32(TickTimeout::getCurrentTickCount()).toString());
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

Crontab::Crontab() : name(String::Empty), enable(true), _schedule(nullptr), _execution(nullptr) {
}

Crontab::Crontab(const String &name, Schedule *schedule, Execution *execution) :
        name(name), enable(true), _schedule(schedule), _execution(execution) {
}

Crontab::Crontab(const Crontab &other) : enable(true), _schedule(nullptr), _execution(nullptr) {
    Crontab::evaluates(other);
}

Crontab::~Crontab() {
    delete _schedule;
    delete _execution;
}

bool Crontab::equals(const Crontab &other) const {
    if (!(name == other.name && enable == other.enable)) {
        return false;
    }

    if (_schedule != nullptr && other._schedule != nullptr) {
        if (!_schedule->equals(*other._schedule)) {
            return false;
        }
    } else if (_schedule != nullptr && other._schedule == nullptr) {
        return false;
    } else if (_schedule == nullptr && other._schedule != nullptr) {
        return false;
    }

    if (_execution != nullptr && other._execution != nullptr) {
        if (!_execution->equals(*other._execution)) {
            return false;
        }
    } else if (_execution != nullptr && other._execution == nullptr) {
        return false;
    } else if (_execution == nullptr && other._execution != nullptr) {
        return false;
    }
    return true;
}

void Crontab::evaluates(const Crontab &other) {
    name = other.name;
    enable = other.enable;
    delete _schedule;
    _schedule = nullptr;
    if (other._schedule != nullptr) {
        _schedule = other._schedule->clone();
    }
    delete _execution;
    _execution = nullptr;
    if (other._execution != nullptr) {
        _execution = other._execution->clone();
    }
}

Crontab &Crontab::operator=(const Crontab &other) {
    if (this != &other) {
        Crontab::evaluates(other);
    }
    return *this;
}

const Schedule *Crontab::schedule() const {
    return _schedule;
}

const Execution *Crontab::execution() const {
    return _execution;
}

bool Crontab::isTimeUp() {
    return isValid() && _schedule->isTimeUp(name);
}

bool Crontab::execute() {
    if (_execution != nullptr) {
        Trace::verb(String::format("Start to execute a task'%s'.", name.c_str()));
        bool result = _execution->execute();
        if (!result) {
            Trace::error(String::format("Can not execute a task'%s'!", name.c_str()));
        } else {
            Trace::verb(String::format("Execute a task'%s' successfully.", name.c_str()));
        }
        return result;
    } else {
        Trace::error(String::format("Can not find execution in task'%s'!", name.c_str()));
        return false;
    }
}

DataRow Crontab::toDataRow(const DataTable &table) const {
    if (isValid()) {
        return DataRow({
                               DataCell(table.columns()["name"], name),
                               DataCell(table.columns()["schedule"], _schedule->toJsonNode().toString()),
                               DataCell(table.columns()["execution"], _execution->toJsonNode().toString()),
                       });
    } else {
        return DataRow({
                               DataCell(table.columns()["name"], name),
                       });
    }
}

JsonNode Crontab::toJsonNode() const {
    JsonNode node;
    node.add(JsonNode("name", name));
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

bool Crontab::findName(const String &n) const {
    return n.isNullOrEmpty() || this->name.find(n) >= 0;
}

void Crontab::updateYmlProperties(YmlNode::Properties &properties, int pos) const {
    properties.add(String::format(TasksPrefix "name", pos), name);
    {
        String type = _schedule->type();
        properties.add(String::format(SchedulePrefix "type", pos), type);
        if (type == "cycle") {
            auto cycle = dynamic_cast<const CycleSchedule *>(_schedule);
            assert(cycle);
            properties.add(String::format(SchedulePrefix "interval", pos), cycle->interval());
        } else if (type == "timing") {
            auto timing = dynamic_cast<const TimingSchedule *>(_schedule);
            assert(timing);
            properties.add(String::format(SchedulePrefix "time", pos), timing->toTimeStr());
            properties.add(String::format(SchedulePrefix "repeat.type", pos), timing->repeatType());
            properties.add(String::format(SchedulePrefix "repeat.value", pos), timing->repeatValue());
        }
    }
    {
        String type = _execution->type();
        properties.add(String::format(ExecutionPrefix "type", pos), type);
        properties.add(String::format(ExecutionPrefix "sync.enable", pos), _execution->sync());
        properties.add(String::format(ExecutionPrefix "sync.timeout", pos), _execution->timeout());
        if (type == "app") {
            auto app = dynamic_cast<const AppExecution *>(_execution);
            assert(app);
            properties.add(String::format(ExecutionPrefix "app", pos), app->app());
            properties.add(String::format(ExecutionPrefix "param", pos), app->param());
        } else if (type == "sql") {
            auto sql = dynamic_cast<const SqlExecution *>(_execution);
            assert(sql);
            if (!sql->isFile()) {
                properties.add(String::format(ExecutionPrefix "sql", pos), sql->sql());
            } else {
                properties.add(String::format(ExecutionPrefix "file", pos), sql->fileName());
            }
        } else if (type == "python") {
            auto python = dynamic_cast<const PythonExecution *>(_execution);
            assert(python);
            if (!python->isFile()) {
                properties.add(String::format(ExecutionPrefix "script", pos), python->script());
            } else {
                properties.add(String::format(ExecutionPrefix "file", pos), python->fileName());
                properties.add(String::format(ExecutionPrefix "param", pos), python->param());
            }
        }
    }
}

void Crontab::removeYmlProperties(YmlNode::Properties &properties, int pos) {
    properties.remove(String::format(TasksPrefix "name", pos));
    properties.remove(String::format(TasksPrefix "enable", pos));
    StringArray removeKeys;
    for (auto it = properties.begin(); it != properties.end(); ++it) {
        const String &k = it.key();
        if (k.find(String::format(TasksPrefix "schedule", pos)) >= 0) {
            removeKeys.add(k);
        } else if (k.find(String::format(TasksPrefix "execution", pos)) >= 0) {
            removeKeys.add(k);
        }
    }
    for (int i = 0; i < removeKeys.count(); ++i) {
        properties.remove(removeKeys[i]);
    }
}

bool Crontab::parse(const StringMap &request, Crontab &crontab) {
    if (!request.at("name", crontab.name)) {
        return false;
    }
    if (!parseSchedule(request["schedule"], crontab._schedule)) {
        return false;
    }
    if (!parseExecution(request["execution"], crontab._execution)) {
        return false;
    }
    return true;
}

bool Crontab::parse(const YmlNode::Properties &properties, int pos, Crontab &crontab) {
    String name;
    if (properties.at(String::format(TasksPrefix "name", pos), name)) {
        crontab.name = name;
        properties.at(String::format(TasksPrefix "enable", pos), crontab.enable);
        // schedule.
        {
            String type;
            properties.at(String::format(SchedulePrefix "type", pos), type);
            if (type == "cycle") {
                TimeSpan interval;
                properties.at(String::format(SchedulePrefix "interval", pos), interval);
                if (interval != TimeSpan::Zero) {
                    crontab._schedule = new CycleSchedule(interval);
                } else {
                    Trace::error("Cycle interval is invalid.");
                }
            } else if (type == "timing") {
                DateTime time;
                properties.at(String::format(SchedulePrefix "time", pos), time);
                String repeatType, repeatValue;
                properties.at(String::format(SchedulePrefix "repeat.type", pos), repeatType);
                properties.at(String::format(SchedulePrefix "repeat.value", pos), repeatValue);
                if (TimingSchedule::allRepeatTypes().contains(repeatType, true)) {
                    crontab._schedule = new TimingSchedule(time, repeatType, repeatValue);
                } else {
                    Trace::error("Repeat type is invalid.");
                }
            } else {
                Trace::error(String::format("Can not find schedule type'%s'!", type.c_str()));
            }
        }

        // execution.
        {
            String type;
            properties.at(String::format(ExecutionPrefix "type", pos), type);
            bool sync = true;
            properties.at(String::format(ExecutionPrefix "sync.enable", pos), sync);
            TimeSpan timeout = TimeSpan::fromSeconds(10);
            properties.at(String::format(ExecutionPrefix "sync.timeout", pos), timeout);
            if (type == "app") {
                String app, param;
                properties.at(String::format(ExecutionPrefix "app", pos), app);
                properties.at(String::format(ExecutionPrefix "param", pos), param);
                crontab._execution = new AppExecution(sync, timeout, app, param);
            } else if (type == "sql") {
                String sql, fileName;
                properties.at(String::format(ExecutionPrefix "sql", pos), sql);
                properties.at(String::format(ExecutionPrefix "file", pos), fileName);
                crontab._execution = !sql.isNullOrEmpty() ?
                                     new SqlExecution(sync, timeout, sql, true) :
                                     new SqlExecution(sync, timeout, fileName, false);
            } else if (type == "python") {
                String script, fileName, param;
                properties.at(String::format(ExecutionPrefix "script", pos), script);
                properties.at(String::format(ExecutionPrefix "file", pos), fileName);
                properties.at(String::format(ExecutionPrefix "param", pos), param);
                crontab._execution = !script.isNullOrEmpty() ?
                                     new PythonExecution(sync, timeout, script) :
                                     new PythonExecution(sync, timeout, fileName, param);
            } else {
                Trace::error(String::format("Can not find the execution type'%s'!", type.c_str()));
            }
        }
        return true;
    }
    return false;
}

bool Crontab::parse(const DataRow &row, Crontab &label) {
    if (!row["name"].value().getValue(label.name)) {
        return false;
    }
    if (!parseSchedule(row["schedule"].valueStr(), label._schedule)) {
        return false;
    }
    if (!parseExecution(row["execution"].valueStr(), label._execution)) {
        return false;
    }
    return true;
}

String Crontab::toSelectSqlStr(const String &prefix, const String &name) {
    String taskTableName = getTableName(prefix, TaskTableName);
    String sql = String::format("SELECT name,schedule,execution FROM %s WHERE name='%s' LIMIT 1",
                                taskTableName.c_str(),
                                name.c_str());
    return sql;
}

String Crontab::toSelectSqlStr(const String &prefix, int pos) {
    String taskTableName = getTableName(prefix, TaskTableName);
    String sql = String::format("SELECT name,schedule,execution FROM %s ORDER BY id asc LIMIT %d,1",
                                taskTableName.c_str(),
                                pos);
    return sql;
}

String Crontab::toSelectSqlStr(const String &prefix, const SqlSelectFilter &filter) {
    String taskTableName = getTableName(prefix, TaskTableName);
    String sql = String::format("SELECT name,schedule,execution FROM %s WHERE %s ORDER BY %s LIMIT %d,%d",
                                taskTableName.c_str(),
                                filter.toLikeStr("name").c_str(),
                                !filter.orderBy().isNullOrEmpty() ? filter.orderBy().c_str() : "name asc",
                                filter.offset(), filter.limit());
    return sql;
}

String Crontab::toCountSqlStr(const String &prefix, const SqlSelectFilter &filter) {
    String taskTableName = getTableName(prefix, TaskTableName);
    String sql = String::format("SELECT COUNT(1) FROM %s WHERE %s",
                                taskTableName.c_str(),
                                filter.toLikeStr("name").c_str());
    return sql;
}

String Crontab::toInsertSqlStr(const String &prefix) const {
    String sql;
    if (isValid()) {
        DateTime now = DateTime::now();
        String taskTableName = getTableName(prefix, TaskTableName);
        sql = String::format("INSERT INTO %s VALUES(%lld,'%s','%s','%s','%s','','%s','');",
                             taskTableName.c_str(),
                             DbClient::generateSnowFlakeId(), name.c_str(),
                             _schedule->toString().c_str(), _execution->toString().c_str(),
                             now.toString().c_str(), now.toString().c_str());
    }
    return sql;
}

String Crontab::toReplaceSqlStr(const String &prefix) const {
    String sql;
    if (isValid()) {
        DateTime now = DateTime::now();
        String taskTableName = getTableName(prefix, TaskTableName);
        sql = String::format(
                "UPDATE %s SET schedule='%s',execution='%s',update_time='%s',update_user='' WHERE name='%s';",
                taskTableName.c_str(),
                _schedule->toString().c_str(), _execution->toString().c_str(),
                now.toString().c_str(), name.c_str());
    }
    return sql;
}

String Crontab::toDeleteSqlStr(const String &prefix, const String &labelName) {
    String sql;
    String taskTableName = getTableName(prefix, TaskTableName);
    sql = String::format("DELETE FROM %s WHERE name='%s';",
                         taskTableName.c_str(),
                         labelName.c_str());
    return sql;
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

String Crontab::getTableName(const String &prefix, const String &tableName) {
    return prefix.isNullOrEmpty() ? tableName : String::format("%s.%s", prefix.c_str(), tableName.c_str());
}
