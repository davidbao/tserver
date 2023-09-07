//
//  TaskStorage.cpp
//  tserver
//
//  Created by baowei on 2023/4/7.
//  Copyright (c) 2023 com. All rights reserved.
//

#include "TaskStorage.h"
#include "IO/Path.h"
#include "IO/File.h"
#include "system/Math.h"
#include "system/Application.h"
#include "system/Environment.h"
#include "configuration/ConfigService.h"
#include "microservice/DataSourceService.h"
#include "../HttpErrorCode.h"

using namespace Config;
using namespace System;
using namespace Microservice;

bool TaskFile::load() {
    // load yml file.
    String fileName = this->fileName();
    Locker locker(&_properties);
    if (YmlNode::loadFile(fileName, _properties)) {
        File::getModifyTime(fileName, _modifyTime);
        return true;
    }
    return false;
}

void TaskFile::rescan() {
    static uint64_t tick = Environment::getTickCount();
    if (Environment::getTickCount() - tick >= 30 * 1000) {
        String fileName = this->fileName();

        DateTime modifyTime;
        File::getModifyTime(fileName, modifyTime);
        if (_modifyTime != modifyTime) {
            // reload.
            Trace::info(String::format("The file'%s' is changed, so reload it.",
                                       Path::getFileName(fileName).c_str()));
            YmlNode::Properties properties;
            if (YmlNode::loadFile(fileName, properties)) {
                _properties = properties;
                File::getModifyTime(fileName, _modifyTime);
            }
        }
    }
}

String TaskFile::fileName() const {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto *cs = factory->getService<IConfigService>();
    assert(cs);

    String path = Application::instance()->rootPath();
    String name = cs->getProperty(TaskPrefix "file.name");
    String fileName = Path::combine(path, String::format("%s.yml", name.c_str()));
    return fileName;
}

bool TaskFile::getTask(const String &name, Crontab &crontab) {
    rescan();

    Locker locker(&_properties);
    for (int i = 0; i < MaxTaskCount; i++) {
        Crontab task;
        if (!Crontab::parse(_properties, i, task)) {
            break;
        }
        if (task.enable && name == task.name) {
            crontab = task;
            return true;
        }
    }
    return false;
}

bool TaskFile::getTask(int pos, Crontab &crontab) {
    rescan();

    Locker locker(&_properties);
    return Crontab::parse(_properties, pos, crontab);
}

bool TaskFile::getTasks(const SqlSelectFilter &filter, DataTable &table) {
    rescan();

    Locker locker(&_properties);
//    const YmlNode::Properties &props = _properties;
//    for (auto it = props.begin(); it != props.end(); ++it) {
//        const String &key = it.key();
//        const String &value = it.value();
//        Trace::info(String::format("%s=%s", key.c_str(), value.c_str()));
//    }

    Crontabs match;
    String name = filter.getValue("name");
    for (int i = 0; i < MaxTaskCount; i++) {
        Crontab crontab;
        if (!Crontab::parse(_properties, i, crontab)) {
            break;
        }
        if (crontab.enable) {
            if (crontab.findName(name)) {
                match.add(crontab);
            }
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
        const Crontab &crontab = match[i];
        table.addRow(crontab.toDataRow(table));
    }

    // order by
    table.sort(filter.orderBy());

    return true;
}

bool TaskFile::addTask(const StringMap &request, StringMap &response) {
    // parse from http request.
    Crontab crontab;
    if (!Crontab::parse(request, crontab)) {
        response.addRange(HttpCode::at(FailedToParseLabel));
        return false;
    }

    // find a task by name.
    int i;
    Locker locker(&_properties);
    for (i = 0; i < MaxTaskCount; i++) {
        Crontab element;
        if (!Crontab::parse(_properties, i, element)) {
            break;
        }
        if (element.enable && crontab.name == element.name) {
            // Duplicate name.
            response.addRange(HttpCode::at(DuplicateName));
            return false;
        }
    }

    // save yml file.
    crontab.updateYmlProperties(_properties, i);
    if (!saveFile(_properties)) {
        // Failed to save config file.
        response.addRange(HttpCode::at(FailedToSave));
        return false;
    }

    return true;
}

bool TaskFile::updateTask(const StringMap &request, StringMap &response) {
    // parse from http request.
    Crontab crontab;
    if (!Crontab::parse(request, crontab)) {
        response.addRange(HttpCode::at(FailedToParseLabel));
        return false;
    }

    // find a task by name.
    int i;
    bool found = false;
    Locker locker(&_properties);
    for (i = 0; i < MaxTaskCount; i++) {
        Crontab element;
        if (!Crontab::parse(_properties, i, element)) {
            break;
        }
        if (element.enable && crontab.name == element.name) {
            found = true;
            break;
        }
    }

    if (!found) {
        // Can not find crontab by name.
        response.addRange(HttpCode::at(CannotFindLabel));
        return false;
    }

    // save yml file.
    crontab.updateYmlProperties(_properties, i);
    if (!saveFile(_properties)) {
        // Failed to save config file.
        response.addRange(HttpCode::at(FailedToSave));
        return false;
    }

    return true;
}

bool TaskFile::removeTask(const StringMap &request, StringMap &response) {
    StringArray names;
    StringArray::parseJson(request["name"], names);

    for (int i = 0; i < MaxTaskCount; i++) {
        Crontab crontab;
        if (!Crontab::parse(_properties, i, crontab)) {
            break;
        }
        if (crontab.enable && names.contains(crontab.name)) {
            Crontab::removeYmlProperties(_properties, i);
            crontab.removeFile();
        }
    }

    // save yml file.
    if (!saveFile(_properties)) {
        // Failed to save config file.
        response.addRange(HttpCode::at(FailedToSave));
        return false;
    }

    return true;
}

bool TaskFile::saveFile(const YmlNode::Properties &properties) {
    String fileName = this->fileName();
    if (YmlNode::saveFile(fileName, properties)) {
        File::getModifyTime(fileName, _modifyTime);
        return true;
    }
    return true;
}

TaskDatabase::TaskDatabase() : _connection(nullptr) {
}

TaskDatabase::~TaskDatabase() {
    if (_connection != nullptr) {
        delete _connection;
        _connection = nullptr;
    }
}

bool TaskDatabase::load() {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto *cs = factory->getService<IConfigService>();
    assert(cs);

    String userName = cs->getProperty(TaskDbPrefix "username");
    String password = cs->getProperty(TaskDbPrefix "password");
    String urlStr = cs->getProperty(TaskDbPrefix "url");
    _connection = new SqlConnection(urlStr, userName, password);
    return _connection->open();
}

bool TaskDatabase::getTask(const String &name, Crontab &crontab) {
    if (_connection->isConnected()) {
        String sql = Crontab::toSelectSqlStr(getTablePrefix(), name);
        DataTable dataTable(TaskTableName);
        if (_connection->executeSqlQuery(sql, dataTable) && dataTable.rowCount() == 1) {
            const DataRow &row = dataTable.rows()[0];
            return Crontab::parse(row, crontab);
        }
    }
    return false;
}

bool TaskDatabase::getTask(int pos, Crontab &crontab) {
    if (_connection->isConnected()) {
        String sql = Crontab::toSelectSqlStr(getTablePrefix(), pos);
        DataTable dataTable(TaskTableName);
        if (_connection->executeSqlQuery(sql, dataTable) && dataTable.rowCount() == 1) {
            const DataRow &row = dataTable.rows()[0];
            return Crontab::parse(row, crontab);
        }
    }
    return false;
}

bool TaskDatabase::getTasks(const SqlSelectFilter &filter, DataTable &table) {
    if (_connection->isConnected()) {
        String sql;
        sql = Crontab::toSelectSqlStr(getTablePrefix(), filter);
        if (_connection->executeSqlQuery(sql, table)) {
            sql = Crontab::toCountSqlStr(getTablePrefix(), filter);
            int totalCount = 0;
            if (_connection->retrieveCount(sql, totalCount))
                table.setTotalCount(totalCount);
            return true;
        }
    }
    return false;
}

bool TaskDatabase::addTask(const StringMap &request, StringMap &response) {
    if (_connection->isConnected()) {
        // parse from http request.
        Crontab crontab;
        if (!Crontab::parse(request, crontab)) {
            response.addRange(HttpCode::at(FailedToParseTable));
            return false;
        }

        // find a crontab by name.
        if (containsTask(crontab.name)) {
            // Duplicate name.
            response.addRange(HttpCode::at(DuplicateName));
            return false;
        }

        // insert crontab record.
        String sql = crontab.toInsertSqlStr(getTablePrefix());
        if (!_connection->executeSql(sql)) {
            // Simulator database error.
            response.addRange(HttpCode::at(SimulatorDbError));
            return false;
        }

        return true;
    }

    response.addRange(HttpCode::at(SimulatorDbError));
    return false;
}

bool TaskDatabase::updateTask(const StringMap &request, StringMap &response) {
    if (_connection->isConnected()) {
        // parse from http request.
        Crontab task;
        if (!Crontab::parse(request, task)) {
            response.addRange(HttpCode::at(FailedToParseTable));
            return false;
        }

        // find a task by name.
        if (!containsTask(task.name)) {
            // Can not find task by name.
            response.addRange(HttpCode::at(CannotFindLabel));
            return false;
        }

        // replace task record.
        String sql = task.toReplaceSqlStr(getTablePrefix());
        if (!_connection->executeSql(sql)) {
            // Simulator database error.
            response.addRange(HttpCode::at(SimulatorDbError));
            return false;
        }

        return true;
    }

    response.addRange(HttpCode::at(SimulatorDbError));
    return false;
}

bool TaskDatabase::removeTask(const StringMap &request, StringMap &response) {
    if (_connection->isConnected()) {
        StringArray names;
        StringArray::parseJson(request["name"], names);

        // delete task record.
        String sql;
        for (size_t i = 0; i < names.count(); ++i) {
            const String &name = names[i];

            Crontab crontab;
            if (getTask(name, crontab)) {
                if (crontab.enable && names.contains(crontab.name)) {
                    sql.appendLine(Crontab::toDeleteSqlStr(getTablePrefix(), name));
                    crontab.removeFile();
                }
            }
        }

        if (!_connection->executeSql(sql)) {
            // Simulator database error.
            response.addRange(HttpCode::at(SimulatorDbError));
            return false;
        }

        return true;
    }

    response.addRange(HttpCode::at(SimulatorDbError));
    return false;
}

bool TaskDatabase::getTaskId(const String &name, uint64_t &id) {
    if (_connection->isConnected()) {
        String sql = String::format("SELECT id FROM %s WHERE name='%s'",
                                    getTableName(TaskTableName).c_str(), name.c_str());
        DataTable dataTable(TaskTableName);
        if (_connection->executeSqlQuery(sql, dataTable) && dataTable.rowCount() == 1) {
            return dataTable.rows()[0]["id"].value().getValue(id);
        }
    }
    return false;
}

bool TaskDatabase::containsTask(const String &name) {
    uint64_t id;
    return getTaskId(name, id);
}

String TaskDatabase::getTablePrefix() {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto *cs = factory->getService<IConfigService>();
    assert(cs);

    String prefix;
    cs->getProperty(TaskDbPrefix "table.prefix", prefix);
    return prefix;
}

String TaskDatabase::getTableName(const String &tableName) {
    String prefix = getTablePrefix();
    return prefix.isNullOrEmpty() ? tableName : String::format("%s.%s", prefix.c_str(), tableName.c_str());
}
