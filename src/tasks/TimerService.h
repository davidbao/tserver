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
    void taskTimeUp();

private:
    Timer *_timer;

    ITaskStorage *_storage;
};


#endif //TSERVER_TIMERSERVICE_H
