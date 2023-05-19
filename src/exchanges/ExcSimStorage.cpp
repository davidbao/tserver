//
//  ExcSimStorage.cpp
//  tserver
//
//  Created by baowei on 2023/4/3.
//  Copyright (c) 2023 com. All rights reserved.
//

#include "ExcSimStorage.h"
#include "IO/Path.h"
#include "system/Math.h"
#include "system/Application.h"
#include "configuration/ConfigService.h"
#include "microservice/DataSourceService.h"
#include "../HttpErrorCode.h"

using namespace Config;
using namespace System;
using namespace Microservice;

bool ExcSimFile::load() {
    // load yml file.
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto *cs = factory->getService<IConfigService>();
    assert(cs);

    String path = Application::instance()->rootPath();
    String name = cs->getProperty(SimulatorPrefix "file.name");
    String fileName = Path::combine(path, String::format("%s.yml", name.c_str()));
    Locker locker(&_properties);
    auto result = YmlNode::loadFile(fileName, _properties);
//    const YmlNode::Properties &props = _properties;
//    for (auto it = props.begin(); it != props.end(); ++it) {
//        const String &key = it.key();
//        const String &value = it.value();
//        Trace::info(String::format("%s=%s", key.c_str(), value.c_str()));
//    }
    return result;
}

bool ExcSimFile::getLabel(const String &name, Label &label) {
    Locker locker(&_properties);
    for (int i = 0; i < MaxLabelCount; i++) {
        Label element;
        if (!Label::parse(_properties, i, element)) {
            break;
        }
        if (element.enable && name == element.name) {
            label = element;
            return true;
        }
    }
    return false;
}

bool ExcSimFile::getLabels(const SqlSelectFilter &filter, DataTable &table) {
    Locker locker(&_properties);
//    const YmlNode::Properties &props = _properties;
//    for (auto it = props.begin(); it != props.end(); ++it) {
//        const String &key = it.key();
//        const String &value = it.value();
//        Trace::info(String::format("%s=%s", key.c_str(), value.c_str()));
//    }

    Labels match;
    String name = filter.getValue("name");
    String tags = filter.getValue("tags");
    for (int i = 0; i < MaxLabelCount; i++) {
        Label label;
        if (!Label::parse(_properties, i, label)) {
            break;
        }
        if (label.enable) {
            if (label.findName(name) && label.findItemName(tags)) {
                match.add(label);
            }
        }
    }

    table.setName("label");
    table.setTotalCount((int) match.count());
    table.addColumns({
                             DataColumn("name", DbType::Text, true),
                             DataColumn("range", DbType::Text, false),
                             DataColumn("step", DbType::Float64, false),
                             DataColumn("tags", DbType::Text, false)
                     });
    size_t offset = Math::max(0, filter.offset());
    size_t count = Math::min((int) match.count(), filter.offset() + filter.limit());
    for (size_t i = offset; i < count; ++i) {
        const Label &label = match[i];
        table.addRow(label.toDataRow(table));
    }

    // order by
    table.sort(filter.orderBy());

    return true;
}

bool ExcSimFile::addLabel(const StringMap &request, StringMap &response) {
    // parse from http request.
    Label label;
    if (!Label::parse(request, label)) {
        response.addRange(HttpCode::at(FailedToParseLabel));
        return false;
    }

    // find a label by name.
    int i;
    Locker locker(&_properties);
    for (i = 0; i < MaxLabelCount; i++) {
        Label element;
        if (!Label::parse(_properties, i, element)) {
            break;
        }
        if (element.enable && label.name == element.name) {
            // Duplicate name.
            response.addRange(HttpCode::at(DuplicateName));
            return false;
        }
    }

    // save yml file.
    label.updateYmlProperties(_properties, i);
    if (!saveFile(_properties)) {
        // Failed to save config file.
        response.addRange(HttpCode::at(FailedToSave));
        return false;
    }

    return true;
}

bool ExcSimFile::updateLabel(const StringMap &request, StringMap &response) {
    // parse from http request.
    Label label;
    if (!Label::parse(request, label)) {
        response.addRange(HttpCode::at(FailedToParseLabel));
        return false;
    }

    // find a label by name.
    int i;
    bool found = false;
    Locker locker(&_properties);
    for (i = 0; i < MaxLabelCount; i++) {
        Label element;
        if (!Label::parse(_properties, i, element)) {
            break;
        }
        if (element.enable && label.name == element.name) {
            found = true;
            break;
        }
    }

    if (!found) {
        // Can not find label by name.
        response.addRange(HttpCode::at(CannotFindLabel));
        return false;
    }

    // save yml file.
    label.updateYmlProperties(_properties, i);
    if (!saveFile(_properties)) {
        // Failed to save config file.
        response.addRange(HttpCode::at(FailedToSave));
        return false;
    }

    return true;
}

bool ExcSimFile::removeLabel(const StringMap &request, StringMap &response) {
    StringArray names;
    StringArray::parseJson(request["name"], names);

    for (int i = 0; i < MaxLabelCount; i++) {
        Label element;
        if (!Label::parse(_properties, i, element)) {
            break;
        }
        if (element.enable && names.contains(element.name)) {
            element.removeYmlProperties(_properties, i);
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

bool ExcSimFile::getTable(const String &name, Table &table) {
    for (int i = 0; i < MaxTableCount; i++) {
        Table t;
        if (!Table::parse(_properties, i, t)) {
            break;
        }
        if (t.enable && name == t.name) {
            table = t;
            return true;
        }
    }
    return false;
}

bool ExcSimFile::getTables(const SqlSelectFilter &filter, DataTable &table) {
    Locker locker(&_properties);
    Tables match;
    String name = filter.getValue("name");
    String tags = filter.getValue("columns");
    for (int i = 0; i < MaxTableCount; i++) {
        Table element;
        if (!Table::parse(_properties, i, element)) {
            break;
        }
        if (element.enable) {
            if (element.findName(name) && element.findItemName(tags)) {
                match.add(element);
            }
        }
    }

    table.setName("table");
    table.setTotalCount((int) match.count());
    table.addColumns({
                             DataColumn("name", DbType::Text, true),
                             DataColumn("range", DbType::Text, false),
                             DataColumn("step", DbType::Float64, false),
                             DataColumn("rowCount", DbType::Integer32, false),
                             DataColumn("columns", DbType::Text, false)
                     });
    size_t offset = Math::max(0, filter.offset());
    size_t count = Math::min((int) match.count(), filter.offset() + filter.limit());
    for (size_t i = offset; i < count; ++i) {
        const Table &element = match[i];
        table.addRow(element.toDataRow(table));
    }

    // order by
    table.sort(filter.orderBy());

    return true;
}

bool ExcSimFile::addTable(const StringMap &request, StringMap &response) {
    // parse from http request.
    Table table;
    if (!Table::parse(request, table)) {
        response.addRange(HttpCode::at(FailedToParseTable));
        return false;
    }

    // find a table by name.
    int i;
    Locker locker(&_properties);
    for (i = 0; i < MaxTableCount; i++) {
        Table element;
        if (!Table::parse(_properties, i, element)) {
            break;
        }
        if (element.enable && table.name == element.name) {
            // Duplicate name.
            response.addRange(HttpCode::at(DuplicateName));
            return false;
        }
    }

    // save yml file.
    table.updateYmlProperties(_properties, i);
    if (!saveFile(_properties)) {
        // Failed to save config file.
        response.addRange(HttpCode::at(FailedToSave));
        return false;
    }

    return true;
}

bool ExcSimFile::updateTable(const StringMap &request, StringMap &response) {
    // parse from http request.
    Table table;
    if (!Table::parse(request, table)) {
        response.addRange(HttpCode::at(FailedToParseTable));
        return false;
    }

    // find a table by name.
    int i;
    bool found = false;
    Locker locker(&_properties);
    for (i = 0; i < MaxTableCount; i++) {
        Table element;
        if (!Table::parse(_properties, i, element)) {
            break;
        }
        if (element.enable && table.name == element.name) {
            found = true;
            break;
        }
    }

    if (!found) {
        // Can not find table by name.
        response.addRange(HttpCode::at(CannotFindTable));
        return false;
    }

    // save yml file.
    table.updateYmlProperties(_properties, i);
    if (!saveFile(_properties)) {
        // Failed to save config file.
        response.addRange(HttpCode::at(FailedToSave));
        return false;
    }

    return true;
}

bool ExcSimFile::removeTable(const StringMap &request, StringMap &response) {
    StringArray names;
    StringArray::parseJson(request["name"], names);

    for (int i = 0; i < MaxTableCount; i++) {
        Table element;
        if (!Table::parse(_properties, i, element)) {
            break;
        }
        if (element.enable && names.contains(element.name)) {
            element.removeYmlProperties(_properties, i);
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

bool ExcSimFile::saveFile(const YmlNode::Properties &properties) {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto *cs = factory->getService<IConfigService>();
    assert(cs);

    String path = Application::instance()->rootPath();
    String name = cs->getProperty(SimulatorPrefix "file.name");
    String fileName = Path::combine(path, String::format("%s.yml", name.c_str()));
    return YmlNode::saveFile(fileName, properties);
}

ExcSimDatabase::ExcSimDatabase() : _dbClient(nullptr) {
}

ExcSimDatabase::~ExcSimDatabase() {
    delete _dbClient;
    _dbClient = nullptr;
}

bool ExcSimDatabase::load() {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto *cs = factory->getService<IConfigService>();
    assert(cs);

    String userName = cs->getProperty(SimDbPrefix "username");
    String password = cs->getProperty(SimDbPrefix "password");
    String urlStr = cs->getProperty(SimDbPrefix "url");
    _dbClient = DataSourceService::open(urlStr, userName, password);
    return _dbClient != nullptr;
}

bool ExcSimDatabase::getLabel(const String &name, Label &label) {
    if (_dbClient != nullptr) {
        String sql = Label::toSelectSqlStr(getTablePrefix(), name);
        DataTable dataTable(LabelTableName);
        if (_dbClient->executeSqlQuery(sql, dataTable) && dataTable.rowCount() == 1) {
            const DataRow &row = dataTable.rows()[0];
            return Label::parse(row, label);
        }
    }
    return false;
}

bool ExcSimDatabase::getLabels(const SqlSelectFilter &filter, DataTable &table) {
    if (_dbClient != nullptr) {
        String sql;
        sql = Label::toSelectSqlStr(getTablePrefix(), filter);
        if (_dbClient->executeSqlQuery(sql, table)) {
            sql = Label::toCountSqlStr(getTablePrefix(), filter);
            int totalCount = 0;
            if(_dbClient->retrieveCount(sql, totalCount))
                table.setTotalCount(totalCount);
            return true;
        }
    }
    return false;
}

bool ExcSimDatabase::addLabel(const StringMap &request, StringMap &response) {
    if (_dbClient != nullptr) {
        // parse from http request.
        Label label;
        if (!Label::parse(request, label)) {
            response.addRange(HttpCode::at(FailedToParseTable));
            return false;
        }

        // find a label by name.
        if (containsLabel(label.name)) {
            // Duplicate name.
            response.addRange(HttpCode::at(DuplicateName));
            return false;
        }

        // insert label record.
        String sql = label.toInsertSqlStr(getTablePrefix());
        if (!_dbClient->executeSql(sql)) {
            // Simulator database error.
            response.addRange(HttpCode::at(SimulatorDbError));
            return false;
        }

        return true;
    }

    response.addRange(HttpCode::at(SimulatorDbError));
    return false;
}

bool ExcSimDatabase::updateLabel(const StringMap &request, StringMap &response) {
    if (_dbClient != nullptr) {
        // parse from http request.
        Label label;
        if (!Label::parse(request, label)) {
            response.addRange(HttpCode::at(FailedToParseTable));
            return false;
        }

        // find a label by name.
        if (!containsLabel(label.name)) {
            // Can not find label by name.
            response.addRange(HttpCode::at(CannotFindLabel));
            return false;
        }

        // replace label record.
        String sql = label.toReplaceSqlStr(getTablePrefix());
        if (!_dbClient->executeSql(sql)) {
            // Simulator database error.
            response.addRange(HttpCode::at(SimulatorDbError));
            return false;
        }

        return true;
    }

    response.addRange(HttpCode::at(SimulatorDbError));
    return false;
}

bool ExcSimDatabase::removeLabel(const StringMap &request, StringMap &response) {
    if (_dbClient != nullptr) {
        StringArray names;
        StringArray::parseJson(request["name"], names);

        // delete label record.
        String sql;
        for (size_t i = 0; i < names.count(); ++i) {
            sql.appendLine(Label::toDeleteSqlStr(getTablePrefix(), names[i]));
            if (!_dbClient->executeSql(sql)) {
                // Simulator database error.
                response.addRange(HttpCode::at(SimulatorDbError));
                return false;
            }
        }

        return true;
    }

    response.addRange(HttpCode::at(SimulatorDbError));
    return false;
}

bool ExcSimDatabase::getTable(const String &name, Table &table) {
    if (_dbClient != nullptr) {
        String sql = Table::toSelectSqlStr(getTablePrefix(), name);
        DataTable dataTable(TableTableName);
        if (_dbClient->executeSqlQuery(sql, dataTable) && dataTable.rowCount() == 1) {
            const DataRow &row = dataTable.rows()[0];
            return Table::parse(row, table);
        }
    }
    return false;
}

bool ExcSimDatabase::getTables(const SqlSelectFilter &filter, DataTable &table) {
    if (_dbClient != nullptr) {
        String sql;
        sql = Table::toSelectSqlStr(getTablePrefix(), filter);
        if (_dbClient->executeSqlQuery(sql, table)) {
            sql = Table::toCountSqlStr(getTablePrefix(), filter);
            int totalCount = 0;
            if(_dbClient->retrieveCount(sql, totalCount))
                table.setTotalCount(totalCount);
            return true;
        }
    }
    return false;
}

bool ExcSimDatabase::addTable(const StringMap &request, StringMap &response) {
    if (_dbClient != nullptr) {
        // parse from http request.
        Table table;
        if (!Table::parse(request, table)) {
            response.addRange(HttpCode::at(FailedToParseTable));
            return false;
        }

        // find a table by name.
        if (containsTable(table.name)) {
            // Duplicate name.
            response.addRange(HttpCode::at(DuplicateName));
            return false;
        }

        // insert table record.
        String sql = table.toInsertSqlStr(getTablePrefix());
        if (!_dbClient->executeSql(sql)) {
            // Simulator database error.
            response.addRange(HttpCode::at(SimulatorDbError));
            return false;
        }

        return true;
    }

    response.addRange(HttpCode::at(SimulatorDbError));
    return false;
}

bool ExcSimDatabase::updateTable(const StringMap &request, StringMap &response) {
    if (_dbClient != nullptr) {
        // parse from http request.
        Table table;
        if (!Table::parse(request, table)) {
            response.addRange(HttpCode::at(FailedToParseTable));
            return false;
        }

        // find a table by name.
        if (!containsTable(table.name)) {
            // Can not find table by name.
            response.addRange(HttpCode::at(CannotFindTable));
            return false;
        }

        // replace table record.
        String sql = table.toReplaceSqlStr(getTablePrefix());
        if (!_dbClient->executeSql(sql)) {
            // Simulator database error.
            response.addRange(HttpCode::at(SimulatorDbError));
            return false;
        }

        return true;
    }

    response.addRange(HttpCode::at(SimulatorDbError));
    return false;
}

bool ExcSimDatabase::removeTable(const StringMap &request, StringMap &response) {
    if (_dbClient != nullptr) {
        StringArray names;
        StringArray::parseJson(request["name"], names);

        // delete table record.
        String sql;
        for (size_t i = 0; i < names.count(); ++i) {
            sql.appendLine(Table::toDeleteSqlStr(getTablePrefix(), names[i]));
            if (!_dbClient->executeSql(sql)) {
                // Simulator database error.
                response.addRange(HttpCode::at(SimulatorDbError));
                return false;
            }
        }

        return true;
    }

    response.addRange(HttpCode::at(SimulatorDbError));
    return false;
}

bool ExcSimDatabase::getLabelId(const String &name, uint64_t &id) {
    if (_dbClient != nullptr) {
        String sql = String::format("SELECT id FROM %s WHERE name='%s'",
                                    getTableName(LabelTableName).c_str(), name.c_str());
        DataTable dataTable(LabelTableName);
        if (_dbClient->executeSqlQuery(sql, dataTable) && dataTable.rowCount() == 1) {
            return dataTable.rows()[0]["id"].value().getValue(id);
        }
    }
    return false;
}

bool ExcSimDatabase::containsLabel(const String &name) {
    uint64_t id;
    return getLabelId(name, id);
}

bool ExcSimDatabase::getTableId(const String &name, uint64_t &id) {
    if (_dbClient != nullptr) {
        String sql = String::format("SELECT id FROM %s WHERE name='%s'",
                                    getTableName(TableTableName).c_str(), name.c_str());
        DataTable dataTable(TableTableName);
        if (_dbClient->executeSqlQuery(sql, dataTable) && dataTable.rowCount() == 1) {
            return dataTable.rows()[0]["id"].value().getValue(id);
        }
    }
    return false;
}

bool ExcSimDatabase::containsTable(const String &name) {
    uint64_t id;
    return getTableId(name, id);
}

String ExcSimDatabase::getTablePrefix() {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto *cs = factory->getService<IConfigService>();
    assert(cs);

    String prefix;
    cs->getProperty(SimDbPrefix "table.prefix", prefix);
    return prefix;
}

String ExcSimDatabase::getTableName(const String &tableName) {
    String prefix = getTablePrefix();
    return prefix.isNullOrEmpty() ? tableName : String::format("%s.%s", prefix.c_str(), tableName.c_str());
}
