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
    factory->addService<IConfigService>("TaskService", this);
}

TaskService::~TaskService() {
    if (_timer != nullptr) {
        delete _timer;
        _timer = nullptr;
    }

    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    factory->removeService<IConfigService>("TaskService");
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
            // schedule.
            Schedule *schedule = nullptr;
            {
                String type;
                cs->getProperty(String::format(schedulePrefix "type", i), type);
                if (type == "cycle") {
                    TimeSpan interval;
                    cs->getProperty(String::format(schedulePrefix "interval", i), interval);
                    if (interval != TimeSpan::Zero) {
                        schedule = new CycleSchedule(interval);
                    }
                } else if (type == "timing") {
                    DateTime time;
                    cs->getProperty(String::format(schedulePrefix "time", i), time);
                    String repeatType, repeatValue;
                    cs->getProperty(String::format(schedulePrefix "repeat.type", i), repeatType);
                    cs->getProperty(String::format(schedulePrefix "repeat.value", i), repeatValue);
                    if (time != DateTime::MinValue) {
                        schedule = new TimingSchedule(time, repeatType, repeatValue);
                    }
                } else {
                    Trace::error(String::format("Can not find schedule type'%s'!", type.c_str()));
                }
            }

            // execution.
            Execution *execution = nullptr;
            {
                String type;
                cs->getProperty(String::format(executionPrefix "type", i), type);
                if (type == "app") {
                    String app, param;
                    cs->getProperty(String::format(executionPrefix "app", i), app);
                    cs->getProperty(String::format(executionPrefix "param", i), param);
                    execution = new AppExecution(path, app, param);
                } else if (type == "sql") {
                    String sql, fileName;
                    cs->getProperty(String::format(executionPrefix "sql", i), sql);
                    cs->getProperty(String::format(executionPrefix "file", i), fileName);
                    execution = !sql.isNullOrEmpty() ?
                                new SqlExecution(sql, true) :
                                new SqlExecution(fileName, false);
                } else if (type == "python") {
                    String script, fileName, param;
                    cs->getProperty(String::format(executionPrefix "script", i), script);
                    cs->getProperty(String::format(executionPrefix "file", i), fileName);
                    cs->getProperty(String::format(executionPrefix "param", i), param);
                    execution = !script.isNullOrEmpty() ?
                                new PythonExecution(script) :
                                new PythonExecution(fileName, param);
                } else {
                }
            }

            if (schedule != nullptr && execution != nullptr) {
                auto task = new Task(name, schedule, execution);
                _tasks.add(task);
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
//    Locker locker(&_tasks);
//    Tasks match(false);
//    for (size_t i = 0; i < _tasks.count(); ++i) {
//        const Task *task = _tasks[i];
//        String name = filter.getValue("name");
//        String type = filter.getValue("type");
//        String app = filter.getValue("app");
//        if ((name.isNullOrEmpty() || task->name.find(name) >= 0) &&
//            (type.isNullOrEmpty() || task->type() == type) &&
//            (app.isNullOrEmpty() || task->app.find(app) >= 0)) {
//            match.add(task);
//        }
//    }
//
//    table.setName("task");
//    table.setTotalCount((int) match.count());
//    table.addColumns({
//                             DataColumn("name", ValueTypes::Text, true),
//                             DataColumn("app", ValueTypes::Text, false),
//                             DataColumn("param", ValueTypes::Text, false),
//                             DataColumn("interval", ValueTypes::Text, false),
//                             DataColumn("time", ValueTypes::Text, false),
//                             DataColumn("repeatType", ValueTypes::Text, false),
//                             DataColumn("repeatValue", ValueTypes::Text, false)
//                     });
//    size_t offset = Math::max(0, filter.offset());
//    size_t count = Math::min((int) match.count(), filter.offset() + filter.limit());
//    for (size_t i = offset; i < count; ++i) {
//        const Task *task = match[i];
//        table.addRow(task->toDataRow(table));
//    }

    return true;
}

bool TaskService::getTask(const StringMap &request, StringMap &response) {
    {
        Locker locker(&_tasks);
        String name = request["name"];
        for (size_t i = 0; i < _tasks.count(); ++i) {
            const Task *task = _tasks[i];
            if (task->name() == name) {
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
        if (task->name() == name) {
            // Duplicate name.
            response.addRange(HttpCode::at(DuplicateName));
            return false;
        }
    }
    return addOrUpdateTask(request, response);
}

bool TaskService::addTaskApp(const StringMap &request, StringMap &response) {
//    String name = request["name"];
//    String md5 = request["md5"];
//    String fullFileName = request["fullFileName"];
//    String app;
//
//    // find app name.
//    {
//        Locker locker(&_tasks);
//        bool found = false;
//        for (size_t i = 0; i < _tasks.count(); ++i) {
//            const Task *task = _tasks[i];
//            if (task->name() == name) {
//                found = true;
//                app = !task->app.isNullOrEmpty() ? task->app : task->name();
//                break;
//            }
//        }
//        if (!found) {
//            // Can not find task by name.
//            response.addRange(HttpCode::at(CannotFindTask));
//            return false;
//        }
//    }
//
//    // file exist?
//    if (!File::exists(fullFileName)) {
//        // Can not find the upload file.
//        response.addRange(HttpCode::at(CannotFindFile));
//        return false;
//    }
//
//    // check md5.
//    if (!md5.isNullOrEmpty()) {
//        String actual;
//        if (Md5Provider::computeFileHash(fullFileName, actual)) {
//            if (!String::equals(actual, md5, true)) {
//                // Failed to verify the upload file md5.
//                response.addRange(HttpCode::at(FailedToVerifyMd5));
//                return false;
//            }
//        }
//    }
//
//    // check task app path.
//    String path = Path::combine(getAppPath(), name);
//    if (!Directory::exists(path)) {
//        Directory::createDirectory(path);
//    }
//
//    // extract or copy app, check zip first.
//    Zip zip(fullFileName);
//    if (zip.isValid()) {
//        if (!Zip::extract(fullFileName, path)) {
//            if (Directory::exists(path)) {
//                Directory::deleteDirectory(path);
//            }
//            // Can not extract the zip file.
//            response.addRange(HttpCode::at(CannotExtractZip));
//            return false;
//        }
//    } else {
//        // not a zip file.
//        String destFileName = Path::combine(path, app);
//        if (!File::copy(fullFileName, destFileName, true)) {
//            if (Directory::exists(path)) {
//                Directory::deleteDirectory(path);
//            }
//            // Can not copy the app file.
//            response.addRange(HttpCode::at(CannotCopyApp));
//            return false;
//        }
//    }
//
//#ifndef WIN32
//    // grant app permission.
//    StringArray files;
//    Directory::getFiles(path, "*", SearchOption::AllDirectories, files);
//    for (size_t i = 0; i < files.count(); ++i) {
//        File::chmod(files[i]);
//    }
//#endif
//
//    response.addRange(HttpCode::okCode());
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
            if (task->name() == names[j]) {
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
        if (task->name() == name) {
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
//    String name = request["name"];
//    Task *task = nullptr;
//    String path = getAppPath();
//    String type = request["type"];
//    if (type == "cycle") {
//        TimeSpan interval;
//        if (TimeSpan::parse(request["interval"], interval) && interval != TimeSpan::Zero) {
//            auto *t = new CycleTask(path);
//            t->name = name;
//            t->app = request["app"];
//            t->param = request["param"];
//            t->interval = interval;
//            task = t;
//        } else {
//            // Cycle interval is invalid.
//            response.addRange(HttpCode::at(CycleInvalid));
//            return false;
//        }
//    } else if (type == "time") {
//        DateTime time;
//        if (!(DateTime::parse(request["time"], time) && time != DateTime::MinValue)) {
//            // Time interval is invalid.
//            response.addRange(HttpCode::at(TimeInvalid));
//            return false;
//        }
//        if (!TimeTask::allRepeatTypes().contains(request["repeatType"], true)) {
//            // Repeat type is invalid.
//            response.addRange(HttpCode::at(RepeatInvalid));
//            return false;
//        }
//
//        auto *t = new TimeTask(path);
//        t->name = name;
//        t->app = request["app"];
//        t->param = request["param"];
//        t->time = time;
//        t->repeatType = request["repeatType"];
//        t->repeatValue = request["repeatValue"];
//        task = t;
//    } else {
//        // Can not find task type.
//        response.addRange(HttpCode::at(CannotFindTaskType));
//        return false;
//    }
//
//    if (task != nullptr) {
//        // update profile yml file.
//        ServiceFactory *factory = ServiceFactory::instance();
//        assert(factory);
//        auto cs = factory->getService<IConfigService>();
//        assert(cs);
//
//        YmlNode::Properties properties;
//        updateYmlProperties(task, position, properties);
//        if (!cs->updateConfigFile(properties)) {
//            delete task;    // Don't forget it.
//            // Failed to save config file.
//            response.addRange(HttpCode::at(FailedToSave));
//            return false;
//        }
//
//        // update tasks in memory.
//        if (position >= 0) {
//            _tasks.set(position, task);
//        } else {
//            _tasks.add(task);
//        }
//
//        // check app file md5.
//        bool needUploadApp = true;
//        String fullFileName = Path::combine(path, task->app);
//        if (File::exists(fullFileName)) {
//            String actual;
//            if (Md5Provider::computeFileHash(fullFileName, actual)) {
//                needUploadApp = !String::equals(actual, request["appMd5"], true);
//            }
//        }
//        response["needUploadApp"] = Boolean(needUploadApp).toString();
//    }
//
//    response.addRange(HttpCode::okCode());
    return true;
}

void TaskService::updateYmlProperties(const Task *task, int position, YmlNode::Properties &properties) {
//    properties.add(String::format(taskPrefix "name", position), task->name);
//    properties.add(String::format(taskPrefix "type", position), task->type());
//    properties.add(String::format(taskPrefix "app", position), task->app);
//    properties.add(String::format(taskPrefix "param", position), task->param);
//    if (task->type() == "cycle") {
//        auto *ct = dynamic_cast<const CycleTask *>(task);
//        assert(ct != nullptr);
//        properties.add(String::format(taskPrefix "interval", position), ct->interval);
//    } else if (task->type() == "time") {
//        auto *tt = dynamic_cast<const TimeTask *>(task);
//        assert(tt != nullptr);
//        properties.add(String::format(taskPrefix "time", position), tt->time);
//        properties.add(String::format(taskPrefix "repeatType", position), tt->repeatType);
//        properties.add(String::format(taskPrefix "repeatValue", position), tt->repeatValue);
//    }
}

void TaskService::updateYmlProperties(bool enable, int position, YmlNode::Properties &properties) {
    properties.add(String::format(taskPrefix "enable", position), enable);
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
