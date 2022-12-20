//
//  AdminService.cpp
//  tserver
//
//  Created by baowei on 2022/12/16.
//  Copyright © 2022 com. All rights reserved.
//

#include "AdminService.h"
#include "IO/Directory.h"
#include "system/ServiceFactory.h"
#include "system/Application.h"

AdminService::AdminService() {
}

AdminService::~AdminService() {
}

bool AdminService::initialize() {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    IHttpRegister *hs = factory->getService<IHttpRegister>();
    assert(hs);

#define BasePath "v1/admin"
    hs->registerMapping(HttpMethod::Get, BasePath "/home", HttpCallback<AdminService>(this, &AdminService::onHome));

    return true;
}

bool AdminService::unInitialize() {
    ServiceFactory* factory = ServiceFactory::instance();
    assert(factory);

    IHttpRegister* hs = factory->getService<IHttpRegister>();
    assert(hs);
    hs->removeMapping(this);

    return true;
}

HttpStatus AdminService::onHome(const HttpRequest& request, HttpResponse& response) {
    return HttpStatus::HttpOk;
}
