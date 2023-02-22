//
//  AppStarter.cpp
//  tserver
//
//  Created by baowei on 2022/12/16.
//  Copyright (c) 2022 com. All rights reserved.
//

#include "AppStarter.h"
#include "AdminService.h"
#include "WebService.h"
#include "ExcService.h"
#include "TaskService.h"

AppStarter::AppStarter() {
    _adminService = new AdminService();
    _webService = new WebService();
    _dataService = new ExcService();
    _taskService = new TaskService();
}

AppStarter::~AppStarter() {
    delete _adminService;
    _adminService = nullptr;

    delete _webService;
    _webService = nullptr;

    delete _dataService;
    _dataService = nullptr;

    delete _taskService;
    _taskService = nullptr;
}

bool AppStarter::initialize() {
    _dataService->initialize();
    _adminService->initialize();
    _webService->initialize();
    _taskService->initialize();

    return true;
}

bool AppStarter::unInitialize() {
    _taskService->unInitialize();
    _webService->unInitialize();
    _adminService->unInitialize();
    _dataService->unInitialize();

    return true;
}
