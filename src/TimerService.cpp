//
//  TimerService.cpp
//  tserver
//
//  Created by baowei on 2022/12/16.
//  Copyright © 2022 com. All rights reserved.
//

#include "configuration/ConfigService.h"
#include "IO/Path.h"
#include "IO/File.h"
#include "IO/Directory.h"
#include "diag/Trace.h"
#include "thread/Process.h"
#include "system/Application.h"
#include "thread/TickTimeout.h"
#include "TimerService.h"

using namespace Config;

TimerService::Task::Task(const String &path) : _path(path) {
}

TimerService::Task::~Task() {
}

void TimerService::Task::invoke() {
    String appFileName = Path::combine(_path, app);
    if (File::exists(appFileName)) {
        Process process;
        process.setRedirectStdout(true);
        process.setWaitingTimeout(3000);
        Process::start(appFileName, param, &process);
        Trace::info(process.stdoutStr());
    }
}

TimerService::CycleTask::CycleTask(const String &path) : Task(path), _tick(0) {
}

TimerService::CycleTask::~CycleTask() {
}

bool TimerService::CycleTask::isTimeUp() {
    if (TickTimeout::isTimeout(_tick, interval)) {
        _tick = TickTimeout::getCurrentTickCount();
        return true;
    }
    return false;
}

TimerService::TimeTask::TimeTask(const String &path) : Task(path) {
}

TimerService::TimeTask::~TimeTask() {
}

bool TimerService::TimeTask::isTimeUp() {
    static uint32_t current = 0;
    if (TickTimeout::isTimeout(current, TimeSpan::fromMinutes(1))) {
        current = TickTimeout::getCurrentTickCount();

        if (String::equals(repeatType, "week", true)) {
            Vector<DayOfWeek> weeks;
            if (parseWeeks(repeatValue, weeks)) {
                DateTime now = DateTime::now();
                if (weeks.contains(now.dayOfWeek()) &&
                    time.hour() == now.hour() && time.minute() == now.minute()) {
                    return true;
                }
            }
        } else if (String::equals(repeatType, "day", true)) {
            DateTime now = DateTime::now();
            if (time.hour() == now.hour() && time.minute() == now.minute()) {
                return true;
            }
        } else if (String::equals(repeatType, "month", true)) {
            Vector<int> months;
            if (parseMonths(repeatValue, months)) {
                DateTime now = DateTime::now();
                if (months.contains(now.month()) &&
                    time.day() == now.day() && time.hour() == now.hour() && time.minute() == now.minute()) {
                    return true;
                }
            }
        } else if (String::equals(repeatType, "quarter", true)) {
            Vector<int> quarters;
            if (parseQuarters(repeatValue, quarters)) {
                DateTime now = DateTime::now();
                if (quarters.contains(now.month() / 4 + 1) &&
                    time.day() == now.day() && time.hour() == now.hour() && time.minute() == now.minute()) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool TimerService::TimeTask::parseWeeks(const String &value, Vector<DayOfWeek> &weeks) {
    if (value.find('-') > 0) {
        StringArray texts;
        StringArray::parse(value, texts, '-');
        if (texts.count() == 2) {
            int start, end;
            if (Int32::parse(texts[0], start) &&
                Int32::parse(texts[1], end) &&
                start >= 0 && start <= 6 && end >= 0 && end <= 6 &&
                end >= start) {
                for (int week = start; week <= end; ++week) {
                    weeks.add((DayOfWeek) week);
                }
                return true;
            }
        }
    } else {
        StringArray texts;
        StringArray::parse(value, texts, ',');
        for (size_t i = 0; i < texts.count(); ++i) {
            int week;
            if (Int32::parse(texts[i], week) &&
                week >= 0 && week <= 6) {
                weeks.add((DayOfWeek) week);
            }
        }
        return weeks.count() > 0;
    }
    return false;
}

bool TimerService::TimeTask::parseMonths(const String &value, Vector<int> &months) {
    if (value.find('-') > 0) {
        StringArray texts;
        StringArray::parse(value, texts, '-');
        if (texts.count() == 2) {
            int start, end;
            if (Int32::parse(texts[0], start) &&
                Int32::parse(texts[1], end) &&
                start >= 1 && start <= 12 && end >= 1 && end <= 12 &&
                end >= start) {
                for (int month = start; month <= end; ++month) {
                    months.add(month);
                }
                return true;
            }
        }
    } else {
        StringArray texts;
        StringArray::parse(value, texts, ',');
        for (size_t i = 0; i < texts.count(); ++i) {
            int month;
            if (Int32::parse(texts[i], month) &&
                month >= 1 && month <= 12) {
                months.add(month);
            }
        }
        return months.count() > 0;
    }
    return false;
}

bool TimerService::TimeTask::parseQuarters(const String &value, Vector<int> &quarters) {
    if (value.find('-') > 0) {
        StringArray texts;
        StringArray::parse(value, texts, '-');
        if (texts.count() == 2) {
            int start, end;
            if (Int32::parse(texts[0], start) &&
                Int32::parse(texts[1], end) &&
                start >= 1 && start <= 4 && end >= 1 && end <= 4 &&
                end >= start) {
                for (int month = start; month <= end; ++month) {
                    quarters.add(month);
                }
                return true;
            }
        }
    } else {
        StringArray texts;
        StringArray::parse(value, texts, ',');
        for (size_t i = 0; i < texts.count(); ++i) {
            int month;
            if (Int32::parse(texts[i], month) &&
                month >= 1 && month <= 4) {
                quarters.add(month);
            }
        }
        return quarters.count() > 0;
    }
    return false;
}

TimerService::TimerService() : _timer(nullptr) {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    factory->addService<TimerService>(this);
}

TimerService::~TimerService() {
    delete _timer;
    _timer = nullptr;

    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    factory->removeService<TimerService>();
}

bool TimerService::initialize() {
    initTasks();

    static const TimeSpan interval = TimeSpan::fromSeconds(1);
    _timer = new Timer("timingTask.timer",
                       TimerCallback<TimerService>(this, &TimerService::taskTimeUp),
                       interval, interval);

    return true;
}

bool TimerService::unInitialize() {
    return true;
}

void TimerService::initTasks() {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    IConfigService *cs = factory->getService<IConfigService>();
    assert(cs);

    String subPath;
    cs->getProperty("summer.timingTask.path", subPath);
    const String appPath = Directory::getAppPath();
    String path = Path::combine(appPath, subPath);
    if (!Directory::exists(path)) {
        Application *app = Application::instance();
        assert(app);
        path = Path::combine(app->rootPath(), subPath);
        if (!Directory::exists(path)) {
            Trace::error("Can not find the task apps.");
            return;
        }
    }

    Locker locker(&_tasks);

    _tasks.clear();

#define maxTaskCount 1000
#define taskPrefix "summer.timingTask.tasks[%d]."
    for (int i = 0; i < maxTaskCount; i++) {
        String type;
        if (!cs->getProperty(String::format(taskPrefix "type", i), type)) {
            break;
        }

        String app, param;
        cs->getProperty(String::format(taskPrefix "app", i), app);
        cs->getProperty(String::format(taskPrefix "param", i), param);

        if (type == "cycle") {
            TimeSpan interval;
            cs->getProperty(String::format(taskPrefix "interval", i), interval);
            if (interval != TimeSpan::Zero) {
                CycleTask *task = new CycleTask(path);
                task->app = app;
                task->param = param;
                task->interval = interval;
                _tasks.add(task);
            }
        } else if (type == "time") {
            DateTime time;
            cs->getProperty(String::format(taskPrefix "time", i), time);
            String repeatType, repeatValue;
            cs->getProperty(String::format(taskPrefix "repeat.type", i), repeatType);
            cs->getProperty(String::format(taskPrefix "repeat.value", i), repeatValue);
            if (time != DateTime::MinValue) {
                TimeTask *task = new TimeTask(path);
                task->app = app;
                task->param = param;
                task->time = time;
                task->repeatType = repeatType;
                task->repeatValue = repeatValue;
                _tasks.add(task);
            }
        }
    }
}

void TimerService::taskTimeUp() {
    Locker locker(&_tasks);
    for (size_t i = 0; i < _tasks.count(); ++i) {
        Task *task = _tasks[i];
        if (task->isTimeUp()) {
            task->invoke();
        }
    }
}
