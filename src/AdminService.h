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
    // exchange
    HttpStatus onGetExchangeType(const HttpRequest &request, HttpResponse &response);

    HttpStatus onSetExchangeType(const HttpRequest &request, HttpResponse &response);

    // web
    HttpStatus onUploadWebBundle(const HttpRequest &request, HttpResponse &response);

    // database
    HttpStatus onModifyDatabase(const HttpRequest &request, HttpResponse &response);

    // task
    bool onGetTaskList(const HttpRequest &request, const SqlSelectFilter &filter, DataTable &table);

    HttpStatus onGetTask(const HttpRequest &request, HttpResponse &response);

    HttpStatus onAddTask(const HttpRequest &request, HttpResponse &response);

    HttpStatus onAddTaskApp(const HttpRequest &request, HttpResponse &response);

    HttpStatus onRemoveTask(const HttpRequest &request, HttpResponse &response);

    HttpStatus onUpdateTask(const HttpRequest &request, HttpResponse &response);

    // label
    bool onGetLabelList(const HttpRequest &request, const SqlSelectFilter &filter, DataTable &table);

    HttpStatus onGetLabel(const HttpRequest &request, HttpResponse &response);

    HttpStatus onAddLabel(const HttpRequest &request, HttpResponse &response);

    HttpStatus onRemoveLabel(const HttpRequest &request, HttpResponse &response);

    HttpStatus onUpdateLabel(const HttpRequest &request, HttpResponse &response);

    // table
    bool onGetTableList(const HttpRequest &request, const SqlSelectFilter &filter, DataTable &table);

    HttpStatus onGetTable(const HttpRequest &request, HttpResponse &response);

    HttpStatus onAddTable(const HttpRequest &request, HttpResponse &response);

    HttpStatus onRemoveTable(const HttpRequest &request, HttpResponse &response);

    HttpStatus onUpdateTable(const HttpRequest &request, HttpResponse &response);

private:
    typedef bool (*Method)(const StringMap &, StringMap &);

    HttpStatus onAction(const HttpRequest &request, HttpResponse &response, Method method);

    bool uploadWebBundle(const StringMap &request, StringMap &response);
};


#endif //TSERVER_ADMINSERVICE_H
