//
//  TimerService.cpp
//  tserver
//
//  Created by baowei on 2023/4/7.
//  Copyright (c) 2023 com. All rights reserved.
//

#include "TimerService.h"
#include "Schedule.h"

TimerService::TimerService() : _cycleTimer(nullptr), _cronTimer(nullptr), _storage(nullptr) {
}

TimerService::~TimerService() {
    delete _cycleTimer;
    _cycleTimer = nullptr;

    delete _cronTimer;
    _cronTimer = nullptr;
}

bool TimerService::initialize(ITaskStorage *storage) {
    _storageMutex.lock();
    _storage = storage;
    _storageMutex.unlock();

    if (_cycleTimer == nullptr) {
        static const TimeSpan interval = TimeSpan::fromMilliseconds(500);
        _cycleTimer = new Timer("cycle.task.timer", interval, interval, &TimerService::cycleTimeUp, this);
    }
    if (_cronTimer == nullptr) {
        static const TimeSpan interval = TimeSpan::fromSeconds(2);
        _cronTimer = new Timer("cron.task.timer", interval, interval, &TimerService::cronTimeUp, this);
    }
    return true;
}

bool TimerService::unInitialize() {
    delete _cycleTimer;
    _cycleTimer = nullptr;

    delete _cronTimer;
    _cronTimer = nullptr;

    _storageMutex.lock();
    _storage = nullptr;
    _storageMutex.unlock();

    return true;
}

void TimerService::cycleTimeUp() {
    if (_storage != nullptr) {
        for (int i = 0; i < MaxTaskCount; i++) {
            Crontab crontab;
            if (getTask(i, crontab)) {
                auto cs = dynamic_cast<const CycleSchedule*>(crontab.schedule());
                if (cs != nullptr) {
                    if (crontab.isTimeUp()) {
                        crontab.execute();
                    }
                }
            }
        }
    }
}

void TimerService::cronTimeUp() {
    if (_storage != nullptr) {
        for (int i = 0; i < MaxTaskCount; i++) {
            Crontab crontab;
            if (getTask(i, crontab)) {
                auto cs = dynamic_cast<const CronSchedule*>(crontab.schedule());
                if (cs != nullptr) {
                    if (crontab.isTimeUp()) {
                        crontab.execute();
                    }
                }
            }
        }
    }
}

bool TimerService::getTask(int pos, Crontab &crontab) {
    Locker locker(&_storageMutex);
    return _storage != nullptr && _storage->getTask(pos, crontab);
}