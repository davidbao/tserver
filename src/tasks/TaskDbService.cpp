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

SqlConnection *TaskDbService::connection() const {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto ds = factory->getService<IDataSourceService>();
    assert(ds);
    return ds->connection();
}

bool TaskDbService::executeSql(const String &sql) {
    SqlConnection *connection = this->connection();
    if (connection != nullptr) {
        return connection->executeSql(sql);
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
