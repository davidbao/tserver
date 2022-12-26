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

    HttpStatus onAddTimingTask(const HttpRequest &request, HttpResponse &response);

    HttpStatus onRemoveTimingTask(const HttpRequest &request, HttpResponse &response);

    HttpStatus onAddLabel(const HttpRequest &request, HttpResponse &response);

    HttpStatus onRemoveLabel(const HttpRequest &request, HttpResponse &response);

    HttpStatus onAddTable(const HttpRequest &request, HttpResponse &response);

    HttpStatus onRemoveTable(const HttpRequest &request, HttpResponse &response);
};


#endif //TSERVER_ADMINSERVICE_H
