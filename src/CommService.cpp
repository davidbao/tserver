//
//  CommService.cpp
//  tserver
//
//  Created by baowei on 2022/12/16.
//  Copyright © 2022 com. All rights reserved.
//

#include "CommService.h"
#include "IO/Path.h"
#include "IO/Directory.h"
#include "system/ServiceFactory.h"
#include "system/Application.h"
#include "microservice/HttpService.h"
#include "configuration/ConfigService.h"
#include "DataService.h"

using namespace Data;
using namespace Microservice;

CommService::CommService() = default;

CommService::~CommService() = default;

bool CommService::initialize() {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto *hs = factory->getService<IHttpRegister>();
    assert(hs);

#define BasePath "v1/catalog"
    hs->registerMapping(HttpMethod::Post, BasePath "/exchange",
                        HttpCallback<CommService>(this, &CommService::onExchange));

    // register web server.
    auto *cs = factory->getService<IConfigService>();
    assert(cs);
    bool enable = true;
    cs->getProperty("summer.web.enable", enable);
    if (enable) {
        String bundlePath = getBundlePath();
        hs->registerWebPath(bundlePath);
    }

    return SsoService::initialize();
}

bool CommService::unInitialize() {
    SsoService::unInitialize();

    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);

    auto *hs = factory->getService<IHttpRegister>();
    assert(hs);
    hs->removeMapping(this);

    return true;
}

String CommService::getBundlePath() {
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

HttpStatus CommService::onExchange(const HttpRequest &request, HttpResponse &response) {
    JsonNode result;
    const HttpStringContent *content = dynamic_cast<const HttpStringContent *>(request.content);
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
        auto *ds = factory->getService<DataService>();
        assert(ds);

        JsonNode dataNode("data");

        JsonNode labelNode;
        FetchResult lResult = ds->getLabelValues(root["label"], labelNode);
        if (lResult == FetchResult::Succeed) {
            dataNode.add(labelNode);

            // process table node.
            JsonNode tableNode;
            FetchResult tResult = ds->getTableValues(root["table"], tableNode);
            if (tResult == FetchResult::Succeed) {
                dataNode.add(tableNode);
                result.add(JsonNode("code", 0));
                result.add(dataNode);
            } else if (tResult == FetchResult::JsonError) {
                result.addRange(HttpCode::at(HttpCode::JsonParseError));
            } else if (tResult == FetchResult::ConfigError) {
                result.addRange(HttpCode::at(CannotFindExchangeType));
            } else {
                result.addRange(HttpCode::at(HttpCode::Unknown));
            }
        } else if (lResult == FetchResult::JsonError) {
            result.addRange(HttpCode::at(HttpCode::JsonParseError));
        } else if (lResult == FetchResult::ConfigError) {
            result.addRange(HttpCode::at(CannotFindExchangeType));
        } else {
            result.addRange(HttpCode::at(HttpCode::Unknown));
        }
    } else {
        result.addRange(HttpCode::at(HttpCode::JsonParseError));
    }
    response.setContent(result);

    return HttpStatus::HttpOk;
}
