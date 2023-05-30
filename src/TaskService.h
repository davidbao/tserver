//
//  TaskService.h
//  tserver
//
//  Created by baowei on 2022/12/16.
//  Copyright (c) 2022 com. All rights reserved.
//

#ifndef TSERVER_TASKSERVICE_H
#define TSERVER_TASKSERVICE_H

#include "system/ServiceFactory.h"
#include "database/DataTable.h"
#include "database/SqlSelectFilter.h"
#include "configuration/ConfigService.h"
#include "tasks/TaskContext.h"
#include "tasks/TaskDbService.h"
#include "tasks/TimerService.h"

using namespace Data;
using namespace Config;
using namespace Database;

class TaskService : public IService {
public:
    TaskService();

    ~TaskService() override;

    bool initialize();

    bool unInitialize();

    // for HTTP.
    bool getTasks(const SqlSelectFilter &filter, DataTable &table);

    bool getTask(const StringMap &request, StringMap &response);

    bool addTask(const StringMap &request, StringMap &response);

    bool addTaskFile(const StringMap &request, StringMap &response);

    bool removeTask(const StringMap &request, StringMap &response);

    bool updateTask(const StringMap &request, StringMap &response);

private:
    bool loadData();

private:
    ITaskStorage *_storage;

    ITaskCache *_cache;

    TimerService _timerService;

    TaskDbService _dbService;
};


#endif //TSERVER_TASKSERVICE_H
