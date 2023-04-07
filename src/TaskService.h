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
#include "data/PList.h"
#include "thread/TaskTimer.h"
#include "database/DataTable.h"
#include "database/SqlSelectFilter.h"
#include "configuration/ConfigService.h"
#include "tasks/TaskContext.h"
#include "tasks/TaskDbService.h"

using namespace Data;
using namespace Config;
using namespace Database;

class TaskService : public IConfigService {
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
    // IConfigService
    const YmlNode::Properties &properties() const final;

    bool setProperty(const String &key, const String &value) final;

    bool updateConfigFile(const YmlNode::Properties &properties) final;

    void initTasks();

    void initTasks(const YmlNode::Properties &properties);

    void initTasks(const DataTable &table);

    void taskTimeUp();

    bool addOrUpdateTask(const StringMap &request, StringMap &response, int position = -1);

    bool loadData();

    bool saveData(const Crontabs &crontabs);

    bool saveData(const Crontab *crontab);

private:
    static void saveData(const Crontabs &crontabs, YmlNode::Properties &properties);

    static void saveData(const Crontab *crontab, size_t position, YmlNode::Properties &properties);

    static void saveData(const Crontabs &crontabs, DataTable &table);

    static void saveData(const Crontab *crontab, DataTable &table);

private:
#define maxTaskCount 1000
#define taskPrefix "tasks[%d]."
#define schedulePrefix "tasks[%d].schedule."
#define executionPrefix "tasks[%d].execution."

    Crontabs _crontabs;

    Timer *_timer;

    TaskDbService _dbService;

    DbClient *_dbClient;
    DataTable _table;

    YmlNode::Properties _properties;
};


#endif //TSERVER_TASKSERVICE_H
