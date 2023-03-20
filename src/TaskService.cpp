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
    factory->addService<ITaskProviders>(this);
    factory->addService<IConfigService>("TaskService", this);
}

TaskService::~TaskService() {
    if (_timer != nullptr) {
        delete _timer;
        _timer = nullptr;
    }

    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    factory->removeService<ITaskProviders>();
    factory->removeService<IConfigService>("TaskService");
}

bool TaskService::initialize() {
    loadData();

    initDataSources();

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
                    String script, fileName;
                    cs->getProperty(String::format(executionPrefix "script", i), script);
                    cs->getProperty(String::format(executionPrefix "file", i), fileName);
                    execution = !script.isNullOrEmpty() ?
                            new PythonExecution(script, true) :
                            new PythonExecution(fileName, false);
                } else { // inner.
                    InnerExecution *ie = new InnerExecution();

                    // vars.
                    for (int j = 0; j < maxVarCount; j++) {
                        String varName;
                        if (!cs->getProperty(String::format(varPrefix "name", i, j), varName)) {
                            break;
                        }
                        Variable var(varName);
                        cs->getProperty(String::format(varPrefix "value", i, j), var.value);
                        ie->vars.add(var);
                    }
                    // actions.
                    for (int j = 0; j < maxActionCount; j++) {
                        String actionName;
                        if (!cs->getProperty(String::format(actionPrefix "name", i, j), actionName)) {
                            break;
                        }
                        TaskAction action(actionName);
                        cs->getProperty(String::format(actionPrefix "style", i, j), action.style);
                        ie->actions.add(action);
                    }
                    // tables.
                    for (int j = 0; j < maxTableCount; j++) {
                        String tableName;
                        if (!cs->getProperty(String::format(tablePrefix "name", i, j), tableName)) {
                            break;
                        }
                        bool tEnable = true;
                        cs->getProperty(String::format(tablePrefix "enable", i, j), tEnable);
                        if (tEnable) {
                            Table table(tableName);
                            cs->getProperty(String::format(tablePrefix "register", i, j), table.registerStr);
                            cs->getProperty(String::format(tablePrefix "style", i, j), table.style);
                            for (int k = 0; k < maxColumnCount; k++) {
                                String columnName;
                                if (!cs->getProperty(String::format(columnPrefix "name", i, j, k), columnName)) {
                                    break;
                                }
                                Column column(columnName);
                                cs->getProperty(String::format(columnPrefix "register", i, j, k), column.registerStr);
                                // update column style from table datasource and style.
                                if (!table.style.isEmpty()) {
                                    column.style.addRange(table.style);
                                }
                                cs->getProperty(String::format(columnPrefix "style", i, j, k), column.style);
                                cs->getProperty(String::format(columnPrefix "type", i, j, k), column.type);
                                cs->getProperty(String::format(columnPrefix "pkey", i, j, k), column.pkey);
                                table.columns.add(column);
                            }
                            ie->tables.add(table);
                        }
                    }
                    execution = ie;
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
        if (String::equals(type, "database", true)) {
            auto db = new DbSource(dsName);
            cs->getProperty(String::format(dataSourcePrefix "username", i), db->userName);
            cs->getProperty(String::format(dataSourcePrefix "password", i), db->password);
            cs->getProperty(String::format(dataSourcePrefix "url", i), db->url);
            ds = db;
        } else if (String::equals(type, "D5000", true) || String::equals(type, "D5K", true)) {
            auto d5k = new D5kSource(dsName);
            cs->getProperty(String::format(dataSourcePrefix "appNo", i), d5k->appNo);
            ds = d5k;
        }
        if (ds != nullptr) {
            ds->createProvider();
            _dss.add(ds);
        }
    }
}

//bool TaskService::execute(const Task *task) {
//    if (task == nullptr) {
//        return false;
//    }
//
//    ServiceFactory *factory = ServiceFactory::instance();
//    assert(factory);
//    auto ss = factory->getService<TaskDbService>();
//    assert(ss);
//
//    // calc vars.
//    Variables vars;
//    updateVars(task, vars);
//
//    // execute.
//    bool executed = false;
//    for (size_t i = 0; i < task->actions.count(); ++i) {
//        const TaskAction &action = task->actions[i];
//        if (String::equals(action.name, "delete all", true)) {
//            ss->deleteRecords(task->tables);
//            executed = true;
//        } else if (String::equals(action.name, "delete", true)) {
//            ss->deleteRecords(action, vars, task->tables);
//            executed = true;
//        } else if (String::equals(action.name, "insert", true)) {
//            // retrieved from provider.
//            for (size_t j = 0; j < task->tables.count(); ++j) {
//                const Table &table = task->tables[j];
//                DataTable dataTable(table.name);
//                if (getValue(action, vars, table, dataTable)) {
//                    ss->insert(dataTable);
//                    executed = true;
//                }
//            }
//        }
//    }
//    return executed;
//}
//
//void TaskService::updateVars(const Task *task, Variables &vars) const {
//    for (size_t i = 0; i < task->vars.count(); ++i) {
//        const TaskVar &var = task->vars[i];
//        ITaskProvider *provider = getProvider(var.value["datasource"]);
//        if (provider != nullptr) {
//            auto v = unique_ptr<Values>(new Values());
//            if (provider->getValue(var.value, v.get())) {
//                vars.add(var.name, v.release());
//            }
//        } else {
//            String str = var.value.toString();
//            double minValue, maxValue, step;
//            if (parseDoubleStyle(str, minValue, maxValue, step)) {
//                auto values = new Values();
//                for (double v = minValue; v < maxValue; v += step) {
//                    values->add(Variant(v));
//                }
//                vars.add(var.name, values);
//                continue;
//            }
//
//            StringArray texts;
//            StringArray::parse(str, texts, ';');
//            if (texts.count() > 0) {
//                auto values = new Values();
//                for (size_t j = 0; j < texts.count(); j++) {
//                    values->add(Variant(texts[j].trim(' ')));
//                }
//                vars.add(var.name, values);
//                continue;
//            }
//
//            auto values = new Values{
//                    Variant(var.value.toString())
//            };
//            vars.add(var.name, values);
//        }
//    }
//}

//bool TaskService::getValue(const TaskAction &action, const Variables &vars, const Table &table, DataTable &dataTable) {
//    dataTable.setName(table.name);
//    const Columns &columns = table.columns;
//    dataTable.addColumns(columns.toColumns());
//
//    if (String::equals(table.registerStr, "database", true)) {
//        ITaskProvider *provider = getProvider(table.style["datasource"]);
//        if (provider != nullptr) {
//            return provider->getValue(table, dataTable);
//        }
//    } else {
//        const String where = action.style["where"];
//        if (!where.isNullOrEmpty()) {
//            for (auto it = vars.begin(); it != vars.end(); ++it) {
//                const String &name = it.key();
//                String varName = String::format("$%s", name.c_str());
//                if (where.find(varName) > 0) {
//                    const Values *values = it.value();
//                    for (size_t j = 0; j < values->count(); ++j) {
//                        const Variant &value = values->at(j);
//                        String whereStr = String::replace(where, varName, values->at(j).toString());
//                        addRow(dataTable, action, columns, {varName, values->at(j).toString()});
//                    }
//                }
//            }
//        } else {
//            addRow(dataTable, action, columns);
//        }
//    }
//
//    return true;
//}

//void TaskService::addRow(DataTable &dataTable, const TaskAction &action,
//                         const Columns &columns, const Variable &var) const {
//    DataRow row;
//    for (size_t i = 0; i < dataTable.columnCount(); ++i) {
//        const DataColumn &c = dataTable.columns()[i];
//        Column column;
//        if (columns.getColumn(c.name(), column)) {
//            if (String::equals(column.registerStr, "SnowFlake", true)) {
//                row.addCell(DataCell(c, DbClient::generateSnowFlakeId()));
//            } else if (String::equals(column.registerStr, "Uuid", true)) {
//                row.addCell(DataCell(c, Uuid::generate().toString()));
//            } else if (String::equals(column.registerStr, "database", true) ||
//                       String::equals(column.registerStr, "rtdb", true)) {
//                ITaskProvider *provider = getProvider(column.style["datasource"]);
//                if (provider != nullptr) {
//                    if (!var.isEmpty()) {
//                        // append where str in column sql.
//                        const String where = action.style["where"];
//                        String whereStr = String::replace(where, var.name, var.value);
//                        String sql = String::format("%s WHERE %s",
//                                                    column.style["sql"].toString().c_str(),
//                                                    whereStr.c_str());
//                        column.style["sql"] = sql;
//                    }
//
//                    DbValue value;
//                    if (provider->getValue(column, value)) {
//                        row.addCell(DataCell(c, value));
//                    } else {
//                        row.addCell(DataCell(c, DbValue::NullValue));
//                    }
//                }
//            } else if (String::equals(column.registerStr, "now", true)) {
//                DateTime now = DateTime::now();
//                row.addCell(DataCell(c, now.toString(column.style["format"].trim(' ', ';', '\''))));
//            } else {
//                if (!var.isEmpty()) {
//                    String valueStr = String::replace(column.registerStr, var.name, var.value);
//                    row.addCell(DataCell(c, DbValue(c.type(), valueStr)));
//                } else {
//                    row.addCell(DataCell(c, DbValue(c.type(), column.registerStr)));
//                }
//            }
//        }
//    }
//    dataTable.addRow(row);
//}

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
