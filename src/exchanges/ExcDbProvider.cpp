//
//  ExcDbProvider.cpp
//  tserver
//
//  Created by baowei on 2022/12/16.
//  Copyright (c) 2022 com. All rights reserved.
//

#include "ExcDbProvider.h"
#include "IO/Path.h"
#include "database/SnowFlake.h"
#include "system/Application.h"
#include "microservice/DataSourceService.h"
#include "system/ServiceFactory.h"
#include "configuration/ConfigService.h"
#include "../TaskService.h"

using namespace Config;
using namespace Microservice;

ExcDbProvider::ExcDbProvider() : _storage(nullptr) {
    Trace::info("The exchange type is database.");

    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto *cs = factory->getService<IConfigService>();
    assert(cs);

    _logSqlInfo = false;
    cs->getProperty(LogPrefix "sqlEnable", _logSqlInfo);

    loadData();
}

ExcDbProvider::~ExcDbProvider() {
    delete _storage;
    _storage = nullptr;
}

SqlConnection *ExcDbProvider::connection() const {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto *ds = factory->getService<IDataSourceService>();
    assert(ds);
    return ds->connection();
}

void ExcDbProvider::createSqlFile(const String &fileName, const String &sql) {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto ds = factory->getService<IDataSourceService>();
    assert(ds);
    return ds->createSqlFile(fileName, sql);
}

String ExcDbProvider::updateSql(const SqlSelectFilter &filter, const String &sql) {
    String str = sql.toUpper();
    const StringMap &values = filter.values();
    for (auto it = values.begin(); it != values.end(); ++it) {
        String k = it.key().toUpper();
        const String &v = it.value();
        str = str.replace(String::format("$%s", k.c_str()), v);
    }
    str = str.replace("$LIMIT", Int32(filter.limit()).toString());
    str = str.replace("$OFFSET", Int32(filter.offset()).toString());
    return str;
}

FetchResult ExcDbProvider::getLabelValues(const String &labelName, const StringArray &tagNames,
                                          const SqlSelectFilter &filter, VariantMap &values) {
    SqlConnection *connection = this->connection();
    if (connection == nullptr)
        return FetchResult::DbError;

    String schema = getSchema();
    String name = schema.isNullOrEmpty() ? labelName : String::format("%s.%s", schema.c_str(), labelName.c_str());

    String sql;
    sql = getSql(labelName, LabelSql);
    if (sql.isNullOrEmpty()) {
        sql = filter.toSelectSql(name, tagNames.toString(','));
    } else {
        sql = updateSql(filter, sql);
    }

    if (_logSqlInfo) {
        createSqlFile(String::format("getLabelValues.%s.sql", labelName.c_str()), sql);
    }

    DataTable table;
    if (connection->executeSqlQuery(sql, table)) {
        const DataRows &rows = table.rows();
        if (rows.count() != 1) {
            StringArray columnsNames = connection->getColumnName(labelName);
            for (size_t i = 0; i < columnsNames.count(); i++) {
                const String &columnName = columnsNames.at(i);
                if (tagNames.isEmpty()) {
                    values.add(columnName, Variant::NullValue);
                } else {
                    if (tagNames.contains(columnName, true)) {
                        values.add(columnName, Variant::NullValue);
                    }
                }
            }
            return FetchResult::Succeed;
        } else {
            const DataRow &row = rows.at(0);
            const DataCells &cells = row.cells();
            for (size_t i = 0; i < cells.count(); i++) {
                const DataCell &cell = cells.at(i);
                if (tagNames.isEmpty()) {
                    values.add(cell.columnName(), cell.value());
                } else {
                    for (size_t j = 0; j < tagNames.count(); j++) {
                        const String &tagName = tagNames[j];
                        if (cell.matchColumnName(tagName)) {
                            values.add(cell.columnName(), cell.value());
                        }
                    }
                }
            }
            return FetchResult::Succeed;
        }
    }
    return FetchResult::ExecFailed;
}

FetchResult ExcDbProvider::getTableValues(const String &tableName, const StringArray &colNames,
                                          const SqlSelectFilter &filter, DataTable &table) {
    SqlConnection *connection = this->connection();
    if (connection == nullptr)
        return FetchResult::DbError;

    String schema = getSchema();
    String name = schema.isNullOrEmpty() ? tableName : String::format("%s.%s", schema.c_str(), tableName.c_str());

    String sql;
    sql = getSql(tableName, TableQuerySql);
    if (sql.isNullOrEmpty()) {
        sql = filter.toSelectSql(name, colNames.toString(','));
    } else {
        sql = updateSql(filter, sql);
    }

    if (_logSqlInfo) {
        createSqlFile(String::format("getTableValues.%s.sql", tableName.c_str()), sql);
    }

    if (connection->executeSqlQuery(sql, table)) {
        sql = getSql(tableName, TableCountSql);
        if (sql.isNullOrEmpty()) {
            sql = filter.toCountSql(name);
        } else {
            sql = updateSql(filter, sql);
        }

        if (_logSqlInfo) {
            createSqlFile(String::format("getTableCount.%s.sql", tableName.c_str()), sql);
        }

        int totalCount = 0;
        if (connection->retrieveCount(sql, totalCount))
            table.setTotalCount(totalCount);
        return FetchResult::Succeed;
    }
    return FetchResult::ExecFailed;
}

FetchResult ExcDbProvider::execButton(const String &buttonName, ExecType type, const StringMap &params, VariantMap &results) {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto ts = factory->getService<TaskService>();
    assert(ts);
    auto storage = ts->storage();
    if (storage != nullptr) {
        Crontab crontab;
        if (storage->getTask(buttonName, crontab) && crontab.hasResult()) {
            if (type == ExecType::ExecTypeArgument) {
                return execByArgument(crontab, buttonName, params, results);
            } else {
                return execByTempTable(crontab, buttonName, params, results);
            }
        }
    }
    return FetchResult::ExecFailed;
}

FetchResult ExcDbProvider::execByArgument(Crontab &crontab, const String &buttonName, const StringMap &params, VariantMap &results) {
    // Invoke an execution.
    JsonNode request("params", params);
    crontab.setParams(StringArray({request.toString()}));
    if (crontab.execute()) {
        const String &result = crontab.result();
        JsonNode response;
        JsonNode::parse(result, response);
        StringArray names;
        response.getAttributeNames(names);
        for (size_t i = 0; i < names.count(); i++) {
            const String &name = names[i];
            String v;
            if (response.getAttribute(name, v)) {
                results.add(name, Variant(v));
            }
        }
        return FetchResult::Succeed;
    }
    return FetchResult::ExecFailed;
}

FetchResult ExcDbProvider::execByTempTable(Crontab &crontab, const String &buttonName, const StringMap &params, VariantMap &results) {
    SqlConnection *connection = this->connection();
    if (connection == nullptr)
        return FetchResult::DbError;

    static const char *idStr = "id";
    static const char *operationTimeStr = "operation_time";
    static const char *operationUserStr = "operation_user";

    auto saveParams = [](SqlConnection *connection, const String &buttonName,
                         const StringMap &params, uint64_t &id) {
        StringMap other = params;
        if (!other.contains(operationTimeStr)) {
            other.add(operationTimeStr, DateTime::now());
        }
        if (!other.contains(operationUserStr)) {
            other.add(operationUserStr, DbValue::NullValue);
        }

        String schema = getSchema();
        String tableName = schema.isNullOrEmpty() ?
                           buttonName :
                           String::format("%s.%s", schema.c_str(), buttonName.c_str());
        DataTable table(tableName);
        table.addColumn(DataColumn(idStr, DbType::Integer64, true));
        for (auto it = other.begin(); it != other.end(); ++it) {
            const String &key = it.key();
            table.addColumn(DataColumn(key, DbType::Text));
        }

        DataRow row;
        id = SnowFlake::generateId();
        row.addCell(DataCell(table.getColumn(idStr), id));
        for (auto it = other.begin(); it != other.end(); ++it) {
            const String &key = it.key();
            const String &value = it.value();
            row.addCell(DataCell(table.getColumn(key), value));
        }
        table.addRow(row);
        return connection->executeSqlInsert(table);
    };
    auto retrieveResult = [](SqlConnection *connection, const String &buttonName, const StringMap &params,
                             uint64_t id, VariantMap &results) {
        DataTable table;
        String schema = getSchema();
        String tableName = schema.isNullOrEmpty() ?
                           buttonName :
                           String::format("%s.%s", schema.c_str(), buttonName.c_str());
        String sql = String::format("SELECT * FROM %s WHERE ID=%" PRIu64, tableName.c_str(), id);
        if (connection->executeSqlQuery(sql, table) && table.rowCount() == 1) {
            const DataCells &cells = table.rows()[0].cells();
            for (size_t i = 0; i < cells.count(); ++i) {
                const DataCell &cell = cells[i];
                if (!(String::equals(cell.columnName(), idStr, true) ||
                      String::equals(cell.columnName(), operationTimeStr, true) ||
                      String::equals(cell.columnName(), operationUserStr, true) ||
                      params.contains(cell.columnName()))) {
                    results.add(cell.columnName(), cell.value());
                }
            }
            return true;
        }
        return false;
    };
    auto cleanRecords = [](SqlConnection *connection, const String &buttonName) {
        String schema = getSchema();
        String tableName = schema.isNullOrEmpty() ?
                           buttonName :
                           String::format("%s.%s", schema.c_str(), buttonName.c_str());
        DateTime time = DateTime::now().date().addDays(-7);
        String sql = String::format("DELETE FROM %s WHERE OPERATION_TIME<='%s'",
                                    tableName.c_str(), time.toString().c_str());
        return connection->executeSql(sql);
    };

    // Clean 7 days ago.
    cleanRecords(connection, buttonName);

    // Save params to db.
    uint64_t id;
    if (!saveParams(connection, buttonName, params, id)) {
        Trace::error(String::format("Can not save button'%s' record!", buttonName.c_str()));
    }

    // Invoke an execution.
    crontab.setParams(StringArray({UInt64(id).toString()}));
    crontab.execute();

    // Retrieve the results.
    if (!retrieveResult(connection, buttonName, params, id, results)) {
        Trace::error(String::format("Can not retrieve button'%s' record!", buttonName.c_str()));
    }

    return FetchResult::Succeed;
}

String ExcDbProvider::getSql(const String &name, ExcSqlType type) {
    if (_storage != nullptr) {
        return _storage->getSql(name, type);
    }
    return String::Empty;
}

String ExcDbProvider::getSchema() {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto *cs = factory->getService<IConfigService>();
    assert(cs);

    String schema;
    if (!cs->getProperty(DatabasePrefix "schema", schema)) {
        cs->getProperty(DatabasePrefix "table.prefix", schema);   // for compatibility
    }
    return schema;
}

bool ExcDbProvider::loadData() {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto *cs = factory->getService<IConfigService>();
    assert(cs);

    String type;
    cs->getProperty(SqlPrefix "type", type);
    if (String::equals(type, "file", true)) {
        Trace::info("Load sql from file.");
        _storage = new ExcDbSqlFile();
    } else if (String::equals(type, "database", true)) {
        Trace::info("Load sql from database.");
        _storage = new ExcDbSqlDatabase();
    }
    return _storage != nullptr && _storage->load();
}
