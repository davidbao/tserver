//
//  AdminService.cpp
//  tserver
//
//  Created by baowei on 2022/12/16.
//  Copyright (c) 2022 com. All rights reserved.
//

#include "AdminService.h"
#include "IO/Directory.h"
#include "system/ServiceFactory.h"
#include "system/Application.h"
#include "crypto/Md5Provider.h"
#include "configuration/ConfigService.h"
#include "TaskService.h"
#include "ExcService.h"
#include "WebService.h"
#include "HttpErrorCode.h"

using namespace Crypto;

AdminService::AdminService() = default;

AdminService::~AdminService() = default;

bool AdminService::initialize() {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto *hs = factory->getService<IHttpRegister>();
    assert(hs);

#define BasePath "v1/admin"
    // exchange
#define ExchangePath BasePath "/exc"
    hs->registerMapping(HttpMethod::Get, ExchangePath "/type",
                        HttpCallback<AdminService>(this, &AdminService::onGetExchangeType));
    hs->registerMapping(HttpMethod::Post, ExchangePath "/type",
                        HttpCallback<AdminService>(this, &AdminService::onSetExchangeType));

    // web
#define WebPath BasePath "/web"
    hs->registerMapping(HttpMethod::Get, WebPath "/status",
                        HttpCallback<AdminService>(this, &AdminService::onWebStatus));
    hs->registerMapping(HttpMethod::Put, WebPath "/bundle",
                        HttpCallback<AdminService>(this, &AdminService::onUploadBundle));
    hs->registerMapping(HttpMethod::Post, WebPath "/bundle/enable",
                        HttpCallback<AdminService>(this, &AdminService::onEnableBundle));

//    // database
//#define DatabasePath BasePath "/db"
//    hs->registerMapping(HttpMethod::Post, DatabasePath "/modify",
//                        HttpCallback<AdminService>(this, &AdminService::onModifyDatabase));

    // task
#define TaskPath BasePath "/tt"
    hs->registerQuery(HttpMethod::Get, TaskPath "/task/list",
                      HttpQueryCallback<AdminService>(this, &AdminService::onGetTaskList));
    hs->registerMapping(HttpMethod::Get, TaskPath "/task",
                        HttpCallback<AdminService>(this, &AdminService::onGetTask));
    hs->registerMapping(HttpMethod::Put, TaskPath "/task",
                        HttpCallback<AdminService>(this, &AdminService::onAddTask));
    hs->registerMapping(HttpMethod::Put, TaskPath "/task/app",
                        HttpCallback<AdminService>(this, &AdminService::onAddTaskApp));
    hs->registerMapping(HttpMethod::Delete, TaskPath "/task",
                        HttpCallback<AdminService>(this, &AdminService::onRemoveTask));
    hs->registerMapping(HttpMethod::Post, TaskPath "/task",
                        HttpCallback<AdminService>(this, &AdminService::onUpdateTask));

#define SimulatorPath BasePath "/simulator"
    // label
    hs->registerQuery(HttpMethod::Get, SimulatorPath "/label/list",
                      HttpQueryCallback<AdminService>(this, &AdminService::onGetLabelList));
    hs->registerMapping(HttpMethod::Get, SimulatorPath "/label",
                        HttpCallback<AdminService>(this, &AdminService::onGetLabel));
    hs->registerMapping(HttpMethod::Put, SimulatorPath "/label",
                        HttpCallback<AdminService>(this, &AdminService::onAddLabel));
    hs->registerMapping(HttpMethod::Delete, SimulatorPath "/label",
                        HttpCallback<AdminService>(this, &AdminService::onRemoveLabel));
    hs->registerMapping(HttpMethod::Post, SimulatorPath "/label",
                        HttpCallback<AdminService>(this, &AdminService::onUpdateLabel));

    // table
    hs->registerQuery(HttpMethod::Get, SimulatorPath "/table/list",
                      HttpQueryCallback<AdminService>(this, &AdminService::onGetTableList));
    hs->registerMapping(HttpMethod::Get, SimulatorPath "/table",
                        HttpCallback<AdminService>(this, &AdminService::onGetTable));
    hs->registerMapping(HttpMethod::Put, SimulatorPath "/table",
                        HttpCallback<AdminService>(this, &AdminService::onAddTable));
    hs->registerMapping(HttpMethod::Delete, SimulatorPath "/table",
                        HttpCallback<AdminService>(this, &AdminService::onRemoveTable));
    hs->registerMapping(HttpMethod::Post, SimulatorPath "/table",
                        HttpCallback<AdminService>(this, &AdminService::onUpdateTable));

    // register web server
    String bundlePath = getBundlePath();
    if (Directory::exists(bundlePath)) {
        hs->registerWebPath(admin_str, bundlePath);
    }

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

String AdminService::getBundlePath() {
    String path;
    const String appPath = Path::getAppPath();
    path = Path::combine(appPath, admin_bundle_str);
    if (Directory::exists(path)) {
    } else {
        Application *app = Application::instance();
        assert(app);
        path = Path::combine(app->rootPath(), admin_bundle_str);
    }
    return path;
}

HttpStatus AdminService::onGetExchangeType(const HttpRequest &request, HttpResponse &response) {
    StringMap tRequest, tResponse;
    auto method = [](const StringMap &tRequest, StringMap &tResponse) {
        ServiceFactory *factory = ServiceFactory::instance();
        assert(factory);
        auto *ss = factory->getService<ExcService>();
        assert(ss);
        return ss->getType(tRequest, tResponse);
    };
    return onAction(request, response, method);
}

HttpStatus AdminService::onSetExchangeType(const HttpRequest &request, HttpResponse &response) {
    StringMap tRequest, tResponse;
    auto method = [](const StringMap &tRequest, StringMap &tResponse) {
        ServiceFactory *factory = ServiceFactory::instance();
        assert(factory);
        auto *ss = factory->getService<ExcService>();
        assert(ss);
        return ss->setType(tRequest, tResponse);
    };
    return onAction(request, response, method);
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

HttpStatus AdminService::onWebStatus(const HttpRequest &request, HttpResponse &response) {
    StringMap tRequest, tResponse;
    auto method = [](const StringMap &tRequest, StringMap &tResponse) {
        return getWebStatus(tRequest, tResponse);
    };
    return onAction(request, response, method);
}

HttpStatus AdminService::onUploadBundle(const HttpRequest &request, HttpResponse &response) {
    JsonNode result;
    StringMap tRequest, tResponse;
    tRequest.add("md5", request.getPropValue("md5"));
    auto *sc = dynamic_cast<HttpStreamContent *>(request.content);
    auto *fs = dynamic_cast<FileStream *>(sc != nullptr ? sc->stream() : nullptr);
    String fullFileName = fs != nullptr ? fs->fileName() : String::Empty;
    tRequest.add("fullFileName", fullFileName);

    uploadWebBundle(tRequest, tResponse);

    int code = HttpCode::Ok;
    Int32::parse(tResponse["code"], code);
    result.add(JsonNode("code", code));
    result.add(JsonNode("msg", tResponse["msg"]));

    response.setContent(result);
    return HttpStatus::HttpOk;
}

HttpStatus AdminService::onEnableBundle(const HttpRequest &request, HttpResponse &response) {
    StringMap tRequest, tResponse;
    auto method = [](const StringMap &tRequest, StringMap &tResponse) {
        return enableBundle(tRequest, tResponse);
    };
    return onAction(request, response, method);
}

bool AdminService::onGetTaskList(const HttpRequest &request, const SqlSelectFilter &filter, DataTable &table) {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto *ss = factory->getService<TaskService>();
    assert(ss);

    return ss->getTasks(filter, table);
}

HttpStatus AdminService::onGetTask(const HttpRequest &request, HttpResponse &response) {
    StringMap tRequest, tResponse;
    auto method = [](const StringMap &tRequest, StringMap &tResponse) {
        ServiceFactory *factory = ServiceFactory::instance();
        assert(factory);
        auto *ss = factory->getService<TaskService>();
        assert(ss);
        return ss->getTask(tRequest, tResponse);
    };
    return onAction(request, response, method);
}

HttpStatus AdminService::onAddTask(const HttpRequest &request, HttpResponse &response) {
    StringMap tRequest, tResponse;
    auto method = [](const StringMap &tRequest, StringMap &tResponse) {
        ServiceFactory *factory = ServiceFactory::instance();
        assert(factory);
        auto *ss = factory->getService<TaskService>();
        assert(ss);
        return ss->addTask(tRequest, tResponse);
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
    auto *ss = factory->getService<TaskService>();
    assert(ss);

    ss->addTaskApp(tRequest, tResponse);

    int code = HttpCode::Ok;
    Int32::parse(tResponse["code"], code);
    result.add(JsonNode("code", code));
    result.add(JsonNode("msg", tResponse["msg"]));

    response.setContent(result);
    return HttpStatus::HttpOk;
}

HttpStatus AdminService::onRemoveTask(const HttpRequest &request, HttpResponse &response) {
    StringMap tRequest, tResponse;
    auto method = [](const StringMap &tRequest, StringMap &tResponse) {
        ServiceFactory *factory = ServiceFactory::instance();
        assert(factory);
        auto *ss = factory->getService<TaskService>();
        assert(ss);
        return ss->removeTask(tRequest, tResponse);
    };
    return onAction(request, response, method);
}

HttpStatus AdminService::onUpdateTask(const HttpRequest &request, HttpResponse &response) {
    StringMap tRequest, tResponse;
    auto method = [](const StringMap &tRequest, StringMap &tResponse) {
        ServiceFactory *factory = ServiceFactory::instance();
        assert(factory);
        auto *ss = factory->getService<TaskService>();
        assert(ss);
        return ss->updateTask(tRequest, tResponse);
    };
    return onAction(request, response, method);
}

bool AdminService::onGetLabelList(const HttpRequest &request, const SqlSelectFilter &filter, DataTable &table) {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto es = factory->getService<ExcService>();
    assert(es);
    return es->getLabels(filter, table);
}

HttpStatus AdminService::onGetLabel(const HttpRequest &request, HttpResponse &response) {
    StringMap tRequest, tResponse;
    auto method = [](const StringMap &tRequest, StringMap &tResponse) {
        ServiceFactory *factory = ServiceFactory::instance();
        assert(factory);
        auto es = factory->getService<ExcService>();
        assert(es);
        return es->getLabel(tRequest, tResponse);
    };
    return onAction(request, response, method);
}

HttpStatus AdminService::onAddLabel(const HttpRequest &request, HttpResponse &response) {
    StringMap tRequest, tResponse;
    auto method = [](const StringMap &tRequest, StringMap &tResponse) {
        ServiceFactory *factory = ServiceFactory::instance();
        assert(factory);
        auto es = factory->getService<ExcService>();
        assert(es);
        return es->addLabel(tRequest, tResponse);
    };
    return onAction(request, response, method);
}

HttpStatus AdminService::onRemoveLabel(const HttpRequest &request, HttpResponse &response) {
    StringMap tRequest, tResponse;
    auto method = [](const StringMap &tRequest, StringMap &tResponse) {
        ServiceFactory *factory = ServiceFactory::instance();
        assert(factory);
        auto es = factory->getService<ExcService>();
        assert(es);
        return es->removeLabel(tRequest, tResponse);
    };
    return onAction(request, response, method);
}

HttpStatus AdminService::onUpdateLabel(const HttpRequest &request, HttpResponse &response) {
    StringMap tRequest, tResponse;
    auto method = [](const StringMap &tRequest, StringMap &tResponse) {
        ServiceFactory *factory = ServiceFactory::instance();
        assert(factory);
        auto es = factory->getService<ExcService>();
        assert(es);
        return es->updateLabel(tRequest, tResponse);
    };
    return onAction(request, response, method);
}

bool AdminService::onGetTableList(const HttpRequest &request, const SqlSelectFilter &filter, DataTable &table) {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto es = factory->getService<ExcService>();
    assert(es);
    return es->getTables(filter, table);
}

HttpStatus AdminService::onGetTable(const HttpRequest &request, HttpResponse &response) {
    StringMap tRequest, tResponse;
    auto method = [](const StringMap &tRequest, StringMap &tResponse) {
        ServiceFactory *factory = ServiceFactory::instance();
        assert(factory);
        auto es = factory->getService<ExcService>();
        assert(es);
        return es->getTable(tRequest, tResponse);
    };
    return onAction(request, response, method);
}

HttpStatus AdminService::onAddTable(const HttpRequest &request, HttpResponse &response) {
    StringMap tRequest, tResponse;
    auto method = [](const StringMap &tRequest, StringMap &tResponse) {
        ServiceFactory *factory = ServiceFactory::instance();
        assert(factory);
        auto es = factory->getService<ExcService>();
        assert(es);
        return es->addTable(tRequest, tResponse);
    };
    return onAction(request, response, method);
}

HttpStatus AdminService::onRemoveTable(const HttpRequest &request, HttpResponse &response) {
    StringMap tRequest, tResponse;
    auto method = [](const StringMap &tRequest, StringMap &tResponse) {
        ServiceFactory *factory = ServiceFactory::instance();
        assert(factory);
        auto es = factory->getService<ExcService>();
        assert(es);
        return es->removeTable(tRequest, tResponse);
    };
    return onAction(request, response, method);
}

HttpStatus AdminService::onUpdateTable(const HttpRequest &request, HttpResponse &response) {
    StringMap tRequest, tResponse;
    auto method = [](const StringMap &tRequest, StringMap &tResponse) {
        ServiceFactory *factory = ServiceFactory::instance();
        assert(factory);
        auto es = factory->getService<ExcService>();
        assert(es);
        return es->updateTable(tRequest, tResponse);
    };
    return onAction(request, response, method);
}

HttpStatus AdminService::onAction(const HttpRequest &request, HttpResponse &response, Method method) {
    if (method != nullptr) {
        JsonNode node, result;
        StringMap tRequest, tResponse;
        String str = !request.text().isNullOrEmpty() ? request.text() : request.toPropsStr();
        if (str.isNullOrEmpty() || (JsonNode::parse(str, node) && node.getAttribute(tRequest))) {
//            for (auto it = tRequest.begin(); it != tRequest.end(); ++it) {
//                const String &k = it.key();
//                const String &v = it.value();
//                printf("key: %s, value: %s\n", k.c_str(), v.c_str());
//            }
            method(tRequest, tResponse);
        } else {
            // Json string parse error.
            tResponse.addRange(HttpCode::at(HttpCode::JsonParseError));
        }

        int code = HttpCode::Ok;
        Int32::parse(tResponse["code"], code);
        result.add(JsonNode("code", code));
        if (tResponse.contains("msg")) {
            result.add(JsonNode("msg", tResponse["msg"]));
        }
        if (tResponse.contains("data")) {
            result.add(JsonNode("data", tResponse["data"]));
        }

        response.setContent(result);
        return HttpStatus::HttpOk;
    } else {
        return HttpStatus::HttpMethodNotAllowed;
    }
}

bool AdminService::uploadWebBundle(const StringMap &request, StringMap &response) {
    String md5 = request["md5"];
    String fullFileName = request["fullFileName"];

    // file exist?
    if (!File::exists(fullFileName)) {
        response.addRange(HttpCode::at(CannotFindFile));
        return false;
    }

    // check md5.
    if (!md5.isNullOrEmpty()) {
        String actual;
        if (Md5Provider::computeFileHash(fullFileName, actual)) {
            if (!String::equals(actual, md5, true)) {
                response.addRange(HttpCode::at(FailedToVerifyMd5));
                return false;
            }
        }
    }

    // extract files.
    String bundlePath = WebService::getBundlePath();
    if (!Directory::exists(bundlePath)) {
        response.addRange(HttpCode::at(CannotFindWWWBundlePath));
        return false;
    }

    // extract or copy app, check zip first.
    Zip zip(fullFileName);
    if (zip.isValid()) {
        if (!Zip::extract(fullFileName, bundlePath)) {
            response.addRange(HttpCode::at(CannotExtractZip));
            return false;
        }
    } else {
        // not a zip file.
        response.addRange(HttpCode::at(NotAZipFile));
        return false;
    }

    response.addRange(HttpCode::okCode());
    return true;
}

bool AdminService::getWebStatus(const StringMap &request, StringMap &response) {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto *cs = factory->getService<IConfigService>();
    assert(cs);
    bool enable = true;
    cs->getProperty("summer.web.enable", enable);
    int port = 0;
    cs->getProperty("server.port", port);
    JsonNode node;
    node.add(JsonNode("enable", enable));
    node.add(JsonNode("scheme", cs->getProperty("server.scheme")));
    node.add(JsonNode("port", port));

    response["code"] = "0";
    response["data"] = node.toString();
    return true;
}

bool AdminService::enableBundle(const StringMap &request, StringMap &response) {
    bool enable;
    if (Boolean::parse(request["enable"], enable)) {
        ServiceFactory *factory = ServiceFactory::instance();
        assert(factory);
        auto *hs = factory->getService<IHttpRegister>();
        assert(hs);
        auto *cs = factory->getService<IConfigService>();
        assert(cs);

        // unregister web server.
        String bundlePath = WebService::getBundlePath();
        if (Directory::exists(bundlePath)) {
            if (enable) {
                hs->registerWebPath(bundlePath);
            } else {
                hs->unregisterWebPath(bundlePath);
            }
        }

        // update profile yml file.
        YmlNode::Properties properties;
        properties.add("summer.web.enable", enable);
        if (!cs->updateConfigFile(properties)) {
            // Failed to save config file.
            response.addRange(HttpCode::at(FailedToSave));
            return false;
        }

        response.addRange(HttpCode::okCode());
        return true;
    } else {
        response.addRange(HttpCode::at(HttpCode::ParameterIncorrect));
        return false;
    }
}
