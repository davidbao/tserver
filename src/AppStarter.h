//
//  AppStarter.h
//  tserver
//
//  Created by baowei on 2022/12/16.
//  Copyright © 2022 com. All rights reserved.
//

#ifndef TSERVER_APPSTARTER_H
#define TSERVER_APPSTARTER_H

class AdminService;
class CommService;
class DataService;
class TaskService;

class AppStarter {
public:
    AppStarter();

    ~AppStarter();

    bool initialize();

    bool unInitialize();

private:
    AdminService* _adminService;
    CommService* _commService;
    DataService* _dataService;
    TaskService* _timerService;
};

#endif //TSERVER_APPSTARTER_H
