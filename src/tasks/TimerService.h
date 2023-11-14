//
//  TimerService.h
//  tserver
//
//  Created by baowei on 2023/4/7.
//  Copyright (c) 2023 com. All rights reserved.
//


#ifndef TSERVER_TIMERSERVICE_H
#define TSERVER_TIMERSERVICE_H

#include "thread/Timer.h"
#include "TaskContext.h"

using namespace Threading;

class TimerService {
public:
    TimerService();

    ~TimerService();

    bool initialize(ITaskStorage *storage);

    bool unInitialize();

private:
    void cycleTimeUp();

    void cronTimeUp();

    bool getTask(int pos, Crontab &crontab);

private:
    Timer *_cycleTimer;
    Timer *_cronTimer;

    ITaskStorage *_storage;
    Mutex _storageMutex;
};


#endif //TSERVER_TIMERSERVICE_H
