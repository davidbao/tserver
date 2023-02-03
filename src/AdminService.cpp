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
#include "crypto/Md5Provider.h"
#include "configuration/ConfigService.h"
#include "TaskService.h"
#include "SimulatorService.h"
#include "DataService.h"

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
    hs->registerMapping(HttpMethod::Put, WebPath "/bundle",
                        HttpCallback<AdminService>(this, &AdminService::onUploadWebBundle));

    // database
#define DatabasePath BasePath "/db"
    hs->registerMapping(HttpMethod::Post, DatabasePath "/modify",
                        HttpCallback<AdminService>(this, &AdminService::onModifyDatabase));

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

//    auto *cs = factory->getService<IConfigService>();
//    assert(cs);
//    cs->printProperties();

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

HttpStatus AdminService::onGetExchangeType(const HttpRequest &request, HttpResponse &response) {
    StringMap tRequest, tResponse;
    auto method = [](const StringMap &tRequest, StringMap &tResponse) {
        ServiceFactory *factory = ServiceFactory::instance();
        assert(factory);
        auto *ss = factory->getService<DataService>();
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
        auto *ss = factory->getService<DataService>();
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

HttpStatus AdminService::onUploadWebBundle(const HttpRequest &request, HttpResponse &response) {
    JsonNode result;
    StringMap tRequest, tResponse;
    tRequest.add("md5", request.getPropValue("md5"));
    auto *sc = dynamic_cast<HttpStreamContent *>(request.content);
    auto *fs = dynamic_cast<FileStream *>(sc != nullptr ? sc->stream() : nullptr);
    String fullFileName = fs != nullptr ? fs->fileName() : String::Empty;
    tRequest.add("fullFileName", fullFileName);

    uploadWebBundle(tRequest, tResponse);
    int code = HttpCode::Unknown;   // unknown code.
    Int32::parse(tResponse["code"], code);
    result.add(JsonNode("code", code));
    result.add(JsonNode("msg", tResponse["msg"]));

    response.setContent(result);
    return HttpStatus::HttpOk;
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
    int code = HttpCode::Unknown;   // unknown code.
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
    auto *ss = factory->getService<SimulatorService>();
    assert(ss);

    return ss->getLabels(filter, table);
}

HttpStatus AdminService::onGetLabel(const HttpRequest &request, HttpResponse &response) {
    StringMap tRequest, tResponse;
    auto method = [](const StringMap &tRequest, StringMap &tResponse) {
        ServiceFactory *factory = ServiceFactory::instance();
        assert(factory);
        auto *ss = factory->getService<SimulatorService>();
        assert(ss);
        return ss->getLabel(tRequest, tResponse);
    };
    return onAction(request, response, method);
}

HttpStatus AdminService::onAddLabel(const HttpRequest &request, HttpResponse &response) {
    StringMap tRequest, tResponse;
    auto method = [](const StringMap &tRequest, StringMap &tResponse) {
        ServiceFactory *factory = ServiceFactory::instance();
        assert(factory);
        auto *ss = factory->getService<SimulatorService>();
        assert(ss);
        return ss->addLabel(tRequest, tResponse);
    };
    return onAction(request, response, method);
}

HttpStatus AdminService::onRemoveLabel(const HttpRequest &request, HttpResponse &response) {
    StringMap tRequest, tResponse;
    auto method = [](const StringMap &tRequest, StringMap &tResponse) {
        ServiceFactory *factory = ServiceFactory::instance();
        assert(factory);
        auto *ss = factory->getService<SimulatorService>();
        assert(ss);
        return ss->removeLabel(tRequest, tResponse);
    };
    return onAction(request, response, method);
}

HttpStatus AdminService::onUpdateLabel(const HttpRequest &request, HttpResponse &response) {
    StringMap tRequest, tResponse;
    auto method = [](const StringMap &tRequest, StringMap &tResponse) {
        ServiceFactory *factory = ServiceFactory::instance();
        assert(factory);
        auto *ss = factory->getService<SimulatorService>();
        assert(ss);
        return ss->updateLabel(tRequest, tResponse);
    };
    return onAction(request, response, method);
}

bool AdminService::onGetTableList(const HttpRequest &request, const SqlSelectFilter &filter, DataTable &table) {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto *ss = factory->getService<SimulatorService>();
    assert(ss);

    return ss->getTables(filter, table);
}

HttpStatus AdminService::onGetTable(const HttpRequest &request, HttpResponse &response) {
    StringMap tRequest, tResponse;
    auto method = [](const StringMap &tRequest, StringMap &tResponse) {
        ServiceFactory *factory = ServiceFactory::instance();
        assert(factory);
        auto *ss = factory->getService<SimulatorService>();
        assert(ss);
        return ss->getTable(tRequest, tResponse);
    };
    return onAction(request, response, method);
}

HttpStatus AdminService::onAddTable(const HttpRequest &request, HttpResponse &response) {
    StringMap tRequest, tResponse;
    auto method = [](const StringMap &tRequest, StringMap &tResponse) {
        ServiceFactory *factory = ServiceFactory::instance();
        assert(factory);
        auto *ss = factory->getService<SimulatorService>();
        assert(ss);
        return ss->addTable(tRequest, tResponse);
    };
    return onAction(request, response, method);
}

HttpStatus AdminService::onRemoveTable(const HttpRequest &request, HttpResponse &response) {
    StringMap tRequest, tResponse;
    auto method = [](const StringMap &tRequest, StringMap &tResponse) {
        ServiceFactory *factory = ServiceFactory::instance();
        assert(factory);
        auto *ss = factory->getService<SimulatorService>();
        assert(ss);
        return ss->removeTable(tRequest, tResponse);
    };
    return onAction(request, response, method);
}

HttpStatus AdminService::onUpdateTable(const HttpRequest &request, HttpResponse &response) {
    StringMap tRequest, tResponse;
    auto method = [](const StringMap &tRequest, StringMap &tResponse) {
        ServiceFactory *factory = ServiceFactory::instance();
        assert(factory);
        auto *ss = factory->getService<SimulatorService>();
        assert(ss);
        return ss->updateTable(tRequest, tResponse);
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
            tResponse.addRange(HttpCode::instance()->at(HttpCode::JsonParseError));
        }
        int code = HttpCode::Unknown;   // unknown code.
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
        response["code"] = Int32(538).toString();
        response["msg"] = "Can not find the upload file.";
        return false;
    }

    // check md5.
    if (!md5.isNullOrEmpty()) {
        String actual;
        if (Md5Provider::computeFileHash(fullFileName, actual)) {
            if (!String::equals(actual, md5, true)) {
                response["code"] = Int32(539).toString();
                response["msg"] = "Failed to verify md5.";
                return false;
            }
        }
    }

    // extract files.
    static const char *admin_bundle_str = "www.bundle";
    String bundlePath;
    const String appPath = Path::getAppPath();
    bundlePath = Path::combine(appPath, admin_bundle_str);
    if (Directory::exists(bundlePath)) {
    } else {
        Application *app = Application::instance();
        assert(app);
        bundlePath = Path::combine(app->rootPath(), admin_bundle_str);
        if (!Directory::exists(bundlePath)) {
            response["code"] = Int32(550).toString();
            response["msg"] = "Can not find www.bundle path.";
            return false;
        }
    }

    // extract or copy app, check zip first.
    Zip zip(fullFileName);
    if (zip.isValid()) {
        if (!Zip::extract(fullFileName, bundlePath)) {
            response["code"] = Int32(540).toString();
            response["msg"] = "Can not extract the zip file.";
            return false;
        }
    } else {
        // not a zip file.
        response["code"] = Int32(551).toString();
        response["msg"] = "The uploaded file is not a zip file.";
        return false;
    }

    response["code"] = "0";
    response["msg"] = String::Empty;
    return true;
}
