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
#include "configuration/ConfigService.h"

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
    hs->registerMapping(HttpMethod::Post, BasePath "/modifyDatabase", HttpCallback<AdminService>(this, &AdminService::onModifyDatabase));
    hs->registerMapping(HttpMethod::Post, BasePath "/addTask", HttpCallback<AdminService>(this, &AdminService::onAddTimingTask));
    hs->registerMapping(HttpMethod::Post, BasePath "/removeTask", HttpCallback<AdminService>(this, &AdminService::onRemoveTimingTask));
    hs->registerMapping(HttpMethod::Post, BasePath "/addLabel", HttpCallback<AdminService>(this, &AdminService::onAddLabel));
    hs->registerMapping(HttpMethod::Post, BasePath "/removeLabel", HttpCallback<AdminService>(this, &AdminService::onRemoveLabel));
    hs->registerMapping(HttpMethod::Post, BasePath "/addTable", HttpCallback<AdminService>(this, &AdminService::onAddTable));
    hs->registerMapping(HttpMethod::Post, BasePath "/removeTable", HttpCallback<AdminService>(this, &AdminService::onRemoveTable));

    // register web server.
    static const char *admin_bundle_str = "admin.bundle";
    static const char *admin_str = "admin";
    String bundlePath;
    const String appPath = Directory::getAppPath();
    bundlePath = Path::combine(appPath, admin_bundle_str);
    if (Directory::exists(bundlePath)) {
        hs->registerWebPath(admin_str, bundlePath);
    } else {
        Application *app = Application::instance();
        assert(app);
        bundlePath = Path::combine(app->rootPath(), admin_bundle_str);
        if (Directory::exists(bundlePath)) {
            hs->registerWebPath(admin_str, bundlePath);
        }
    }

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

HttpStatus AdminService::onModifyDatabase(const HttpRequest& request, HttpResponse& response) {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    IConfigService *cs = factory->getService<IConfigService>();
    assert(cs);

    JsonNode node, result;
    if (JsonNode::parse(request.text(), node)) {
        String name;
        if (!node.getAttribute("name", name)) {
            result.add(JsonNode("code", "1100"));
            result.add(JsonNode("msg", String::format("The field'%s' is incorrect.", "name")));
            response.setContent(result);
            return HttpStatus::HttpOk;
        }
    }

    return HttpStatus::HttpOk;
}

HttpStatus AdminService::onAddTimingTask(const HttpRequest &request, HttpResponse &response) {
    return HttpStatus::HttpOk;
}

HttpStatus AdminService::onRemoveTimingTask(const HttpRequest &request, HttpResponse &response) {
    return HttpStatus::HttpOk;
}

HttpStatus AdminService::onAddLabel(const HttpRequest &request, HttpResponse &response) {
    return HttpStatus::HttpOk;
}

HttpStatus AdminService::onRemoveLabel(const HttpRequest &request, HttpResponse &response) {
    return HttpStatus::HttpOk;
}

HttpStatus AdminService::onAddTable(const HttpRequest &request, HttpResponse &response) {
    return HttpStatus::HttpOk;
}

HttpStatus AdminService::onRemoveTable(const HttpRequest &request, HttpResponse &response) {
    return HttpStatus::HttpOk;
}
