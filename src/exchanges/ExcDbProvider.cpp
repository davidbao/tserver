//
//  ExcDbProvider.cpp
//  tserver
//
//  Created by baowei on 2022/12/16.
//  Copyright (c) 2022 com. All rights reserved.
//

#include "ExcDbProvider.h"
#include "IO/Path.h"
#include "IO/File.h"
#include "IO/FileStream.h"
#include "database/SnowFlake.h"
#include "system/Application.h"
#include "microservice/DataSourceService.h"
#include "system/ServiceFactory.h"
#include "configuration/ConfigService.h"
#include "../TaskService.h"

using namespace Config;
using namespace Microservice;

ExcDbProvider::ExcDbProvider() {
    Trace::info("The exchange type is database.");

    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto *cs = factory->getService<IConfigService>();
    assert(cs);

    _logSqlInfo = false;
    cs->getProperty(LogPrefix "sqlEnable", _logSqlInfo);
}

ExcDbProvider::~ExcDbProvider() = default;

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

FetchResult ExcDbProvider::getLabelValues(const String &labelName, const StringArray &tagNames,
                                          const SqlSelectFilter &filter, VariantMap &values) {
    SqlConnection *connection = this->connection();
    if (connection == nullptr)
        return FetchResult::DbError;

    String sql;
    sql = getSql(labelName, 0);
    if (sql.isNullOrEmpty()) {
        String prefix = getTablePrefix();
        String name = prefix.isNullOrEmpty() ? labelName : String::format("%s.%s", prefix.c_str(), labelName.c_str());
        sql = filter.toSelectSql(name, tagNames.toString(','));
//        sql = String::format("SELECT %s FROM %s limit 1", tagNames.toString(',').c_str(), name.c_str());
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

    String prefix = getTablePrefix();
    String name = prefix.isNullOrEmpty() ? tableName : String::format("%s.%s", prefix.c_str(), tableName.c_str());

    String sql;
    sql = getSql(tableName, 1);
    if (sql.isNullOrEmpty()) {
        sql = filter.toSelectSql(name, colNames.toString(','));
    }

    if (_logSqlInfo) {
        createSqlFile(String::format("getTableValues.%s.sql", tableName.c_str()), sql);
    }

    if (connection->executeSqlQuery(sql, table)) {
        sql = getSql(tableName, 2);
        if (sql.isNullOrEmpty()) {
            sql = filter.toCountSql(name);
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

FetchResult ExcDbProvider::execButton(const String &buttonName, const StringMap &params, VariantMap &results) {
    SqlConnection *connection = this->connection();
    if (connection == nullptr)
        return FetchResult::DbError;

    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto ts = factory->getService<TaskService>();
    assert(ts);
    auto storage = ts->storage();
    if (storage != nullptr) {
        static const char *idStr = "id";
        static const char *operationTimeStr = "operation_time";
        static const char *operationUserStr = "operation_user";

        Crontab crontab;
        if (storage->getTask(buttonName, crontab) && crontab.hasResult()) {
            auto saveParams = [](SqlConnection *connection, const String &buttonName,
                                 const StringMap &params, uint64_t &id) {
                StringMap other = params;
                if (!other.contains(operationTimeStr)) {
                    other.add(operationTimeStr, DateTime::now());
                }
                if (!other.contains(operationUserStr)) {
                    other.add(operationUserStr, DbValue::NullValue);
                }

                String prefix = getTablePrefix();
                String tableName = prefix.isNullOrEmpty() ?
                                   buttonName :
                                   String::format("%s.%s", prefix.c_str(), buttonName.c_str());
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
                String prefix = getTablePrefix();
                String tableName = prefix.isNullOrEmpty() ?
                                   buttonName :
                                   String::format("%s.%s", prefix.c_str(), buttonName.c_str());
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
                String prefix = getTablePrefix();
                String tableName = prefix.isNullOrEmpty() ?
                                   buttonName :
                                   String::format("%s.%s", prefix.c_str(), buttonName.c_str());
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
            crontab.execute();

            // Retrieve the results.
            if (!retrieveResult(connection, buttonName, params, id, results)) {
                Trace::error(String::format("Can not retrieve button'%s' record!", buttonName.c_str()));
            }
            return FetchResult::Succeed;
        }
    }
    return FetchResult::ExecFailed;
}

String ExcDbProvider::getSqlFileName(const String &name, int sqlIndex) {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto *cs = factory->getService<IConfigService>();
    assert(cs);

    String subPath;
    cs->getProperty(DatabasePrefix "path", subPath);

    String sqlFileName;
    if (sqlIndex == 0) {
        for (int i = 0; i < MaxLabelCount; i++) {
            String n;
            if (!cs->getProperty(String::format(DbLabelPrefix "name", i), n)) {
                break;
            }
            if (name == n) {
                if (cs->getProperty(String::format(DbLabelPrefix "sql", i), sqlFileName)) {
                    break;
                }
            }
        }
    } else {
        for (int i = 0; i < MaxTableCount; i++) {
            String n;
            if (!cs->getProperty(String::format(DbTablePrefix "name", i), n)) {
                break;
            }
            if (name == n) {
                const char *fmt = sqlIndex == 1 ? (DbTablePrefix "querySql") : (DbTablePrefix "countSql");
                if (cs->getProperty(String::format(fmt, i), sqlFileName)) {
                    break;
                }
            }
        }
    }

    const String appPath = Path::getAppPath();
    String fileName = Path::combine(Path::combine(appPath, subPath), sqlFileName);
    if (File::exists(fileName)) {
        return fileName;
    } else {
        Application *app = Application::instance();
        assert(app);
        fileName = Path::combine(Path::combine(app->rootPath(), subPath), sqlFileName);
        if (File::exists(fileName)) {
            return fileName;
        }
    }
    return String::Empty;
}

String ExcDbProvider::getSql(const String &name, int sqlIndex) {
    String fileName = getSqlFileName(name, sqlIndex);
    if (File::exists(fileName)) {
        FileStream fs(fileName, FileMode::FileOpenWithoutException, FileAccess::FileRead);
        String sql;
        fs.readToEnd(sql);
        return sql;
    }
    return String::Empty;
}

String ExcDbProvider::getTablePrefix() {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto *cs = factory->getService<IConfigService>();
    assert(cs);

    String prefix;
    cs->getProperty(DatabasePrefix "table.prefix", prefix);
    return prefix;
}
