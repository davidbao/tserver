//
//  TaskStorage.h
//  tserver
//
//  Created by baowei on 2023/4/7.
//  Copyright (c) 2023 com. All rights reserved.
//

#ifndef TSERVER_TASKSTORAGE_H
#define TSERVER_TASKSTORAGE_H

#include "TaskContext.h"
#include "database/SqlConnection.h"

using namespace Database;

class TaskFile : public ITaskStorage {
public:
    TaskFile() = default;

    bool load() override;

    bool getTask(const String &name, Crontab &crontab) override;

    bool getTask(int pos, Crontab &crontab) override;

    bool getTasks(const SqlSelectFilter &filter, DataTable &table) override;

    bool addTask(const StringMap &request, StringMap &response) override;

    bool updateTask(const StringMap &request, StringMap &response) override;

    bool removeTask(const StringMap &request, StringMap &response) override;

private:
    String fileName() const;

    void rescan();

    bool saveFile(const YmlNode::Properties &properties);

private:
    YmlNode::Properties _properties;

    DateTime _modifyTime;
};

class TaskDatabase : public ITaskStorage {
public:
    TaskDatabase();

    ~TaskDatabase() override;

    bool load() override;

    bool getTask(const String &name, Crontab &crontab) override;

    bool getTask(int pos, Crontab &crontab) override;

    bool getTasks(const SqlSelectFilter &filter, DataTable &table) override;

    bool addTask(const StringMap &request, StringMap &response) override;

    bool updateTask(const StringMap &request, StringMap &response) override;

    bool removeTask(const StringMap &request, StringMap &response) override;

private:
    SqlConnection *connection();

    bool getTaskId(const String &name, uint64_t &id);

    bool containsTask(const String &name);

private:
    static String getSchema();

    static String getTableName(const String &tableName);

private:
    SqlConnection *_connection;
};


#endif //TSERVER_TASKSTORAGE_H
