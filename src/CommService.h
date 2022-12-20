//
//  CommService.h
//  tserver
//
//  Created by baowei on 2022/12/16.
//  Copyright © 2022 com. All rights reserved.
//

#ifndef TSERVER_COMMSERVICE_H
#define TSERVER_COMMSERVICE_H

#include "microservice/SsoService.h"

using namespace Microservice;

class CommService : public SsoService {
public:
    CommService();

    ~CommService() override;

    bool initialize() override;

    bool unInitialize() override;

private:
    HttpStatus onExchange(const HttpRequest& request, HttpResponse& response);
};


#endif //TSERVER_COMMSERVICE_H
