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
#include "database/SqlConnection.h"
#include "TaskContext.h"

using namespace Database;
using namespace System;

class TaskDbService : public IService {
public:
    TaskDbService();

    ~TaskDbService() override;

    bool initialize();

    bool unInitialize();

    bool executeSql(const String &sql);

private:
    SqlConnection *connection() const;

private:
    static void createSqlFile(const String &fileName, const String &sql);

private:
    SqlConnection *_connection;
};

#endif //TSERVER_TASKDBSERVICE_H
