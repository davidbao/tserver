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
#include "TaskService.h"
#include "SimulatorService.h"

AdminService::AdminService() = default;

AdminService::~AdminService() = default;

bool AdminService::initialize() {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto *hs = factory->getService<IHttpRegister>();
    assert(hs);

#define BasePath "v1/api/admin"
    hs->registerMapping(HttpMethod::Post, BasePath "/modifyDatabase",
                        HttpCallback<AdminService>(this, &AdminService::onModifyDatabase));

    hs->registerQuery(HttpMethod::Post, BasePath "/getTask",
                      HttpQueryCallback<AdminService>(this, &AdminService::onGetTask));
    hs->registerMapping(HttpMethod::Post, BasePath "/addTask",
                        HttpCallback<AdminService>(this, &AdminService::onAddTask));
    hs->registerMapping(HttpMethod::Put, BasePath "/addTaskApp",
                        HttpCallback<AdminService>(this, &AdminService::onAddTaskApp));
    hs->registerMapping(HttpMethod::Post, BasePath "/removeTask",
                        HttpCallback<AdminService>(this, &AdminService::onRemoveTask));
    hs->registerMapping(HttpMethod::Post, BasePath "/updateTask",
                        HttpCallback<AdminService>(this, &AdminService::onUpdateTask));

    hs->registerQuery(HttpMethod::Post, BasePath "/getLabel",
                      HttpQueryCallback<AdminService>(this, &AdminService::onGetLabel));
    hs->registerMapping(HttpMethod::Post, BasePath "/addLabel",
                        HttpCallback<AdminService>(this, &AdminService::onAddLabel));
    hs->registerMapping(HttpMethod::Post, BasePath "/removeLabel",
                        HttpCallback<AdminService>(this, &AdminService::onRemoveLabel));
    hs->registerMapping(HttpMethod::Post, BasePath "/updateLabel",
                        HttpCallback<AdminService>(this, &AdminService::onUpdateLabel));

    hs->registerQuery(HttpMethod::Post, BasePath "/getTable",
                        HttpQueryCallback<AdminService>(this, &AdminService::onGetTable));
    hs->registerMapping(HttpMethod::Post, BasePath "/addTable",
                        HttpCallback<AdminService>(this, &AdminService::onAddTable));
    hs->registerMapping(HttpMethod::Post, BasePath "/removeTable",
                        HttpCallback<AdminService>(this, &AdminService::onRemoveTable));
    hs->registerMapping(HttpMethod::Post, BasePath "/updateTable",
                        HttpCallback<AdminService>(this, &AdminService::onUpdateTable));

    // register web server.
    static const char *admin_bundle_str = "admin.bundle";
    static const char *admin_str = "admin";
    String bundlePath;
    const String appPath = Path::getAppPath();
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

    auto *cs = factory->getService<IConfigService>();
    assert(cs);
    cs->printProperties();

    return true;
}

bool AdminService::unInitialize() {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);

    auto *hs = factory->getService<IHttpRegister>();
    assert(hs);
    hs->removeMapping(this);

    return true;
}

HttpStatus AdminService::onModifyDatabase(const HttpRequest &request, HttpResponse &response) {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto *cs = factory->getService<IConfigService>();
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

bool AdminService::onGetTask(const HttpRequest &request, const SqlSelectFilter &filter, DataTable &table) {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto *ts = factory->getService<TaskService>();
    assert(ts);

    return ts->getTasks(filter, table);
}

HttpStatus AdminService::onAddTask(const HttpRequest &request, HttpResponse &response) {
    StringMap tRequest, tResponse;
    auto method = [](const StringMap &tRequest, StringMap &tResponse) {
        ServiceFactory *factory = ServiceFactory::instance();
        assert(factory);
        auto *ts = factory->getService<TaskService>();
        assert(ts);
        return ts->addTask(tRequest, tResponse);
    };
    return onAction(request, response, method);
}

HttpStatus AdminService::onAddTaskApp(const HttpRequest &request, HttpResponse &response) {
    JsonNode result;
    StringMap tRequest, tResponse;
    tRequest.add("name", request.getPropValue("name"));
    tRequest.add("md5", request.getPropValue("md5"));
    auto *sc = dynamic_cast<HttpStreamContent *>(request.content);
    auto *fs = dynamic_cast<FileStream *>(sc != nullptr ? sc->stream() : nullptr);
    String fullFileName = fs != nullptr ? fs->fileName() : String::Empty;
    tRequest.add("fullFileName", fullFileName);

    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto *ts = factory->getService<TaskService>();
    assert(ts);

    ts->addTaskApp(tRequest, tResponse);
    result.add(JsonNode("code", tResponse["code"]));
    result.add(JsonNode("msg", tResponse["msg"]));

    response.setContent(result);
    return HttpStatus::HttpOk;
}

HttpStatus AdminService::onRemoveTask(const HttpRequest &request, HttpResponse &response) {
    StringMap tRequest, tResponse;
    auto method = [](const StringMap &tRequest, StringMap &tResponse) {
        ServiceFactory *factory = ServiceFactory::instance();
        assert(factory);
        auto *ts = factory->getService<TaskService>();
        assert(ts);
        return ts->removeTask(tRequest, tResponse);
    };
    return onAction(request, response, method);
}

HttpStatus AdminService::onUpdateTask(const HttpRequest &request, HttpResponse &response) {
    StringMap tRequest, tResponse;
    auto method = [](const StringMap &tRequest, StringMap &tResponse) {
        ServiceFactory *factory = ServiceFactory::instance();
        assert(factory);
        auto *ts = factory->getService<TaskService>();
        assert(ts);
        return ts->updateTask(tRequest, tResponse);
    };
    return onAction(request, response, method);
}

bool AdminService::onGetLabel(const HttpRequest &request, const SqlSelectFilter &filter, DataTable &table) {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto *ts = factory->getService<SimulatorService>();
    assert(ts);

    return ts->getLabels(filter, table);
}

HttpStatus AdminService::onAddLabel(const HttpRequest &request, HttpResponse &response) {
    StringMap tRequest, tResponse;
    auto method = [](const StringMap &tRequest, StringMap &tResponse) {
        ServiceFactory *factory = ServiceFactory::instance();
        assert(factory);
        auto *ts = factory->getService<SimulatorService>();
        assert(ts);
        return ts->addLabel(tRequest, tResponse);
    };
    return onAction(request, response, method);
}

HttpStatus AdminService::onRemoveLabel(const HttpRequest &request, HttpResponse &response) {
    StringMap tRequest, tResponse;
    auto method = [](const StringMap &tRequest, StringMap &tResponse) {
        ServiceFactory *factory = ServiceFactory::instance();
        assert(factory);
        auto *ts = factory->getService<SimulatorService>();
        assert(ts);
        return ts->removeLabel(tRequest, tResponse);
    };
    return onAction(request, response, method);
}

HttpStatus AdminService::onUpdateLabel(const HttpRequest &request, HttpResponse &response) {
    StringMap tRequest, tResponse;
    auto method = [](const StringMap &tRequest, StringMap &tResponse) {
        ServiceFactory *factory = ServiceFactory::instance();
        assert(factory);
        auto *ts = factory->getService<SimulatorService>();
        assert(ts);
        return ts->updateLabel(tRequest, tResponse);
    };
    return onAction(request, response, method);
}

bool AdminService::onGetTable(const HttpRequest &request, const SqlSelectFilter &filter, DataTable &table) {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto *ts = factory->getService<SimulatorService>();
    assert(ts);

    return ts->getTables(filter, table);
}

HttpStatus AdminService::onAddTable(const HttpRequest &request, HttpResponse &response) {
    StringMap tRequest, tResponse;
    auto method = [](const StringMap &tRequest, StringMap &tResponse) {
        ServiceFactory *factory = ServiceFactory::instance();
        assert(factory);
        auto *ts = factory->getService<SimulatorService>();
        assert(ts);
        return ts->addTable(tRequest, tResponse);
    };
    return onAction(request, response, method);
}

HttpStatus AdminService::onRemoveTable(const HttpRequest &request, HttpResponse &response) {
    StringMap tRequest, tResponse;
    auto method = [](const StringMap &tRequest, StringMap &tResponse) {
        ServiceFactory *factory = ServiceFactory::instance();
        assert(factory);
        auto *ts = factory->getService<SimulatorService>();
        assert(ts);
        return ts->removeTable(tRequest, tResponse);
    };
    return onAction(request, response, method);
}

HttpStatus AdminService::onUpdateTable(const HttpRequest &request, HttpResponse &response) {
    StringMap tRequest, tResponse;
    auto method = [](const StringMap &tRequest, StringMap &tResponse) {
        ServiceFactory *factory = ServiceFactory::instance();
        assert(factory);
        auto *ts = factory->getService<SimulatorService>();
        assert(ts);
        return ts->updateTable(tRequest, tResponse);
    };
    return onAction(request, response, method);
}

HttpStatus AdminService::onAction(const HttpRequest &request, HttpResponse &response, Method method) {
    if (method != nullptr) {
        JsonNode node, result;
        StringMap tRequest, tResponse;
        if (JsonNode::parse(request.text(), node) && node.getAttribute(tRequest)) {
//            for (auto it = tRequest.begin(); it != tRequest.end(); ++it) {
//                const String &k = it.key();
//                const String &v = it.value();
//                printf("key: %s, value: %s\n", k.c_str(), v.c_str());
//            }
            method(tRequest, tResponse);
            result.add(JsonNode("code", tResponse["code"]));
            result.add(JsonNode("msg", tResponse["msg"]));
        } else {
            // Json string parse error.
            result.add(JsonNode("code", "511"));
            result.add(JsonNode("msg", "Json string parse error."));
        }
        response.setContent(result);
        return HttpStatus::HttpOk;
    } else {
        return HttpStatus::HttpMethodNotAllowed;
    }
}
