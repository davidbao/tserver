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
    HttpStatus onHome(const HttpRequest& request, HttpResponse& response);
};


#endif //TSERVER_ADMINSERVICE_H
