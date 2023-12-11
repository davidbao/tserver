//
//  ExcSimStorage.cpp
//  tserver
//
//  Created by baowei on 2023/4/3.
//  Copyright (c) 2023 com. All rights reserved.
//

#include "ExcSimStorage.h"
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

bool ExcSimFile::load() {
    // load yml file.
    String fileName = this->fileName();
    Locker locker(&_properties);
    if (YmlNode::loadFile(fileName, _properties)) {
        File::getModifyTime(fileName, _modifyTime);
        return true;
    }
    return false;
}

void ExcSimFile::rescan() {
    static uint64_t tick = Environment::getTickCount();
    if (Environment::getTickCount() - tick >= 30 * 1000) {
        tick = Environment::getTickCount();

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

String ExcSimFile::fileName() const {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto *cs = factory->getService<IConfigService>();
    assert(cs);

    String path = Application::instance()->rootPath();
    String name = cs->getProperty(SimulatorPrefix "file.name");
    String fileName = Path::combine(path, String::format("%s.yml", name.c_str()));
    return fileName;
}

bool ExcSimFile::getLabel(const String &name, Label &label) {
    rescan();

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
    rescan();

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
    rescan();

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
    rescan();

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

bool ExcSimFile::getButton(const String &name, Button &button) {
    rescan();

    Locker locker(&_properties);
    for (int i = 0; i < MaxButtonCount; i++) {
        Button element;
        if (!Button::parse(_properties, i, element)) {
            break;
        }
        if (element.enable && name == element.name) {
            button = element;
            return true;
        }
    }
    return false;
}

bool ExcSimFile::getButtons(const SqlSelectFilter &filter, DataTable &table) {
    rescan();

    Locker locker(&_properties);
//    const YmlNode::Properties &props = _properties;
//    for (auto it = props.begin(); it != props.end(); ++it) {
//        const String &key = it.key();
//        const String &value = it.value();
//        Trace::info(String::format("%s=%s", key.c_str(), value.c_str()));
//    }

    Buttons match;
    String name = filter.getValue("name");
    String tags = filter.getValue("tags");
    for (int i = 0; i < MaxButtonCount; i++) {
        Button button;
        if (!Button::parse(_properties, i, button)) {
            break;
        }
        if (button.enable) {
            if (button.findName(name) && button.findItemName(tags)) {
                match.add(button);
            }
        }
    }

    table.setName("button");
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
        const Button &button = match[i];
        table.addRow(button.toDataRow(table));
    }

    // order by
    table.sort(filter.orderBy());

    return true;
}

bool ExcSimFile::addButton(const StringMap &request, StringMap &response) {
    // parse from http request.
    Button button;
    if (!Button::parse(request, button)) {
        response.addRange(HttpCode::at(FailedToParseButton));
        return false;
    }

    // find a button by name.
    int i;
    Locker locker(&_properties);
    for (i = 0; i < MaxButtonCount; i++) {
        Button element;
        if (!Button::parse(_properties, i, element)) {
            break;
        }
        if (element.enable && button.name == element.name) {
            // Duplicate name.
            response.addRange(HttpCode::at(DuplicateName));
            return false;
        }
    }

    // save yml file.
    button.updateYmlProperties(_properties, i);
    if (!saveFile(_properties)) {
        // Failed to save config file.
        response.addRange(HttpCode::at(FailedToSave));
        return false;
    }

    return true;
}

bool ExcSimFile::updateButton(const StringMap &request, StringMap &response) {
    // parse from http request.
    Button button;
    if (!Button::parse(request, button)) {
        response.addRange(HttpCode::at(FailedToParseButton));
        return false;
    }

    // find a button by name.
    int i;
    bool found = false;
    Locker locker(&_properties);
    for (i = 0; i < MaxButtonCount; i++) {
        Button element;
        if (!Button::parse(_properties, i, element)) {
            break;
        }
        if (element.enable && button.name == element.name) {
            found = true;
            break;
        }
    }

    if (!found) {
        // Can not find button by name.
        response.addRange(HttpCode::at(CannotFindButton));
        return false;
    }

    // save yml file.
    button.updateYmlProperties(_properties, i);
    if (!saveFile(_properties)) {
        // Failed to save config file.
        response.addRange(HttpCode::at(FailedToSave));
        return false;
    }

    return true;
}

bool ExcSimFile::removeButton(const StringMap &request, StringMap &response) {
    StringArray names;
    StringArray::parseJson(request["name"], names);

    for (int i = 0; i < MaxButtonCount; i++) {
        Button element;
        if (!Button::parse(_properties, i, element)) {
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
    String fileName = this->fileName();
    if (YmlNode::saveFile(fileName, properties)) {
        File::getModifyTime(fileName, _modifyTime);
        return true;
    }
    return false;
}

ExcSimDatabase::ExcSimDatabase() : _connection(nullptr) {
}

ExcSimDatabase::~ExcSimDatabase() {
    if (_connection != nullptr) {
        delete _connection;
        _connection = nullptr;
    }
}

bool ExcSimDatabase::load() {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto *cs = factory->getService<IConfigService>();
    assert(cs);

    String connectionStr = DataSourceService::createConnectionStr(SimDbPrefix);
    if (!connectionStr.isNullOrEmpty()) {
        _connection = new SqlConnection(connectionStr);
        return _connection->open();
    }
    return false;
}

SqlConnection *ExcSimDatabase::connection() {
    if (_connection != nullptr) {
        return _connection;
    } else {
        ServiceFactory *factory = ServiceFactory::instance();
        assert(factory);
        auto ds = factory->getService<IDataSourceService>();
        if (ds != nullptr) {
            return ds->connection();
        }
        return nullptr;
    }
}

bool ExcSimDatabase::getLabel(const String &name, Label &label) {
    SqlConnection *connection = this->connection();
    if (connection != nullptr) {
        if (connection->isConnected()) {
            String sql = Label::toSelectSqlStr(getSchema(), name);
            DataTable dataTable(LabelTableName);
            if (connection->executeSqlQuery(sql, dataTable) && dataTable.rowCount() == 1) {
                const DataRow &row = dataTable.rows()[0];
                return Label::parse(row, label);
            }
        }
    }
    return false;
}

bool ExcSimDatabase::getLabels(const SqlSelectFilter &filter, DataTable &table) {
    SqlConnection *connection = this->connection();
    if (connection != nullptr) {
        if (connection->isConnected()) {
            String sql;
            sql = Label::toSelectSqlStr(getSchema(), filter);
            if (connection->executeSqlQuery(sql, table)) {
                sql = Label::toCountSqlStr(getSchema(), filter);
                int totalCount = 0;
                if (connection->retrieveCount(sql, totalCount))
                    table.setTotalCount(totalCount);
                return true;
            }
        }
    }
    return false;
}

bool ExcSimDatabase::addLabel(const StringMap &request, StringMap &response) {
    SqlConnection *connection = this->connection();
    if (connection != nullptr) {
        if (connection->isConnected()) {
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
            String sql = label.toInsertSqlStr(getSchema());
            if (!connection->executeSql(sql)) {
                // Simulator database error.
                response.addRange(HttpCode::at(SimulatorDbError));
                return false;
            }

            return true;
        }
    }

    response.addRange(HttpCode::at(SimulatorDbError));
    return false;
}

bool ExcSimDatabase::updateLabel(const StringMap &request, StringMap &response) {
    SqlConnection *connection = this->connection();
    if (connection != nullptr) {
        if (connection->isConnected()) {
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
            String sql = label.toReplaceSqlStr(getSchema());
            if (!connection->executeSql(sql)) {
                // Simulator database error.
                response.addRange(HttpCode::at(SimulatorDbError));
                return false;
            }

            return true;
        }
    }

    response.addRange(HttpCode::at(SimulatorDbError));
    return false;
}

bool ExcSimDatabase::removeLabel(const StringMap &request, StringMap &response) {
    SqlConnection *connection = this->connection();
    if (connection != nullptr) {
        if (connection->isConnected()) {
            StringArray names;
            StringArray::parseJson(request["name"], names);

            // delete label record.
            String sql;
            for (size_t i = 0; i < names.count(); ++i) {
                sql.appendLine(Label::toDeleteSqlStr(getSchema(), names[i]));
                if (!connection->executeSql(sql)) {
                    // Simulator database error.
                    response.addRange(HttpCode::at(SimulatorDbError));
                    return false;
                }
            }

            return true;
        }
    }

    response.addRange(HttpCode::at(SimulatorDbError));
    return false;
}

bool ExcSimDatabase::getTable(const String &name, Table &table) {
    SqlConnection *connection = this->connection();
    if (connection != nullptr) {
        if (connection->isConnected()) {
            String sql = Table::toSelectSqlStr(getSchema(), name);
            DataTable dataTable(TableTableName);
            if (connection->executeSqlQuery(sql, dataTable) && dataTable.rowCount() == 1) {
                const DataRow &row = dataTable.rows()[0];
                return Table::parse(row, table);
            }
        }
    }
    return false;
}

bool ExcSimDatabase::getTables(const SqlSelectFilter &filter, DataTable &table) {
    SqlConnection *connection = this->connection();
    if (connection != nullptr) {
        if (connection->isConnected()) {
            String sql;
            sql = Table::toSelectSqlStr(getSchema(), filter);
            if (connection->executeSqlQuery(sql, table)) {
                sql = Table::toCountSqlStr(getSchema(), filter);
                int totalCount = 0;
                if (connection->retrieveCount(sql, totalCount))
                    table.setTotalCount(totalCount);
                return true;
            }
        }
    }
    return false;
}

bool ExcSimDatabase::addTable(const StringMap &request, StringMap &response) {
    SqlConnection *connection = this->connection();
    if (connection != nullptr) {
        if (connection->isConnected()) {
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
            String sql = table.toInsertSqlStr(getSchema());
            if (!connection->executeSql(sql)) {
                // Simulator database error.
                response.addRange(HttpCode::at(SimulatorDbError));
                return false;
            }

            return true;
        }
    }

    response.addRange(HttpCode::at(SimulatorDbError));
    return false;
}

bool ExcSimDatabase::updateTable(const StringMap &request, StringMap &response) {
    SqlConnection *connection = this->connection();
    if (connection != nullptr) {
        if (connection->isConnected()) {
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
            String sql = table.toReplaceSqlStr(getSchema());
            if (!connection->executeSql(sql)) {
                // Simulator database error.
                response.addRange(HttpCode::at(SimulatorDbError));
                return false;
            }

            return true;
        }
    }

    response.addRange(HttpCode::at(SimulatorDbError));
    return false;
}

bool ExcSimDatabase::removeTable(const StringMap &request, StringMap &response) {
    SqlConnection *connection = this->connection();
    if (connection != nullptr) {
        if (connection->isConnected()) {
            StringArray names;
            StringArray::parseJson(request["name"], names);

            // delete table record.
            String sql;
            for (size_t i = 0; i < names.count(); ++i) {
                sql.appendLine(Table::toDeleteSqlStr(getSchema(), names[i]));
                if (!connection->executeSql(sql)) {
                    // Simulator database error.
                    response.addRange(HttpCode::at(SimulatorDbError));
                    return false;
                }
            }

            return true;
        }
    }

    response.addRange(HttpCode::at(SimulatorDbError));
    return false;
}

bool ExcSimDatabase::getButton(const String &name, Button &button) {
    SqlConnection *connection = this->connection();
    if (connection != nullptr) {
        if (connection->isConnected()) {
            String sql = Button::toSelectSqlStr(getSchema(), name);
            DataTable dataTable(ButtonTableName);
            if (connection->executeSqlQuery(sql, dataTable) && dataTable.rowCount() == 1) {
                const DataRow &row = dataTable.rows()[0];
                return Button::parse(row, button);
            }
        }
    }
    return false;
}

bool ExcSimDatabase::getButtons(const SqlSelectFilter &filter, DataTable &table) {
    SqlConnection *connection = this->connection();
    if (connection != nullptr) {
        if (connection->isConnected()) {
            String sql;
            sql = Button::toSelectSqlStr(getSchema(), filter);
            if (connection->executeSqlQuery(sql, table)) {
                sql = Button::toCountSqlStr(getSchema(), filter);
                int totalCount = 0;
                if (connection->retrieveCount(sql, totalCount))
                    table.setTotalCount(totalCount);
                return true;
            }
        }
    }
    return false;
}

bool ExcSimDatabase::addButton(const StringMap &request, StringMap &response) {
    SqlConnection *connection = this->connection();
    if (connection != nullptr) {
        if (connection->isConnected()) {
            // parse from http request.
            Button button;
            if (!Button::parse(request, button)) {
                response.addRange(HttpCode::at(FailedToParseTable));
                return false;
            }

            // find a button by name.
            if (containsButton(button.name)) {
                // Duplicate name.
                response.addRange(HttpCode::at(DuplicateName));
                return false;
            }

            // insert button record.
            String sql = button.toInsertSqlStr(getSchema());
            if (!connection->executeSql(sql)) {
                // Simulator database error.
                response.addRange(HttpCode::at(SimulatorDbError));
                return false;
            }

            return true;
        }
    }

    response.addRange(HttpCode::at(SimulatorDbError));
    return false;
}

bool ExcSimDatabase::updateButton(const StringMap &request, StringMap &response) {
    SqlConnection *connection = this->connection();
    if (connection != nullptr) {
        if (connection->isConnected()) {
            // parse from http request.
            Button button;
            if (!Button::parse(request, button)) {
                response.addRange(HttpCode::at(FailedToParseTable));
                return false;
            }

            // find a button by name.
            if (!containsButton(button.name)) {
                // Can not find button by name.
                response.addRange(HttpCode::at(CannotFindButton));
                return false;
            }

            // replace button record.
            String sql = button.toReplaceSqlStr(getSchema());
            if (!connection->executeSql(sql)) {
                // Simulator database error.
                response.addRange(HttpCode::at(SimulatorDbError));
                return false;
            }

            return true;
        }
    }

    response.addRange(HttpCode::at(SimulatorDbError));
    return false;
}

bool ExcSimDatabase::removeButton(const StringMap &request, StringMap &response) {
    SqlConnection *connection = this->connection();
    if (connection != nullptr) {
        if (connection->isConnected()) {
            StringArray names;
            StringArray::parseJson(request["name"], names);

            // delete button record.
            String sql;
            for (size_t i = 0; i < names.count(); ++i) {
                sql.appendLine(Button::toDeleteSqlStr(getSchema(), names[i]));
                if (!connection->executeSql(sql)) {
                    // Simulator database error.
                    response.addRange(HttpCode::at(SimulatorDbError));
                    return false;
                }
            }

            return true;
        }
    }

    response.addRange(HttpCode::at(SimulatorDbError));
    return false;
}

bool ExcSimDatabase::getLabelId(const String &name, uint64_t &id) {
    SqlConnection *connection = this->connection();
    if (connection != nullptr) {
        if (connection->isConnected()) {
            String sql = String::format("SELECT id FROM %s WHERE name='%s'",
                                        getTableName(LabelTableName).c_str(), name.c_str());
            DataTable dataTable(LabelTableName);
            if (connection->executeSqlQuery(sql, dataTable) && dataTable.rowCount() == 1) {
                return dataTable.rows()[0]["id"].value().getValue(id);
            }
        }
    }
    return false;
}

bool ExcSimDatabase::containsLabel(const String &name) {
    uint64_t id;
    return getLabelId(name, id);
}

bool ExcSimDatabase::getTableId(const String &name, uint64_t &id) {
    SqlConnection *connection = this->connection();
    if (connection != nullptr) {
        if (connection->isConnected()) {
            String sql = String::format("SELECT id FROM %s WHERE name='%s'",
                                        getTableName(TableTableName).c_str(), name.c_str());
            DataTable dataTable(TableTableName);
            if (connection->executeSqlQuery(sql, dataTable) && dataTable.rowCount() == 1) {
                return dataTable.rows()[0]["id"].value().getValue(id);
            }
        }
    }
    return false;
}

bool ExcSimDatabase::containsTable(const String &name) {
    uint64_t id;
    return getTableId(name, id);
}

bool ExcSimDatabase::getButtonId(const String &name, uint64_t &id) {
    SqlConnection *connection = this->connection();
    if (connection != nullptr) {
        if (connection->isConnected()) {
            String sql = String::format("SELECT id FROM %s WHERE name='%s'",
                                        getTableName(ButtonTableName).c_str(), name.c_str());
            DataTable dataTable(ButtonTableName);
            if (connection->executeSqlQuery(sql, dataTable) && dataTable.rowCount() == 1) {
                return dataTable.rows()[0]["id"].value().getValue(id);
            }
        }
    }
    return false;
}

bool ExcSimDatabase::containsButton(const String &name) {
    uint64_t id;
    return getButtonId(name, id);
}

String ExcSimDatabase::getSchema() {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto *cs = factory->getService<IConfigService>();
    assert(cs);

    String schema;
    if (!cs->getProperty(SimDbPrefix "schema", schema)) {
        cs->getProperty(SimDbPrefix "table.prefix", schema);   // for compatibility
    }
    return schema;
}

String ExcSimDatabase::getTableName(const String &tableName) {
    String schema = getSchema();
    return schema.isNullOrEmpty() ? tableName : String::format("%s.%s", schema.c_str(), tableName.c_str());
}
