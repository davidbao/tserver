//
//  TimerService.cpp
//  tserver
//
//  Created by baowei on 2023/4/7.
//  Copyright (c) 2023 com. All rights reserved.
//

#include "TimerService.h"

TimerService::TimerService() : _timer(nullptr), _storage(nullptr) {
}

TimerService::~TimerService() {
    delete _timer;
    _timer = nullptr;
}

bool TimerService::initialize(ITaskStorage *storage) {
    _storage = storage;

    if (_timer == nullptr) {
        static const TimeSpan interval = TimeSpan::fromMilliseconds(500);
        _timer = new Timer("task.timer", interval, interval, &TimerService::taskTimeUp, this);
    }
    return true;
}

bool TimerService::unInitialize() {
    delete _timer;
    _timer = nullptr;

    return true;
}

void TimerService::taskTimeUp() {
    if (_storage != nullptr) {
        for (int i = 0; i < MaxTaskCount; i++) {
            Crontab crontab;
            if (_storage->getTask(i, crontab)) {
                if (crontab.isTimeUp()) {
                    crontab.execute();
                }
            }
        }
    }
}