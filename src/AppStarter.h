//
//  AppStarter.h
//  tserver
//
//  Created by baowei on 2022/12/16.
//  Copyright (c) 2022 com. All rights reserved.
//

#ifndef TSERVER_APPSTARTER_H
#define TSERVER_APPSTARTER_H

class AdminService;
class WebService;
class ExcService;
class TaskService;

class AppStarter {
public:
    AppStarter();

    ~AppStarter();

    bool initialize();

    bool unInitialize();

private:
    AdminService* _adminService;
    WebService* _webService;
    ExcService* _dataService;
    TaskService* _taskService;
};

#endif //TSERVER_APPSTARTER_H
