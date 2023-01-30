//
//  StorageService.cpp
//  tserver
//
//  Created by baowei on 2022/12/16.
//  Copyright © 2022 com. All rights reserved.
//

#include "StorageService.h"
#include "IO/Path.h"
#include "IO/File.h"
#include "IO/Directory.h"
#include "IO/FileStream.h"
#include "system/Application.h"
#include "microservice/DataSourceService.h"
#include "system/ServiceFactory.h"
#include "configuration/ConfigService.h"

using namespace Config;
using namespace Microservice;

StorageService::StorageService() = default;

StorageService::~StorageService() = default;

DbClient *StorageService::dbClient() const {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto *ds = factory->getService<IDataSourceService>();
    assert(ds);
    DbClient *dbClient = ds->dbClient();
    return dbClient;
}

void StorageService::createSqlFile(const String &fileName, const String &sql) {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto *ds = factory->getService<IDataSourceService>();
    assert(ds);
    return ds->createSqlFile(fileName, sql);
}

FetchResult StorageService::getLabelValues(const String &labelName, const StringArray &tagNames, StringMap &values) {
    DbClient *dbClient = this->dbClient();
    if (dbClient == nullptr)
        return FetchResult::ConfigError;

    String sql;
    sql = getSql(labelName, 0);
    if (sql.isNullOrEmpty()) {
        sql = String::format("SELECT %s FROM %s limit 1", tagNames.toString(',').c_str(), labelName.c_str());
    }

#ifdef DEBUG
    createSqlFile("getLabelValues.sql", sql);
#endif

    DataTable table;
    if (dbClient->executeSqlQuery(sql, table)) {
        const DataRows &rows = table.rows();
        if (rows.count() != 1) {
            return FetchResult::RowCountError;
        }

        const DataRow &row = rows.at(0);
        const DataCells &cells = row.cells();
        for (size_t i = 0; i < cells.count(); i++) {
            const DataCell &cell = cells.at(i);
            for (size_t j = 0; j < tagNames.count(); j++) {
                const String &key = tagNames[j];
                if (cell.matchColumnName(key)) {
                    String value = cell.value().valueStr();
                    values.add(key, value);
                }
            }
        }
        return FetchResult::Succeed;
    }
    return FetchResult::ExecFailed;
}

FetchResult StorageService::getTableValues(const String &tableName, const SqlSelectFilter &filter, DataTable &table) {
    DbClient *dbClient = this->dbClient();
    if (dbClient == nullptr)
        return FetchResult::ConfigError;

    String sql;
    sql = getSql(tableName, 1);
    if (sql.isNullOrEmpty()) {
        sql = filter.toQuerySql(tableName);
    }

#ifdef DEBUG
    createSqlFile("getTableValues.sql", sql);
#endif

    if (dbClient->executeSqlQuery(sql, table)) {
        sql = getSql(tableName, 2);
        if (sql.isNullOrEmpty()) {
            sql = filter.toCountSql(tableName);
        }

#ifdef DEBUG
        createSqlFile("getTableCount.sql", sql);
#endif
        int totalCount = 0;
        if (dbClient->retrieveCount(sql, totalCount))
            table.setTotalCount(totalCount);
        return FetchResult::Succeed;
    }
    return FetchResult::ExecFailed;
}

String StorageService::getSqlFileName(const String &name, int sqlIndex) {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto *cs = factory->getService<IConfigService>();
    assert(cs);

    String subPath;
    cs->getProperty("summer.exchange.database.path", subPath);

    String sqlFileName;
    if (sqlIndex == 0) {
#define maxLabelCount 1000
#define labelPrefix "summer.exchange.database.labels[%d]."
        for (int i = 0; i < maxLabelCount; i++) {
            String n;
            if (!cs->getProperty(String::format(labelPrefix "name", i), n)) {
                break;
            }
            if (name == n) {
                if (cs->getProperty(String::format(labelPrefix "sql", i), sqlFileName)) {
                    break;
                }
            }
        }
    } else {
#define maxTableCount 1000
#define tablePrefix "summer.exchange.database.tables[%d]."
        for (int i = 0; i < maxTableCount; i++) {
            String n;
            if (!cs->getProperty(String::format(tablePrefix "name", i), n)) {
                break;
            }
            if (name == n) {
                const char *fmt = sqlIndex == 1 ? (tablePrefix "querySql") : (tablePrefix "countSql");
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

String StorageService::getSql(const String &name, int sqlIndex) {
    String fileName = getSqlFileName(name, sqlIndex);
    if (File::exists(fileName)) {
        FileStream fs(fileName, FileMode::FileOpenWithoutException, FileAccess::FileRead);
        String sql;
        fs.readToEnd(sql);
        return sql;
    }
    return String::Empty;
}
