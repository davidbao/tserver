//
//  WebService.h
//  tserver
//
//  Created by baowei on 2022/12/16.
//  Copyright (c) 2022 com. All rights reserved.
//

#ifndef TSERVER_WEBSERVICE_H
#define TSERVER_WEBSERVICE_H

#include "microservice/SsoService.h"

using namespace Microservice;

class WebService : public SsoService {
public:
    WebService();

    ~WebService() override;

    bool initialize() override;

    bool unInitialize() override;

public:
    static String getBundlePath();

private:
    HttpStatus onExchange(const HttpRequest& request, HttpResponse& response);

private:
    static constexpr const char *www_bundle_str = "www.bundle";
};


#endif //TSERVER_WEBSERVICE_H
