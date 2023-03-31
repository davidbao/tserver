//
//  WebService.cpp
//  tserver
//
//  Created by baowei on 2022/12/16.
//  Copyright (c) 2022 com. All rights reserved.
//

#include "WebService.h"
#include "IO/Path.h"
#include "IO/Directory.h"
#include "system/ServiceFactory.h"
#include "system/Application.h"
#include "microservice/HttpService.h"
#include "configuration/ConfigService.h"
#include "ExcService.h"

using namespace Data;
using namespace Microservice;

WebService::WebService() = default;

WebService::~WebService() = default;

bool WebService::initialize() {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto hs = factory->getService<IHttpRegister>();
    assert(hs);

#define BasePath "v1/catalog"
    hs->registerMapping(HttpMethod::Post, BasePath "/exchange",
                        HttpCallback<WebService>(this, &WebService::onExchange));

    // register web server.
    auto cs = factory->getService<IConfigService>();
    assert(cs);
    bool enable = true;
    cs->getProperty("summer.web.enable", enable);
    if (enable) {
        String bundlePath = getBundlePath();
        hs->registerWebPath(bundlePath);
    }

    return SsoService::initialize();
}

bool WebService::unInitialize() {
    SsoService::unInitialize();

    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);

    auto hs = factory->getService<IHttpRegister>();
    assert(hs);
    hs->removeMapping(this);

    return true;
}

String WebService::getBundlePath() {
    String bundlePath;
    const String appPath = Path::getAppPath();
    bundlePath = Path::combine(appPath, www_bundle_str);
    if (Directory::exists(bundlePath)) {
    } else {
        Application *app = Application::instance();
        assert(app);
        bundlePath = Path::combine(app->rootPath(), www_bundle_str);
        if (!Directory::exists(bundlePath)) {
            Directory::createDirectory(bundlePath);
        }
    }
    return bundlePath;
}

HttpStatus WebService::onExchange(const HttpRequest &request, HttpResponse &response) {
    JsonNode result;
    auto content = dynamic_cast<const HttpStringContent *>(request.content);
    JsonNode root;
    if (content != nullptr && JsonNode::parse(content->value(), root)) {
//        {
//            "label": [
//            {
//                "name": "n1",
//                    "tags": [
//                    "tag1",
//                    "tag2",
//                    "tag3"
//                ]
//            },
//            {
//                "name": "n2",
//                "tags": [
//                    "tag1",
//                    "tag2",
//                    "tag3",
//                    "tag4"
//                ]
//            }
//            ],
//            "table": [
//            {
//                "name": "t1",
//                "condition": {
//                "page": 1,
//                    "pageSize": 100,
//                    "timeFrom": "2022-01-02 00:00:00",
//                    "timeTo": "2022-01-02 23:59:59",
//                    "interval": "01:00:00"
//                }
//            }
//            ]
//        }

        ServiceFactory *factory = ServiceFactory::instance();
        assert(factory);
        auto es = factory->getService<ExcService>();
        assert(es);

        JsonNode dataNode("data");

        // process label node.
        JsonNode labelNode;
        FetchResult lResult = es->getLabelValues(root["label"], labelNode);
        if (lResult == FetchResult::Succeed) {
            dataNode.add(labelNode);
        }

        // process table node.
        JsonNode tableNode;
        FetchResult tResult = es->getTableValues(root["table"], tableNode);
        if (tResult == FetchResult::Succeed) {
            dataNode.add(tableNode);
        }

        if (lResult == FetchResult::NodeNotFound && tResult == FetchResult::NodeNotFound) {
            result.addRange(HttpCode::at(HttpCode::JsonParseError));
        } else {
            result.add(JsonNode("code", (int) HttpCode::Ok));
            result.add(dataNode);
        }
    } else {
        result.addRange(HttpCode::at(HttpCode::JsonParseError));
    }
    response.setContent(result);

    return HttpStatus::HttpOk;
}
