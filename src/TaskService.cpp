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

using namespace IO;
using namespace Config;
using namespace Crypto;

TaskService::TaskService() : _timer(nullptr) {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    factory->addService<ITaskExecution>(this);
    factory->addService<IConfigService>("TaskService", this);
}

TaskService::~TaskService() {
    if (_timer != nullptr) {
        delete _timer;
        _timer = nullptr;
    }

    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    factory->removeService<ITaskExecution>();
    factory->removeService<IConfigService>("TaskService");
}

bool TaskService::initialize() {
    loadData();

    initDataSources();

    initTasks();

    _dbService.initialize();

    if (_timer == nullptr) {
        static const TimeSpan interval = TimeSpan::fromSeconds(1);
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
    String path = getAppPath();
    if (!Directory::exists(path)) {
        Directory::createDirectory(path);
    }

    Locker locker(&_tasks);
    _tasks.clear();

    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto cs = factory->getService<IConfigService>("TaskService");
    assert(cs);

    for (int i = 0; i < maxTaskCount; i++) {
        String name;
        if (!cs->getProperty(String::format(taskPrefix "name", i), name)) {
            break;
        }

        bool enable = true;
        cs->getProperty(String::format(taskPrefix "enable", i), enable);
        if (enable) {
            String type, app, param;
            cs->getProperty(String::format(taskPrefix "type", i), type);
            cs->getProperty(String::format(taskPrefix "app", i), app);
            cs->getProperty(String::format(taskPrefix "param", i), param);

            Task *task = nullptr;
            if (type == "cycle") {
                TimeSpan interval;
                cs->getProperty(String::format(taskPrefix "interval", i), interval);
                if (interval != TimeSpan::Zero) {
                    auto ct = new CycleTask(path);
                    ct->name = name;
                    ct->app = app;
                    ct->param = param;
                    ct->interval = interval;
                    task = ct;
                    _tasks.add(ct);
                }
            } else if (type == "time") {
                DateTime time;
                cs->getProperty(String::format(taskPrefix "time", i), time);
                String repeatType, repeatValue;
                cs->getProperty(String::format(taskPrefix "repeat.type", i), repeatType);
                cs->getProperty(String::format(taskPrefix "repeat.value", i), repeatValue);
                if (time != DateTime::MinValue) {
                    auto tt = new TimeTask(path);
                    tt->name = name;
                    tt->app = app;
                    tt->param = param;
                    tt->time = time;
                    tt->repeatType = repeatType;
                    tt->repeatValue = repeatValue;
                    task = tt;
                    _tasks.add(tt);
                }
            } else {
                Trace::error(String::format("Can not find task type'%s'!", type.c_str()));
            }

            if (task != nullptr) {
                for (int j = 0; j < maxActionCount; j++) {
                    String actionName;
                    if (!cs->getProperty(String::format(actionPrefix "name", i, j), actionName)) {
                        break;
                    }
                    TaskAction action(actionName);
                    task->actions.add(action);
                }
                for (int j = 0; j < maxTableCount; j++) {
                    String tableName;
                    if (!cs->getProperty(String::format(tablePrefix "name", i, j), tableName)) {
                        break;
                    }
                    bool tEnable = true;
                    cs->getProperty(String::format(tablePrefix "enable", i, j), tEnable);
                    if (tEnable) {
                        Table table(tableName);
                        cs->getProperty(String::format(tablePrefix "datasource", i, j), table.datasource);
                        cs->getProperty(String::format(tablePrefix "style", i, j), table.style);
                        for (int k = 0; k < maxColumnCount; k++) {
                            String columnName;
                            if (!cs->getProperty(String::format(columnPrefix "name", i, j, k), columnName)) {
                                break;
                            }
                            Column column(columnName);
                            cs->getProperty(String::format(columnPrefix "register", i, j, k), column.registerStr);
                            cs->getProperty(String::format(columnPrefix "style", i, j, k), column.style);
                            cs->getProperty(String::format(columnPrefix "type", i, j, k), column.type);
                            cs->getProperty(String::format(columnPrefix "pkey", i, j, k), column.pkey);
                            table.columns.add(column);
                        }
                        task->tables.add(table);
                    }
                }
            }
        }
    }
}

String TaskService::getAppPath() {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto cs = factory->getService<IConfigService>();
    assert(cs);

    String subPath;
    cs->getProperty("summer.task.path", subPath);
    const String appPath = Path::getAppPath();
    String path = Path::combine(appPath, subPath);
    if (!Directory::exists(path)) {
        Application *app = Application::instance();
        assert(app);
        path = Path::combine(app->rootPath(), subPath);
    }
    return path;
}

void TaskService::taskTimeUp() {
    Locker locker(&_tasks);
    for (size_t i = 0; i < _tasks.count(); ++i) {
        Task *task = _tasks[i];
        if (task->isTimeUp()) {
            task->execute();
        }
    }
}

bool TaskService::getTasks(const SqlSelectFilter &filter, DataTable &table) {
    Locker locker(&_tasks);
    Tasks match(false);
    for (size_t i = 0; i < _tasks.count(); ++i) {
        const Task *task = _tasks[i];
        String name = filter.getValue("name");
        String type = filter.getValue("type");
        String app = filter.getValue("app");
        if ((name.isNullOrEmpty() || task->name.find(name) >= 0) &&
            (type.isNullOrEmpty() || task->type() == type) &&
            (app.isNullOrEmpty() || task->app.find(app) >= 0)) {
            match.add(task);
        }
    }

    table.setName("task");
    table.setTotalCount((int) match.count());
    table.addColumns({
                             DataColumn("name", ValueTypes::Text, true),
                             DataColumn("app", ValueTypes::Text, false),
                             DataColumn("param", ValueTypes::Text, false),
                             DataColumn("interval", ValueTypes::Text, false),
                             DataColumn("time", ValueTypes::Text, false),
                             DataColumn("repeatType", ValueTypes::Text, false),
                             DataColumn("repeatValue", ValueTypes::Text, false)
                     });
    size_t offset = Math::max(0, filter.offset());
    size_t count = Math::min((int) match.count(), filter.offset() + filter.limit());
    for (size_t i = offset; i < count; ++i) {
        const Task *task = match[i];
        table.addRow(task->toDataRow(table));
    }

    return true;
}

bool TaskService::getTask(const StringMap &request, StringMap &response) {
    {
        Locker locker(&_tasks);
        String name = request["name"];
        for (size_t i = 0; i < _tasks.count(); ++i) {
            const Task *task = _tasks[i];
            if (task->name == name) {
                response["code"] = "0";
                response["data"] = task->toJsonNode().toString();
                return true;
            }
        }
    }

    // Can not find task by name.
    response.addRange(HttpCode::at(CannotFindTask));
    return false;
}

bool TaskService::addTask(const StringMap &request, StringMap &response) {
    Locker locker(&_tasks);
    String name = request["name"];
    for (size_t i = 0; i < _tasks.count(); ++i) {
        const Task *task = _tasks[i];
        if (task->name == name) {
            // Duplicate name.
            response.addRange(HttpCode::at(DuplicateName));
            return false;
        }
    }
    return addOrUpdateTask(request, response);
}

bool TaskService::addTaskApp(const StringMap &request, StringMap &response) {
    String name = request["name"];
    String md5 = request["md5"];
    String fullFileName = request["fullFileName"];
    String app;

    // find app name.
    {
        Locker locker(&_tasks);
        bool found = false;
        for (size_t i = 0; i < _tasks.count(); ++i) {
            const Task *task = _tasks[i];
            if (task->name == name) {
                found = true;
                app = !task->app.isNullOrEmpty() ? task->app : task->name;
                break;
            }
        }
        if (!found) {
            // Can not find task by name.
            response.addRange(HttpCode::at(CannotFindTask));
            return false;
        }
    }

    // file exist?
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

    // check task app path.
    String path = Path::combine(getAppPath(), name);
    if (!Directory::exists(path)) {
        Directory::createDirectory(path);
    }

    // extract or copy app, check zip first.
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
        String destFileName = Path::combine(path, app);
        if (!File::copy(fullFileName, destFileName, true)) {
            if (Directory::exists(path)) {
                Directory::deleteDirectory(path);
            }
            // Can not copy the app file.
            response.addRange(HttpCode::at(CannotCopyApp));
            return false;
        }
    }

#ifndef WIN32
    // grant app permission.
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
    Locker locker(&_tasks);
    bool found = false;
    Vector<int> positions;
    for (int i = (int) _tasks.count() - 1; i >= 0; --i) {
        const Task *task = _tasks[i];
        for (size_t j = 0; j < names.count(); ++j) {
            if (task->name == names[j]) {
                positions.add((int) i);
                _tasks.removeAt(i);
                found = true;
                break;
            }
        }
    }
    if (!found) {
        // Can not find task by name.
        response.addRange(HttpCode::at(CannotFindTask));
        return false;
    }

    // update profile yml file.
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto cs = factory->getService<IConfigService>();
    assert(cs);

    YmlNode::Properties properties;
    for (size_t i = 0; i < positions.count(); ++i) {
        updateYmlProperties(false, positions[i], properties);
    }
    if (!cs->updateConfigFile(properties)) {
        // Failed to save config file.
        response.addRange(HttpCode::at(FailedToSave));
        return false;
    }

    // remove task app path.
    for (size_t i = 0; i < names.count(); ++i) {
        const String &name = names[i];
        String path = Path::combine(getAppPath(), name);
        if (Directory::exists(path)) {
            Directory::deleteDirectory(path);
        }
    }

    response.addRange(HttpCode::okCode());
    return true;
}

bool TaskService::updateTask(const StringMap &request, StringMap &response) {
    Locker locker(&_tasks);
    String name = request["name"];
    int position;
    bool found = false;
    for (size_t i = 0; i < _tasks.count(); ++i) {
        const Task *task = _tasks[i];
        if (task->name == name) {
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
    Task *task = nullptr;
    String path = getAppPath();
    String type = request["type"];
    if (type == "cycle") {
        TimeSpan interval;
        if (TimeSpan::parse(request["interval"], interval) && interval != TimeSpan::Zero) {
            auto *t = new CycleTask(path);
            t->name = name;
            t->app = request["app"];
            t->param = request["param"];
            t->interval = interval;
            task = t;
        } else {
            // Cycle interval is invalid.
            response.addRange(HttpCode::at(CycleInvalid));
            return false;
        }
    } else if (type == "time") {
        DateTime time;
        if (!(DateTime::parse(request["time"], time) && time != DateTime::MinValue)) {
            // Time interval is invalid.
            response.addRange(HttpCode::at(TimeInvalid));
            return false;
        }
        if (!TimeTask::allRepeatTypes().contains(request["repeatType"], true)) {
            // Repeat type is invalid.
            response.addRange(HttpCode::at(RepeatInvalid));
            return false;
        }

        auto *t = new TimeTask(path);
        t->name = name;
        t->app = request["app"];
        t->param = request["param"];
        t->time = time;
        t->repeatType = request["repeatType"];
        t->repeatValue = request["repeatValue"];
        task = t;
    } else {
        // Can not find task type.
        response.addRange(HttpCode::at(CannotFindTaskType));
        return false;
    }

    if (task != nullptr) {
        // update profile yml file.
        ServiceFactory *factory = ServiceFactory::instance();
        assert(factory);
        auto cs = factory->getService<IConfigService>();
        assert(cs);

        YmlNode::Properties properties;
        updateYmlProperties(task, position, properties);
        if (!cs->updateConfigFile(properties)) {
            delete task;    // Don't forget it.
            // Failed to save config file.
            response.addRange(HttpCode::at(FailedToSave));
            return false;
        }

        // update tasks in memory.
        if (position >= 0) {
            _tasks.set(position, task);
        } else {
            _tasks.add(task);
        }

        // check app file md5.
        bool needUploadApp = true;
        String fullFileName = Path::combine(path, task->app);
        if (File::exists(fullFileName)) {
            String actual;
            if (Md5Provider::computeFileHash(fullFileName, actual)) {
                needUploadApp = !String::equals(actual, request["appMd5"], true);
            }
        }
        response["needUploadApp"] = Boolean(needUploadApp).toString();
    }

    response.addRange(HttpCode::okCode());
    return true;
}

void TaskService::updateYmlProperties(const Task *task, int position, YmlNode::Properties &properties) {
    properties.add(String::format(taskPrefix "name", position), task->name);
    properties.add(String::format(taskPrefix "type", position), task->type());
    properties.add(String::format(taskPrefix "app", position), task->app);
    properties.add(String::format(taskPrefix "param", position), task->param);
    if (task->type() == "cycle") {
        auto *ct = dynamic_cast<const CycleTask *>(task);
        assert(ct != nullptr);
        properties.add(String::format(taskPrefix "interval", position), ct->interval);
    } else if (task->type() == "time") {
        auto *tt = dynamic_cast<const TimeTask *>(task);
        assert(tt != nullptr);
        properties.add(String::format(taskPrefix "time", position), tt->time);
        properties.add(String::format(taskPrefix "repeatType", position), tt->repeatType);
        properties.add(String::format(taskPrefix "repeatValue", position), tt->repeatValue);
    }
}

void TaskService::updateYmlProperties(bool enable, int position, YmlNode::Properties &properties) {
    properties.add(String::format(taskPrefix "enable", position), enable);
}


void TaskService::initDataSources() {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto *cs = factory->getService<IConfigService>("TaskService");
    assert(cs);

//    cs->printProperties();

    _dss.clear();
    for (int i = 0; i < maxDataSourceCount; i++) {
        String dsName;
        if (!cs->getProperty(String::format(dataSourcePrefix "name", i), dsName)) {
            break;
        }
        DataSource *ds = nullptr;
        String type = cs->getProperty(String::format(dataSourcePrefix "type", i));
        if (String::equals(type, "Database", true)) {
            auto *db = new DbSource(dsName);
            cs->getProperty(String::format(dataSourcePrefix "username", i), db->userName);
            cs->getProperty(String::format(dataSourcePrefix "password", i), db->password);
            cs->getProperty(String::format(dataSourcePrefix "url", i), db->url);
            ds = db;
        } else if (String::equals(type, "D5000", true)) {
            auto *d5k = new D5kSource(dsName);
            cs->getProperty(String::format(dataSourcePrefix "appNo", i), d5k->appNo);
            ds = d5k;
        }
        if (ds != nullptr) {
            ds->createProvider();
            _dss.add(ds);
        }
    }
}

bool TaskService::execute(const Task *task) {
    if (task == nullptr) {
        return false;
    }

    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto ss = factory->getService<TaskDbService>();
    assert(ss);

    bool executed = false;
    for (size_t i = 0; i < task->actions.count(); ++i) {
        const TaskAction &action = task->actions[i];
        if (String::equals(action.name, "delete all", true)) {
            ss->deleteAll(task->tables);
            executed = true;
        } else if (String::equals(action.name, "insert", true)) {
            // retrieved from provider.
            for (size_t j = 0; j < task->tables.count(); ++j) {
                const Table &table = task->tables[j];
                ITaskProvider *provider = getProvider(table.datasource);
                if (provider != nullptr) {
                    DataTable dataTable(table.name);
                    if (provider->getValue(table, dataTable)) {
                        // executed insert.
                        ss->insert(table, dataTable);
                        executed = true;
                    }
                }
            }
        }
    }
    return executed;
}

ITaskProvider *TaskService::getProvider(const String &dsName) const {
    for (size_t i = 0; i < _dss.count(); ++i) {
        const DataSource *ds = _dss[i];
        if (ds->name == dsName) {
            return ds->provider();
        }
    }
    return nullptr;
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
        // todo: load from database.
    }
    return false;
}
