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

class TaskService : public IConfigService, public ITaskProviders {
public:
    TaskService();

    ~TaskService() override;

    ITaskProvider *getProvider(const String &dsName) const override;

    bool initialize();

    bool unInitialize();

    // for HTTP.
    bool getTasks(const SqlSelectFilter &filter, DataTable &table);

    bool getTask(const StringMap &request, StringMap &response);

    bool addTask(const StringMap &request, StringMap &response);

    bool addTaskApp(const StringMap &request, StringMap &response);

    bool removeTask(const StringMap &request, StringMap &response);

    bool updateTask(const StringMap &request, StringMap &response);

private:
    void initTasks();

    void taskTimeUp();

    bool addOrUpdateTask(const StringMap &request, StringMap &response, int position = -1);

    const YmlNode::Properties &properties() const final;

    bool setProperty(const String &key, const String &value) final;

    bool updateConfigFile(const YmlNode::Properties &properties) final;

    bool loadData();

    void initDataSources();

//    bool getValue(const TaskAction &action, const Variables &vars, const Table &table, DataTable &dataTable);
//
//    void updateVars(const Task *task, Variables &vars) const;
//
//    void addRow(DataTable &dataTable, const TaskAction &action,
//                const Columns &columns, const Variable &var = Variable::Empty) const;

private:
    static String getAppPath();

    static void updateYmlProperties(const Task *task, int position, YmlNode::Properties &properties);

    static void updateYmlProperties(bool enable, int position, YmlNode::Properties &properties);

private:
#define maxTaskCount 1000
#define taskPrefix "tasks[%d]."
#define schedulePrefix "tasks[%d].schedule."
#define executionPrefix "tasks[%d].execution."

#define maxVarCount 100
#define varPrefix executionPrefix "vars[%d]."

#define maxActionCount 100
#define actionPrefix executionPrefix "actions[%d]."

#define maxDataSourceCount 1000
#define dataSourcePrefix "datasource[%d]."

#define maxTableCount 1000
#define tablePrefix executionPrefix "tables[%d]."
#define maxColumnCount 100
#define columnPrefix tablePrefix "columns[%d]."

    DataSources _dss;

    Tasks _tasks;

    YmlNode::Properties _properties;

    Timer *_timer;

    TaskDbService _dbService;
};


#endif //TSERVER_TASKSERVICE_H
