//
//  TaskService.h
//  tserver
//
//  Created by baowei on 2022/12/16.
//  Copyright © 2022 com. All rights reserved.
//

#ifndef TSERVER_TASKSERVICE_H
#define TSERVER_TASKSERVICE_H

#include "system/ServiceFactory.h"
#include "data/PList.h"
#include "thread/TaskTimer.h"
#include "database/DataTable.h"
#include "database/SqlSelectFilter.h"

using namespace Data;
using namespace Database;

class TaskService : public IService {
public:
    class Task {
    public:
        String name;
        String app;
        String param;

        explicit Task(const String &path);

        virtual ~Task();

        virtual bool isTimeUp() = 0;

        virtual DataRow toDataRow(const DataTable &table) const = 0;

        virtual String type() const = 0;

        void invoke();

    private:
        String _path;
    };

    typedef PList<Task> Tasks;

    class CycleTask : public Task {
    public:
        TimeSpan interval;

        explicit CycleTask(const String &path);

        ~CycleTask() override;

        bool isTimeUp() override;

        DataRow toDataRow(const DataTable &table) const override;

        String type() const override;

    private:
        uint32_t _tick;
    };

    class TimeTask : public Task {
    public:
        DateTime time;
        String repeatType;
        String repeatValue;

        explicit TimeTask(const String &path);

        ~TimeTask() override;

        bool isTimeUp() override;

        DataRow toDataRow(const DataTable &table) const override;

        String type() const override;

    public:
        static StringArray allRepeatTypes();

    private:
        static bool parseWeeks(const String &value, Vector<DayOfWeek> &weeks);

        static bool parseMonths(const String &value, Vector<int> &months);

        static bool parseQuarters(const String &value, Vector<int> &quarters);
    };

    TaskService();

    ~TaskService() override;

    bool initialize();

    bool unInitialize();

    bool getTasks(const SqlSelectFilter &filter, DataTable &table);

    bool addTask(const StringMap &request, StringMap &response);

    bool addTaskApp(const StringMap &request, StringMap &response);

    bool removeTask(const StringMap &request, StringMap &response);

    bool updateTask(const StringMap &request, StringMap &response);

private:
    void initTasks();

    void taskTimeUp();

    bool addOrUpdateTask(const StringMap &request, StringMap &response, int position);

private:
    static String getAppPath();

    static void updateYmlProperties(const Task *task, int position, YmlNode::Properties &properties);

    static void updateYmlProperties(bool enable, int position, YmlNode::Properties &properties);

private:
#define maxTaskCount 1000
#define taskPrefix "summer.timingTask.tasks[%d]."

    Tasks _tasks;

    Timer *_timer;
};


#endif //TSERVER_TASKSERVICE_H
