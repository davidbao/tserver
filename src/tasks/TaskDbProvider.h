//
//  TaskDbProvider.h
//  tserver
//
//  Created by baowei on 2022/12/16.
//  Copyright (c) 2022 com. All rights reserved.
//

#ifndef TSERVER_TASKPDBROVIDER_H
#define TSERVER_TASKPDBROVIDER_H

#include "system/ServiceFactory.h"
#include "database/SqlSelectFilter.h"
#include "database/DbClient.h"
#include "TaskContext.h"
#include <vector>

using namespace Database;
using namespace System;

class TaskDbProvider : public ITaskProvider {
public:
    explicit TaskDbProvider(const DbSource *ds);

    ~TaskDbProvider() override;

    bool getValue(const Table &table, DataTable &t) override;

private:
    bool getValue(const Columns &columns, DataTable &t);

private:
    DbClient *_dbClient;
};

#endif //TSERVER_TASKPDBROVIDER_H
