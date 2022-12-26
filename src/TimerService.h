//
//  TimerService.h
//  tserver
//
//  Created by baowei on 2022/12/16.
//  Copyright © 2022 com. All rights reserved.
//

#ifndef TSERVER_TIMERSERVICE_H
#define TSERVER_TIMERSERVICE_H

#include "system/ServiceFactory.h"
#include "data/PList.h"
#include "thread/TaskTimer.h"

using namespace Data;

class TimerService : public IService {
public:
    class Task {
    public:
        String app;
        String param;

        Task(const String &path);

        virtual ~Task();

        virtual bool isTimeUp() = 0;

        void invoke();

    private:
        String _path;
    };

    typedef PList<Task> Tasks;

    class CycleTask : public Task {
    public:
        TimeSpan interval;

        CycleTask(const String &path);

        ~CycleTask() override;

        bool isTimeUp() override;

    private:
        uint32_t _tick;
    };

    class TimeTask : public Task {
    public:
        DateTime time;
        String repeatType;
        String repeatValue;

        TimeTask(const String &path);

        ~TimeTask() override;

        bool isTimeUp() override;

    private:
        static bool parseWeeks(const String &value, Vector<DayOfWeek> &weeks);

        static bool parseMonths(const String &value, Vector<int> &months);

        static bool parseQuarters(const String &value, Vector<int> &quarters);
    };

    TimerService();

    ~TimerService() override;

    bool initialize();

    bool unInitialize();

private:
    void initTasks();

    void taskTimeUp();

private:
    Tasks _tasks;

    Timer *_timer;
};


#endif //TSERVER_TIMERSERVICE_H
