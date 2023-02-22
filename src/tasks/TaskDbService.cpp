//
//  TaskDbService.cpp
//  tserver
//
//  Created by baowei on 2022/12/16.
//  Copyright (c) 2022 com. All rights reserved.
//

#include "TaskDbService.h"
#include "system/Application.h"
#include "microservice/DataSourceService.h"
#include "system/ServiceFactory.h"

using namespace Microservice;

TaskDbService::TaskDbService() {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    factory->addService<TaskDbService>(this);
}

TaskDbService::~TaskDbService() {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    factory->removeService<TaskDbService>();
}

bool TaskDbService::initialize() {
    return true;
}

bool TaskDbService::unInitialize() {
    return true;
}

DbClient *TaskDbService::dbClient() const {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto ds = factory->getService<IDataSourceService>();
    assert(ds);
    DbClient *dbClient = ds->dbClient();
    return dbClient;
}

bool TaskDbService::deleteAll(const Tables &tables) {
    if (tables.count() == 0) {
        return false;
    }

    DbClient *dbClient = this->dbClient();
    if (dbClient != nullptr) {
        String sql;
        for (size_t i = 0; i < tables.count(); i++) {
            const Table &table = tables[i];
            sql.appendLine(String::format("DELETE FROM %s;", table.name.c_str()));
        }
#ifdef DEBUG
        createSqlFile("deleteAll.sql", sql);
#endif
        return dbClient->executeSql(sql);
    }
    return false;
}

bool TaskDbService::insert(const Table &table, const DataTable &dataTable) {
    if (table.columns.count() == 0) {
        return false;
    }
    if (dataTable.rowCount() == 0) {
        return false;
    }

    DbClient *dbClient = this->dbClient();
    if (dbClient != nullptr) {
        DataTable t(table.name);
        t.addColumns(table.columns.toColumns());

        for (size_t i = 0; i < dataTable.rowCount(); ++i) {
            DataRow r;
            const DataRow &row = dataTable.rows()[i];
            for (size_t j = 0; j < t.columnCount(); ++j) {
                const DataColumn &c = t.columns()[j];
                Column column;
                if (table.columns.getColumn(c.name(), column)) {
                    if (String::equals(column.registerStr, "SnowFlake", true)) {
                        r.addCell(DataCell(c, DbClient::generateSnowFlakeId()));
                    } else if (String::equals(column.registerStr, "Uuid", true)) {
                        r.addCell(DataCell(c, Uuid::generate().toString()));
                    } else {
                        const DataCell &cell = row.cells().at(c.name());
                        if (!cell.isNullValue()) {
                            r.addCell(DataCell(c, cell.value()));
                        }
                    }
                }
            }
            t.addRow(r);
        }
        return dbClient->executeSqlInsert(t);
    }
    return false;
}

void TaskDbService::createSqlFile(const String &fileName, const String &sql) {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto *ds = factory->getService<IDataSourceService>();
    assert(ds);
    return ds->createSqlFile(fileName, sql);
}
