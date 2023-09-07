//
//  TaskContext.h
//  tserver
//
//  Created by baowei on 2022/12/17.
//  Copyright (c) 2022 com. All rights reserved.
//

#ifndef TSERVER_TASKCONTEXT_H
#define TSERVER_TASKCONTEXT_H

#include "Crontab.h"

// for application.yml
#define TaskPrefix "summer.task."
#define TaskDbPrefix TaskPrefix "database."

// for yml file.
#define MaxTaskCount 1000
#define TasksPrefix "tasks[%d]."
#define SchedulePrefix TasksPrefix "schedule."
#define ExecutionPrefix TasksPrefix "execution."

// for database.
#define TaskTableName "task"

class ITaskStorage {
public:
    ITaskStorage() = default;

    virtual ~ITaskStorage() = default;

    virtual bool load() = 0;

    virtual bool getTask(const String &name, Crontab &crontab) = 0;

    virtual bool getTask(int pos, Crontab &crontab) = 0;

    virtual bool getTasks(const SqlSelectFilter &filter, DataTable &table) = 0;

    virtual bool addTask(const StringMap &request, StringMap &response) = 0;

    virtual bool updateTask(const StringMap &request, StringMap &response) = 0;

    virtual bool removeTask(const StringMap &request, StringMap &response) = 0;
};

class ITaskCache : public IService {
public:
    ITaskCache() = default;

    virtual String getValue(const String &key) = 0;

    virtual bool setValue(const String &key, const String &value) = 0;
};

#endif //TSERVER_TASKCONTEXT_H
