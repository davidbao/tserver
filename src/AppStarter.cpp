//
//  AppStarter.cpp
//  tserver
//
//  Created by baowei on 2022/12/16.
//  Copyright © 2022 com. All rights reserved.
//

#include "AppStarter.h"
#include "AdminService.h"
#include "CommService.h"
#include "DataService.h"
#include "TimerService.h"

AppStarter::AppStarter() {
    _adminService = new AdminService();
    _commService = new CommService();
    _dataService = new DataService();
    _timerService = new TimerService();
}

AppStarter::~AppStarter() {
    delete _adminService;
    _adminService = nullptr;

    delete _commService;
    _commService = nullptr;

    delete _dataService;
    _dataService = nullptr;

    delete _timerService;
    _timerService = nullptr;
}

bool AppStarter::initialize() {
    _dataService->initialize();
    _adminService->initialize();
    _commService->initialize();
    _timerService->initialize();

    return true;
}

bool AppStarter::unInitialize() {
    _timerService->unInitialize();
    _commService->unInitialize();
    _adminService->unInitialize();
    _dataService->unInitialize();

    return true;
}
