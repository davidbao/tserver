//
//  Execution.cpp
//  tserver
//
//  Created by baowei on 2023/8/21.
//  Copyright (c) 2023 com. All rights reserved.
//

#include "Execution.h"
#include "IO/Path.h"
#include "IO/File.h"
#include "diag/Trace.h"
#include "diag/Process.h"
#include "thread/TickTimeout.h"
#include "thread/Task.h"
#include "system/Application.h"
#include "crypto/Md5Provider.h"
#include "database/SnowFlake.h"
#include "diag/Stopwatch.h"
#include "TaskDbService.h"

using namespace Diag;
using namespace Crypto;

Execution::Execution() : _sync(true), _timeout(TimeSpan::Zero) {
}

Execution::Execution(bool sync, const TimeSpan &timeout, const StringArray &params) : _sync(sync), _timeout(timeout),
                                                                                _params(params) {
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

const StringArray &Execution::params() const {
    return _params;
}

void Execution::setParams(const StringArray &params) {
    _params = params;
}

const String &Execution::result() const {
    return _result;
}

AppExecution::AppExecution(bool sync, const TimeSpan &timeout,
                           const String &app, const StringArray &params) :
        Execution(sync, timeout, params), _app(app) {
}

bool AppExecution::equals(const Execution &other) const {
    if (!Execution::equals(other)) {
        return false;
    }
    auto p = dynamic_cast<const AppExecution *>(&other);
    assert(p);
    return _app == p->_app && _params == p->_params && _result == p->_result;
}

void AppExecution::evaluates(const Execution &other) {
    Execution::evaluates(other);
    auto p = dynamic_cast<const AppExecution *>(&other);
    assert(p);
    _app = p->_app;
    _params = p->_params;
    _result = p->_result;
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

Execution::Result AppExecution::execute() {
    String fileName = currentFile();
    if (File::exists(fileName)) {
        if (_sync) {
            Process process;
            process.setRedirectStdout(true);
            process.setWaitingTimeout(_timeout);
            bool result = Process::start(fileName, _params, &process);
            _result = process.stdoutStr();
            Trace::info(process.stdoutStr());
            if (result) {
                return !process.exist() ? Execution::Succeed : Execution::Timeout;
            } else {
                return Execution::FailedToStartProcess;
            }
        } else {
            bool result = Process::start(fileName, String::format("%s &", _params.toString(' ').c_str()));
            return result ? Execution::Succeed : Execution::FailedToStartProcess;
        }
    }
    return Execution::NotFound;
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
    node.add(JsonNode("param", _params.toString(' ')));
    node.add(JsonNode("result", _result));
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

Execution::Result SqlExecution::execute() {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto ss = factory->getService<TaskDbService>();
    assert(ss);

    if (!_sql.isNullOrEmpty()) {
        if (_sync) {
            return ss->executeSql(_sql) ? Execution::Succeed : Execution::FailedToExecuteSql;
        } else {
            Task::run(&TaskDbService::executeSql, ss, _sql);
            return Execution::Succeed;
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
                return ss->executeSql(sql) ? Execution::Succeed : Execution::FailedToExecuteSql;
            } else {
                Task::run(&TaskDbService::executeSql, ss, sql);
                return Execution::Succeed;
            }
        } else {
            Trace::error(String::format("Can not find the execution sql file'%s'!", fileName.c_str()));
            return Execution::NotFound;
        }
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

PythonExecution::PythonExecution(bool sync, const TimeSpan &timeout, const String &fileName, const StringArray &params) :
        Execution(sync, timeout, params), _fileName(fileName) {
}

bool PythonExecution::equals(const Execution &other) const {
    if (!Execution::equals(other)) {
        return false;
    }
    auto p = dynamic_cast<const PythonExecution *>(&other);
    assert(p);
    return _script == p->_script && _fileName == p->_fileName && _params == p->_params && _result == p->_result;
}

void PythonExecution::evaluates(const Execution &other) {
    Execution::evaluates(other);
    auto p = dynamic_cast<const PythonExecution *>(&other);
    assert(p);
    _script = p->_script;
    _fileName = p->_fileName;
    _params = p->_params;
    _result = p->_result;
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

bool PythonExecution::isFile() const {
    return !_fileName.isNullOrEmpty();
}

Execution::Result PythonExecution::execute() {
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
        StringArray params;
        params.add(fileName);
        if (_params.count() > 0) {
            params.addRange(_params);
        }
        static const char *PythonApp = "python";

        Execution::Result result;
        if (_sync) {
            Process process;
            process.setRedirectStdout(true);
            process.setWaitingTimeout(_timeout);
            bool r = Process::start(PythonApp, params, &process);
            _result = process.stdoutStr();
            Trace::verb(String::format("Start a process'%d', param: '%s'.", process.id(), params.toString(' ').c_str()));
            Trace::info(process.stdoutStr());
            if (r) {
                result = !process.exist() ? Execution::Succeed : Execution::Timeout;
            } else {
                result = Execution::FailedToStartProcess;
            }
        } else {
            Process process;
            bool r = Process::start(PythonApp, String::format("%s &", params.toString(' ').c_str()), &process);
            Trace::verb(String::format("Start a process'%d', param: '%s'.", process.id(), params.toString(' ').c_str()));
            if (r) {
                result = Execution::Succeed;
            } else {
                result = Execution::FailedToStartProcess;
            }
        }
        if (!_script.isNullOrEmpty()) {
            File::deleteFile(fileName);
        }
        return result;
    } else {
        Trace::error(String::format("Can not find the execution python file'%s'!", fileName.c_str()));
        return Execution::NotFound;
    }
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
        node.add(JsonNode("param", _params));
        node.add(JsonNode("result", _result));
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
