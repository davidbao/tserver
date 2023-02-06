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

public:
    static String getBundlePath();

private:
    HttpStatus onExchange(const HttpRequest& request, HttpResponse& response);

private:
    static constexpr const char *www_bundle_str = "www.bundle";
};


#endif //TSERVER_COMMSERVICE_H
