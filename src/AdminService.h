//
//  AdminService.h
//  tserver
//
//  Created by baowei on 2022/12/16.
//  Copyright © 2022 com. All rights reserved.
//

#ifndef TSERVER_ADMINSERVICE_H
#define TSERVER_ADMINSERVICE_H

#include "microservice/SsoService.h"

using namespace Microservice;

class AdminService {
public:
    AdminService();

    ~AdminService();

    bool initialize();

    bool unInitialize();

private:
    HttpStatus onModifyDatabase(const HttpRequest &request, HttpResponse &response);

    // task
    bool onGetTask(const HttpRequest &request, const SqlSelectFilter &filter, DataTable &table);

    HttpStatus onAddTask(const HttpRequest &request, HttpResponse &response);

    HttpStatus onAddTaskApp(const HttpRequest &request, HttpResponse &response);

    HttpStatus onRemoveTask(const HttpRequest &request, HttpResponse &response);

    HttpStatus onUpdateTask(const HttpRequest &request, HttpResponse &response);

    // label
    bool onGetLabel(const HttpRequest &request, const SqlSelectFilter &filter, DataTable &table);

    HttpStatus onAddLabel(const HttpRequest &request, HttpResponse &response);

    HttpStatus onRemoveLabel(const HttpRequest &request, HttpResponse &response);

    HttpStatus onUpdateLabel(const HttpRequest &request, HttpResponse &response);

    // table
    bool onGetTable(const HttpRequest &request, const SqlSelectFilter &filter, DataTable &table);

    HttpStatus onAddTable(const HttpRequest &request, HttpResponse &response);

    HttpStatus onRemoveTable(const HttpRequest &request, HttpResponse &response);

    HttpStatus onUpdateTable(const HttpRequest &request, HttpResponse &response);

    typedef bool (*Method)(const StringMap &, StringMap &);

    HttpStatus onAction(const HttpRequest &request, HttpResponse &response, Method method);
};


#endif //TSERVER_ADMINSERVICE_H
