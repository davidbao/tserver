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

bool TaskDbService::deleteRecords(const Tables &tables) {
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
        createSqlFile("deleteRecords.sql", sql);
#endif
        return dbClient->executeSql(sql);
    }
    return false;
}

bool TaskDbService::deleteRecords(const TaskAction &action, const Variables &vars, const Tables &tables) {
    if (action.style.isEmpty()) {
        return deleteRecords(tables);
    } else {
        if (tables.count() == 0) {
            return false;
        }

        DbClient *dbClient = this->dbClient();
        if (dbClient != nullptr) {
            String sql;
            for (size_t i = 0; i < tables.count(); i++) {
                const Table &table = tables[i];
                const String where = action.style["where"];
                if (!where.isNullOrEmpty()) {
                    for (size_t j = 0; j < vars.count(); ++j) {
                        const Variable &var = vars[j];
                        const String &name = var.name;
                        String varName = String::format("$%s", name.c_str());
                        if (where.find(varName) > 0) {
                            const Variable::Values &values = var.values();
                            for (size_t k = 0; k < values.count(); ++k) {
                                String whereStr = String::replace(where, varName, values.at(k).toString());
                                sql.appendLine(String::format("DELETE FROM %s WHERE %s;",
                                                              table.name.c_str(), whereStr.c_str()));
                            }
                        }
                    }
                } else {
                    sql.appendLine(String::format("DELETE FROM %s;", table.name.c_str()));
                }
            }
#ifdef DEBUG
            createSqlFile("deleteRecords.sql", sql);
#endif
            return dbClient->executeSql(sql);
        }
        return false;
    }
}

bool TaskDbService::insert(const DataTable &dataTable) {
    if (dataTable.rowCount() == 0) {
        return false;
    }

    DbClient *dbClient = this->dbClient();
    if (dbClient != nullptr) {
        return dbClient->executeSqlInsert(dataTable);
    }
    return false;
}

bool TaskDbService::executeSql(const String &sql) {
    DbClient *dbClient = this->dbClient();
    if (dbClient != nullptr) {
        return dbClient->executeSql(sql);
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
