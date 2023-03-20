//
//  TaskDbService.h
//  tserver
//
//  Created by baowei on 2022/12/16.
//  Copyright (c) 2022 com. All rights reserved.
//

#ifndef TSERVER_TASKDBSERVICE_H
#define TSERVER_TASKDBSERVICE_H

#include "system/ServiceFactory.h"
#include "database/SqlSelectFilter.h"
#include "database/DbClient.h"
#include "TaskContext.h"

using namespace Database;
using namespace System;

class TaskDbService : public IService {
public:
    TaskDbService();

    ~TaskDbService() override;

    bool initialize();

    bool unInitialize();

    bool deleteRecords(const Tables &tables);

    bool deleteRecords(const TaskAction &action, const Variables &vars, const Tables &tables);

    bool insert(const DataTable &dataTable);

    bool executeSql(const String &sql);

private:
    DbClient *dbClient() const;

private:
    static void createSqlFile(const String &fileName, const String &sql);
};

#endif //TSERVER_TASKDBSERVICE_H
