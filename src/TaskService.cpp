//
//  TaskService.cpp
//  tserver
//
//  Created by baowei on 2022/12/16.
//  Copyright (c) 2022 com. All rights reserved.
//

#include "configuration/ConfigService.h"
#include "IO/Path.h"
#include "IO/File.h"
#include "IO/Directory.h"
#include "diag/Trace.h"
#include "diag/Process.h"
#include "system/Application.h"
#include "thread/TickTimeout.h"
#include "system/Math.h"
#include "crypto/Md5Provider.h"
#include "IO/Zip.h"
#include "TaskService.h"
#include "HttpErrorCode.h"
#include "tasks/TaskContext.h"
#include "microservice/DataSourceService.h"


using namespace IO;
using namespace Config;
using namespace Crypto;
using namespace Microservice;

TaskService::TaskService() : _timer(nullptr), _dbClient(nullptr), _table("task") {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    factory->addService<IConfigService>("TaskService", this);
    factory->addService<TaskService>(this);
}

TaskService::~TaskService() {
    if (_dbClient != nullptr) {
        delete _dbClient;
        _dbClient = nullptr;
    }
    if (_timer != nullptr) {
        delete _timer;
        _timer = nullptr;
    }

    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    factory->removeService<IConfigService>("TaskService");
    factory->removeService<TaskService>();
}

bool TaskService::initialize() {
    loadData();

    initTasks();

    _dbService.initialize();

    if (_timer == nullptr) {
        static const TimeSpan interval = TimeSpan::fromMilliseconds(100);
        _timer = new Timer("task.timer", interval, interval, &TaskService::taskTimeUp, this);
    }

    return true;
}

bool TaskService::unInitialize() {
    if (_timer != nullptr) {
        delete _timer;
        _timer = nullptr;
    }

    _dbService.unInitialize();

    return true;
}

void TaskService::initTasks() {
    Locker locker(&_crontabs);
    _crontabs.clear();

    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto cs = factory->getService<IConfigService>();
    assert(cs);

    String type;
    cs->getProperty("summer.task.type", type);
    if (String::equals(type, "file", true)) {
        initTasks(_properties);
    } else if (String::equals(type, "database", true)) {
        initTasks(_table);
    }
}

void TaskService::initTasks(const YmlNode::Properties &properties) {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto ts = factory->getService<IConfigService>("TaskService");
    assert(ts);

    for (int i = 0; i < maxTaskCount; i++) {
        String name;
        if (!ts->getProperty(String::format(taskPrefix "name", i), name)) {
            break;
        }

        bool enable = true;
        ts->getProperty(String::format(taskPrefix "enable", i), enable);
        if (enable) {
            // schedule.
            Schedule *schedule = nullptr;
            {
                String type;
                ts->getProperty(String::format(schedulePrefix "type", i), type);
                if (type == "cycle") {
                    TimeSpan interval;
                    ts->getProperty(String::format(schedulePrefix "interval", i), interval);
                    if (interval != TimeSpan::Zero) {
                        schedule = new CycleSchedule(interval);
                    } else {
                        Trace::error("Cycle interval is invalid.");
                    }
                } else if (type == "timing") {
                    DateTime time;
                    ts->getProperty(String::format(schedulePrefix "time", i), time);
                    String repeatType, repeatValue;
                    ts->getProperty(String::format(schedulePrefix "repeat.type", i), repeatType);
                    ts->getProperty(String::format(schedulePrefix "repeat.value", i), repeatValue);
                    if (TimingSchedule::allRepeatTypes().contains(repeatType, true)) {
                        schedule = new TimingSchedule(time, repeatType, repeatValue);
                    } else {
                        Trace::error("Repeat type is invalid.");
                    }
                } else {
                    Trace::error(String::format("Can not find schedule type'%s'!", type.c_str()));
                }
            }

            // execution.
            Execution *execution = nullptr;
            {
                String type;
                ts->getProperty(String::format(executionPrefix "type", i), type);
                bool sync = true;
                ts->getProperty(String::format(executionPrefix "sync.enable", i), sync);
                TimeSpan timeout = TimeSpan::fromSeconds(10);
                ts->getProperty(String::format(executionPrefix "sync.timeout", i), timeout);
                if (type == "app") {
                    String app, param;
                    ts->getProperty(String::format(executionPrefix "app", i), app);
                    ts->getProperty(String::format(executionPrefix "param", i), param);
                    execution = new AppExecution(sync, timeout, app, param);
                } else if (type == "sql") {
                    String sql, fileName;
                    ts->getProperty(String::format(executionPrefix "sql", i), sql);
                    ts->getProperty(String::format(executionPrefix "file", i), fileName);
                    execution = !sql.isNullOrEmpty() ?
                                new SqlExecution(sync, timeout, sql, true) :
                                new SqlExecution(sync, timeout, fileName, false);
                } else if (type == "python") {
                    String script, fileName, param;
                    ts->getProperty(String::format(executionPrefix "script", i), script);
                    ts->getProperty(String::format(executionPrefix "file", i), fileName);
                    ts->getProperty(String::format(executionPrefix "param", i), param);
                    execution = !script.isNullOrEmpty() ?
                                new PythonExecution(sync, timeout, script) :
                                new PythonExecution(sync, timeout, fileName, param);
                } else {
                    Trace::error(String::format("Can not find the execution type'%s'!", type.c_str()));
                }
            }

            if (schedule != nullptr && execution != nullptr) {
                auto task = new Crontab(name, schedule, execution);
                _crontabs.add(task);
            }
        }
    }
}

void TaskService::initTasks(const DataTable &table) {
    for (size_t i = 0; i < table.rowCount(); ++i) {
        const DataRow &row = table.rows()[i];
        const DataCells &cells = row.cells();
        String name = cells["name"].valueStr();
        String scheduleStr = cells["schedule"].valueStr();
        String executionStr = cells["execution"].valueStr();

        Schedule *schedule = nullptr;
        Execution *execution = nullptr;
        Crontab::parseSchedule(scheduleStr, schedule);
        Crontab::parseExecution(executionStr, execution);
        if (schedule != nullptr && execution != nullptr) {
            auto task = new Crontab(name, schedule, execution);
            _crontabs.add(task);
        } else {
            delete schedule;
            delete execution;
        }
    }
}

void TaskService::taskTimeUp() {
    Locker locker(&_crontabs);
    for (size_t i = 0; i < _crontabs.count(); ++i) {
        Crontab *crontab = _crontabs[i];
        if (crontab->isTimeUp()) {
            crontab->execute();
        }
    }
}

bool TaskService::getTasks(const SqlSelectFilter &filter, DataTable &table) {
    Locker locker(&_crontabs);
    Crontabs match(false);
    for (size_t i = 0; i < _crontabs.count(); ++i) {
        const Crontab *crontab = _crontabs[i];
        String name = filter.getValue("name");
        if ((name.isNullOrEmpty() || crontab->name().find(name) >= 0)) {
            match.add(crontab);
        }
    }

    table.setName("task");
    table.setTotalCount((int) match.count());
    table.addColumns({
                             DataColumn("name", DbType::Text, true),
                             DataColumn("schedule", DbType::Text, false),
                             DataColumn("execution", DbType::Text, false)
                     });
    size_t offset = Math::max(0, filter.offset());
    size_t count = Math::min((int) match.count(), filter.offset() + filter.limit());
    for (size_t i = offset; i < count; ++i) {
        const Crontab *crontab = match[i];
        table.addRow(crontab->toDataRow(table));
    }

    // order by
    table.sort(filter.orderBy());

    return true;
}

bool TaskService::getTask(const StringMap &request, StringMap &response) {
    {
        Locker locker(&_crontabs);
        String name = request["name"];
        for (size_t i = 0; i < _crontabs.count(); ++i) {
            const Crontab *crontab = _crontabs[i];
            if (crontab->name() == name) {
                response["code"] = "0";
                response["data"] = crontab->toJsonNode().toString();
                return true;
            }
        }
    }

    // Can not find task by name.
    response.addRange(HttpCode::at(CannotFindTask));
    return false;
}

bool TaskService::addTask(const StringMap &request, StringMap &response) {
    Locker locker(&_crontabs);
    String name = request["name"];
    for (size_t i = 0; i < _crontabs.count(); ++i) {
        const Crontab *crontab = _crontabs[i];
        if (crontab->name() == name) {
            // Duplicate name.
            response.addRange(HttpCode::at(DuplicateName));
            return false;
        }
    }
    return addOrUpdateTask(request, response);
}

bool TaskService::addTaskFile(const StringMap &request, StringMap &response) {
    String name = request["name"];
    String md5 = request["md5"];
    String fullFileName = request["fullFileName"];

    // find file name.
    const Crontab *current = nullptr;
    {
        Locker locker(&_crontabs);
        for (size_t i = 0; i < _crontabs.count(); ++i) {
            const Crontab *crontab = _crontabs[i];
            if (crontab->isValid() && crontab->name() == name) {
                current = crontab;
                break;
            }
        }
        if (current == nullptr) {
            // Can not find task by name.
            response.addRange(HttpCode::at(CannotFindTask));
            return false;
        }
    }

    // file not exist?
    if (!File::exists(fullFileName)) {
        // Can not find the upload file.
        response.addRange(HttpCode::at(CannotFindFile));
        return false;
    }

    // check md5.
    if (!md5.isNullOrEmpty()) {
        String actual;
        if (Md5Provider::computeFileHash(fullFileName, actual)) {
            if (!String::equals(actual, md5, true)) {
                // Failed to verify the upload file md5.
                response.addRange(HttpCode::at(FailedToVerifyMd5));
                return false;
            }
        }
    }

    // check task file path.
    String file = current->execution()->currentFile();
    String path = Path::getDirectoryName(file);
    if (!Directory::exists(path)) {
        Directory::createDirectory(path);
    }

    // extract or copy file, check zip first.
    Zip zip(fullFileName);
    if (zip.isValid()) {
        if (!Zip::extract(fullFileName, path)) {
            if (Directory::exists(path)) {
                Directory::deleteDirectory(path);
            }
            // Can not extract the zip file.
            response.addRange(HttpCode::at(CannotExtractZip));
            return false;
        }
    } else {
        // not a zip file.
        if (!File::copy(fullFileName, file, true)) {
            // Can not copy the file.
            response.addRange(HttpCode::at(CannotCopyApp));
            return false;
        }
    }

#ifndef WIN32
    // grant file permission.
    StringArray files;
    Directory::getFiles(path, "*", SearchOption::AllDirectories, files);
    for (size_t i = 0; i < files.count(); ++i) {
        File::chmod(files[i]);
    }
#endif

    response.addRange(HttpCode::okCode());
    return true;
}

bool TaskService::removeTask(const StringMap &request, StringMap &response) {
    StringArray names;
    StringArray::parse(request["name"], names, ';');

    // find app name.
    Locker locker(&_crontabs);
    Crontabs removed(true);
    for (int i = (int) _crontabs.count() - 1; i >= 0; --i) {
        Crontab *crontab = _crontabs[i];
        for (size_t j = 0; j < names.count(); ++j) {
            if (crontab->name() == names[j]) {
                removed.add(crontab);
                _crontabs.removeAt(i, false);
                break;
            }
        }
    }
    if (removed.count() == 0) {
        // Can not find task by name.
        response.addRange(HttpCode::at(CannotFindTask));
        return false;
    }

    // save data.
    if (!saveData(removed)) {
        // Failed to save config file.
        response.addRange(HttpCode::at(FailedToSave));
        return false;
    }

    // remove task file.
    for (size_t i = 0; i < removed.count(); ++i) {
        Crontab *crontab = removed[i];
        crontab->removeFile();
    }

    response.addRange(HttpCode::okCode());
    return true;
}

bool TaskService::updateTask(const StringMap &request, StringMap &response) {
    Locker locker(&_crontabs);
    String name = request["name"];
    int position;
    bool found = false;
    for (size_t i = 0; i < _crontabs.count(); ++i) {
        const Crontab *crontab = _crontabs[i];
        if (crontab->name() == name) {
            found = true;
            position = (int) i;
            break;
        }
    }
    if (!found) {
        // Can not find task by name.
        response.addRange(HttpCode::at(CannotFindTask));
        return false;
    }
    return addOrUpdateTask(request, response, position);
}

bool TaskService::addOrUpdateTask(const StringMap &request, StringMap &response, int position) {
    String name = request["name"];

    bool error = false;
    Schedule *schedule = nullptr;
    {
        JsonNode scheduleNode;
        JsonNode::parse(request["schedule"], scheduleNode);
        String type = scheduleNode.getAttribute("type");
        if (type == "cycle") {
            TimeSpan interval;
            scheduleNode.getAttribute("interval", interval);
            if (interval != TimeSpan::Zero) {
                schedule = new CycleSchedule(interval);
            } else {
                // Cycle interval is invalid.
                response.addRange(HttpCode::at(CycleInvalid));
                error = true;
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
                response.addRange(HttpCode::at(RepeatInvalid));
                error = true;
            }
            schedule = new TimingSchedule(time, repeatType, repeatValue);
        } else {
            // Can not find schedule type.
            response.addRange(HttpCode::at(CannotFindScheduleType));
            error = true;
        }
    }

    Execution *execution = nullptr;
    {
        JsonNode executionNode;
        JsonNode::parse(request["execution"], executionNode);
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
        } else if (type == "sql") {
            String sql, fileName;
            executionNode.getAttribute("sql", sql);
            executionNode.getAttribute("file", fileName);
            execution = !sql.isNullOrEmpty() ?
                        new SqlExecution(sync, timeout, sql, true) :
                        new SqlExecution(sync, timeout, fileName, false);
        } else if (type == "python") {
            String script, fileName, param;
            executionNode.getAttribute("script", script);
            executionNode.getAttribute("file", fileName);
            executionNode.getAttribute("param", param);
            execution = !script.isNullOrEmpty() ?
                        new PythonExecution(sync, timeout, script) :
                        new PythonExecution(sync, timeout, fileName, param);
        } else {
            // Can not find execution type.
            response.addRange(HttpCode::at(CannotFindExecutionType));
            error = true;
        }
    }

    if (error) {
        delete schedule;
        delete execution;
        return false;
    }

    auto crontab = new Crontab(name, schedule, execution);

    // update tasks in memory.
    if (position >= 0) {
        _crontabs.set(position, crontab);
    } else {
        _crontabs.add(crontab);
    }

    // save data.
    if (!saveData(crontab)) {
        delete crontab;    // Don't forget it.
        // Failed to save config file.
        response.addRange(HttpCode::at(FailedToSave));
        return false;
    }

    // check file md5.
    bool needUpload = !execution->checkFile(request["md5"]);
    JsonNode dataNode;
    dataNode.add(JsonNode("needUpload", needUpload));
    response["data"] = dataNode.toString();

    response.addRange(HttpCode::okCode());
    return true;
}

bool TaskService::loadData() {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto cs = factory->getService<IConfigService>();
    assert(cs);

    String type;
    cs->getProperty("summer.task.type", type);
    if (String::equals(type, "file", true)) {
        // load yml file.
        String path = Application::instance()->rootPath();
        String name = cs->getProperty("summer.task.file.name");
        String fileName = Path::combine(path, String::format("%s.yml", name.c_str()));
        return YmlNode::loadFile(fileName, _properties);
    } else if (String::equals(type, "database", true)) {
        // load from database.
#define DatabasePrefix "summer.task.database."
        String userName;
        if (!cs->getProperty(DatabasePrefix "username", userName))
            return false;
        if (userName.isNullOrEmpty()) {
            Trace::error("task database user name is incorrect.");
            return false;
        }
        String password;
        if (!cs->getProperty(DatabasePrefix "password", password)) {
            Trace::error("task database password is incorrect.");
            return false;
        }

        String urlStr = cs->getProperty(DatabasePrefix "url");
        DbClient *client = DataSourceService::open(urlStr, userName, password);
        if (client != nullptr) {
            _dbClient = client;
            String sql = String::format("select * from %s", _table.name().c_str());
            return _dbClient->executeSqlQuery(sql, _table);
        } else {
            Trace::error("task database url is incorrect.");
            return false;
        }
    }
    return false;
}

bool TaskService::saveData(const Crontab *crontab) {
    Crontabs crontabs(false);
    crontabs.add(crontab);
    return saveData(crontabs);
}

bool TaskService::saveData(const Crontabs &crontabs) {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto cs = factory->getService<IConfigService>();
    assert(cs);

    String type;
    cs->getProperty("summer.task.type", type);
    if (String::equals(type, "file", true)) {
        // save yml file.
        String path = Application::instance()->rootPath();
        String name = cs->getProperty("summer.task.file.name");
        String fileName = Path::combine(path, String::format("%s.yml", name.c_str()));
        saveData(_crontabs, _properties);
        return YmlNode::saveFile(fileName, _properties);
    } else if (String::equals(type, "database", true)) {
        // insert or replace to database.
        saveData(crontabs, _table);
        if (_dbClient != nullptr) {
            return _dbClient->executeSqlReplace(_table);
        }
    }
    return false;
}

void TaskService::saveData(const Crontabs &crontabs, YmlNode::Properties &properties) {
    properties.clear();
    for (size_t i = 0; i < crontabs.count(); ++i) {
        const Crontab *crontab = crontabs[i];
        saveData(crontab, i, properties);
    }
}

void TaskService::saveData(const Crontab *crontab, size_t position, YmlNode::Properties &properties) {
    properties.add(String::format(taskPrefix "name", position), crontab->name());
    {
        String type = crontab->schedule()->type();
        properties.add(String::format(schedulePrefix "type", position), type);
        if (type == "cycle") {
            auto cycle = dynamic_cast<const CycleSchedule *>(crontab->schedule());
            assert(cycle);
            properties.add(String::format(schedulePrefix "interval", position), cycle->interval());
        } else if (type == "timing") {
            auto timing = dynamic_cast<const TimingSchedule *>(crontab->schedule());
            assert(timing);
            properties.add(String::format(schedulePrefix "time", position), timing->toTimeStr());
            properties.add(String::format(schedulePrefix "repeat.type", position), timing->repeatType());
            properties.add(String::format(schedulePrefix "repeat.value", position), timing->repeatValue());
        }
    }
    {
        String type = crontab->execution()->type();
        properties.add(String::format(executionPrefix "type", position), type);
        properties.add(String::format(executionPrefix "sync.enable", position), crontab->execution()->sync());
        properties.add(String::format(executionPrefix "sync.timeout", position), crontab->execution()->timeout());
        if (type == "app") {
            auto app = dynamic_cast<const AppExecution *>(crontab->execution());
            assert(app);
            properties.add(String::format(executionPrefix "app", position), app->app());
            properties.add(String::format(executionPrefix "param", position), app->param());
        } else if (type == "sql") {
            auto sql = dynamic_cast<const SqlExecution *>(crontab->execution());
            assert(sql);
            if (!sql->isFile()) {
                properties.add(String::format(executionPrefix "sql", position), sql->sql());
            } else {
                properties.add(String::format(executionPrefix "file", position), sql->fileName());
            }
        } else if (type == "python") {
            auto python = dynamic_cast<const PythonExecution *>(crontab->execution());
            assert(python);
            if (!python->isFile()) {
                properties.add(String::format(executionPrefix "script", position), python->script());
            } else {
                properties.add(String::format(executionPrefix "file", position), python->fileName());
                properties.add(String::format(executionPrefix "param", position), python->param());
            }
        }
    }
}

void TaskService::saveData(const Crontabs &crontabs, DataTable &table) {
    for (size_t i = 0; i < crontabs.count(); ++i) {
        const Crontab *crontab = crontabs[i];
        saveData(crontab, table);
    }
}

void TaskService::saveData(const Crontab *crontab, DataTable &table) {
    bool found = false;
    String replaceId;
    for (size_t i = 0; i < table.rowCount(); ++i) {
        const DataRow &row = table.rows()[i];
        const DataCells &cells = row.cells();
        if (crontab->name() == cells["name"].valueStr()) {
            found = true;
            replaceId = row.cells()["id"].valueStr();
            table.removeRow(i);
            break;
        }
    }

    const DataColumns &columns = table.columns();
    DataRow row = {
            DataCell(columns["id"], found ? replaceId : UInt64(DbClient::generateSnowFlakeId()).toString()),
            DataCell(columns["name"], crontab->name()),
            DataCell(columns["schedule"], crontab->schedule()->toString()),
            DataCell(columns["execution"], crontab->execution()->toString())
    };
    table.addRow(row);
}

const YmlNode::Properties &TaskService::properties() const {
    return _properties;
}

bool TaskService::setProperty(const String &key, const String &value) {
    _properties.add(key, value);
    return true;
}

bool TaskService::updateConfigFile(const YmlNode::Properties &properties) {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto cs = factory->getService<IConfigService>();
    assert(cs);

    String path = Application::instance()->rootPath();
    String name = cs->getProperty("summer.task.file.name");
    String fileName = Path::combine(path, String::format("%s.yml", name.c_str()));
    bool result = YmlNode::updateFile(fileName, properties);
    if (result) {
        // update properties in memory.
        StringArray keys;
        properties.keys(keys);
        for (size_t i = 0; i < keys.count(); i++) {
            const String &key = keys[i];
            String value;
            if (properties.at(key, value)) {
                _properties.add(key, value);
            }
        }
    }
    return result;
}
