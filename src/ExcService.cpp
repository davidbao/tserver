//
//  ExcService.cpp
//  tserver
//
//  Created by baowei on 2022/12/17.
//  Copyright (c) 2022 com. All rights reserved.
//

#include "configuration/ConfigService.h"
#include "ExcService.h"
#include "exchanges/ExcSimProvider.h"
#include "exchanges/ExcDbProvider.h"
#include "HttpErrorCode.h"
#include "diag/Trace.h"

using namespace Config;
using namespace Diag;

ExcService::ExcService() : _provider(nullptr) {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    factory->addService<ExcService>(this);

    HttpRegisters::registerCodes();
}

ExcService::~ExcService() {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    factory->removeService<ExcService>();

    if (_provider != nullptr) {
        delete _provider;
        _provider = nullptr;
    }
}

bool ExcService::initialize() {
    _provider = createProvider(type());

    return true;
}

bool ExcService::unInitialize() {
    if (_provider != nullptr) {
        delete _provider;
        _provider = nullptr;
    }

    return true;
}

String ExcService::type() const {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto *cs = factory->getService<IConfigService>();
    assert(cs);

    String type;
    cs->getProperty(ExcPrefix "type", type);
    return isTypeValid(type) ? type : String::Empty;
}

bool ExcService::isTypeValid(const String &type) {
    if (String::equals(type, "simulator", true) ||
        String::equals(type, "database", true)) {
        return true;
    }
    return false;
}

IExcProvider *ExcService::createProvider(const String &type) {
    IExcProvider *provider = nullptr;
    if (String::equals(type, "simulator", true)) {
        provider = new ExcSimProvider();
    } else if (String::equals(type, "database", true)) {
        provider = new ExcDbProvider();
    }
    return provider;
}

IExcProvider *ExcService::provider() const {
    return _provider;
}

FetchResult ExcService::getLabelValues(const JsonNode &request, JsonNode &response) {
    IExcProvider *provider = this->provider();
    if (provider == nullptr) {
        return FetchResult::ConfigError;
    }

    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto *cs = factory->getService<IConfigService>();
    assert(cs);

    String responseStyle;
    cs->getProperty(ExcPrefix "response.style", responseStyle);

    if (request.name() == "label") {
        List<JsonNode> nodes;
        request.subNodes(nodes);
        if (nodes.count() > 0) {
            JsonNode lNode("label", JsonNode::TypeArray);
            for (size_t i = 0; i < nodes.count(); ++i) {
                const JsonNode &node = nodes[i];
                String name = node.getAttribute("name");
                StringArray tags;
                node.getAttribute("tags", tags);
                SqlSelectFilter filter(1, 1);
                SqlSelectFilter::parse(node["condition"].toString(), filter);
                VariantMap values;
                FetchResult result = provider->getLabelValues(name, tags, filter, values);
                JsonNode iNode("item");
                iNode.add(JsonNode("name", name));
                iNode.add(JsonNode("errorCode", (int) result));
                if (result == FetchResult::Succeed) {
                    JsonNode tagsNode("tags");
                    for (auto it = values.begin(); it != values.end(); ++it) {
                        const auto& tagName = it.key();
                        const auto& tagValue = it.value();
                        if (responseStyle == "string") {
                            tagsNode.add(JsonNode(tagName, tagValue.toString()));
                        } else {
                            tagsNode.add(JsonNode(tagName, tagValue));
                        }
                    }
                    iNode.add(tagsNode);
                }
                lNode.add(iNode);
            }
            response = lNode;

            return FetchResult::Succeed;
        } else {
            // Without labels.
            return FetchResult::NodeNotFound;
        }
    }
    return FetchResult::NodeNotFound;
}

FetchResult ExcService::getTableValues(const JsonNode &request, JsonNode &response) {
    IExcProvider *provider = this->provider();
    if (provider == nullptr) {
        return FetchResult::ConfigError;
    }

    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto *cs = factory->getService<IConfigService>();
    assert(cs);

    String responseStyle;
    cs->getProperty(ExcPrefix "response.style", responseStyle);

    if (request.name() == "table") {
        List<JsonNode> nodes;
        request.subNodes(nodes);
        if (nodes.count() > 0) {
            JsonNode tNode("table", JsonNode::TypeArray);
            for (size_t i = 0; i < nodes.count(); ++i) {
                const JsonNode &node = nodes[i];
                String name = node.getAttribute("name");
                StringArray columns;
                node.getAttribute("columns", columns);
                SqlSelectFilter filter;
                if (SqlSelectFilter::parse(node["condition"].toString(), filter)) {
                    DataTable table("rows");
                    FetchResult result = provider->getTableValues(name, columns, filter, table);
                    JsonNode iNode("item");
                    iNode.add(JsonNode("name", name));
                    iNode.add(JsonNode("errorCode", (int) result));
                    if (result == FetchResult::Succeed) {
                        iNode.add(JsonNode("pageNo", filter.page()));
                        iNode.add(JsonNode("pageSize", filter.pageSize()));
                        int pageCount = filter.pageSize() > 0 ?
                                        (table.totalCount() / filter.pageSize() + (
                                                table.totalCount() % filter.pageSize() == 0 ? 0 : 1)) : 0;
                        iNode.add(JsonNode("pageCount", pageCount));
                        iNode.add(JsonNode("totalCount", table.totalCount()));

//                        if (ignoreCase) {
//                            for (size_t j = 0; j < table.columnCount(); ++j) {
//                                auto column = table.columns()[j];
//                                if (String::equals(column.name(), tags))
//                            }
//                        }
                        JsonNode dataNode;
                        table.toJsonNode(dataNode, responseStyle == "string" ? "cs" : "cn");
                        iNode.add(dataNode);
                    }
                    tNode.add(iNode);
                }
            }
            response = tNode;

            return FetchResult::Succeed;
        } else {
            // Without tables.
            return FetchResult::NodeNotFound;
        }
    }
    return FetchResult::NodeNotFound;
}

FetchResult ExcService::execButton(const JsonNode &request, JsonNode &response) {
    IExcProvider *provider = this->provider();
    if (provider == nullptr) {
        return FetchResult::ConfigError;
    }

    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto *cs = factory->getService<IConfigService>();
    assert(cs);

    String responseStyle;
    cs->getProperty(ExcPrefix "response.style", responseStyle);

    if (request.name() == "button") {
        List<JsonNode> nodes;
        request.subNodes(nodes);
        if (nodes.count() > 0) {
            JsonNode bNode("button", JsonNode::TypeArray);
            for (size_t i = 0; i < nodes.count(); ++i) {
                const JsonNode &node = nodes[i];
                String name = node.getAttribute("name");

                auto toParameters = [](const DataTable &table, StringMap &params) {
                    for (size_t i = 0; i < table.columnCount(); ++i) {
                        const String &colName = table.columns()[i].name();
                        JsonNode valueNode(JsonNode::TypeArray);
                        for (size_t j = 0; j < table.rowCount(); ++j) {
                            const DataRow &row = table.rows()[j];
                            valueNode.add(JsonNode("item", row.cells()[i].valueStr()));
                        }
                        params.add(colName, valueNode.toString());
                    }
                };

                auto updateTimeAndUser = [](const JsonNode &node, StringMap &params) {
                    static const char *operationTimeStr = "operation_time";
                    static const char *operationUserStr = "operation_user";

                    DateTime time;
                    if (node.getAttribute("time", time)) {
                        params.add(operationTimeStr, time);
                    }
                    String user;
                    if (node.getAttribute("user", user)) {
                        params.add(operationUserStr, user);
                    }
                };

                StringMap params(true);
                VariantMap values;
                FetchResult result = FetchResult::NodeNotFound;
                if (node.hasAttribute("parameters")) {
                    node["parameters"].getAttribute(params);
                    updateTimeAndUser(node, params);
                    result = provider->execButton(name, params, values);
                } else if (node.hasAttribute("table")) {
                    DataTable table("table");
                    DataTable::parse(node.at("table"), table);
                    toParameters(table, params);
                    updateTimeAndUser(node, params);
                    result = provider->execButton(name, params, values);
                }
                JsonNode iNode("item");
                iNode.add(JsonNode("name", name));
                iNode.add(JsonNode("errorCode", (int) result));
                if (result == FetchResult::Succeed) {
                    JsonNode resultsNode("results");
                    for (auto it = values.begin(); it != values.end(); ++it) {
                        const auto& resultName = it.key();
                        const auto& resultValue = it.value();
                        if (responseStyle == "string") {
                            resultsNode.add(JsonNode(resultName, resultValue.toString()));
                        } else {
                            resultsNode.add(JsonNode(resultName, resultValue));
                        }
                    }
                    iNode.add(resultsNode);
                }
                bNode.add(iNode);
            }
            response = bNode;

            return FetchResult::Succeed;
        } else {
            // Without buttons.
            return FetchResult::NodeNotFound;
        }
    }
    return FetchResult::NodeNotFound;
}

bool ExcService::getType(const StringMap &request, StringMap &response) {
    String type = this->type();
    if (!type.isNullOrEmpty()) {
        JsonNode data;
        data.add(JsonNode("type", type));
        response["data"] = data.toString();
        return true;
    }
    // The current type is invalid.
    response.addRange(HttpCode::at(ExchangeTypeInvalid));
    return false;
}

bool ExcService::setType(const StringMap &request, StringMap &response) {
    String type = request["type"];
    if (isTypeValid(type)) {
        // update profile yml file.
        ServiceFactory *factory = ServiceFactory::instance();
        assert(factory);
        auto *cs = factory->getService<IConfigService>();
        assert(cs);

        YmlNode::Properties properties;
        properties.add(ExcPrefix "type", type);
        if (!cs->updateConfigFile(properties)) {
            // Failed to save config file.
            response.addRange(HttpCode::at(FailedToSave));
            return false;
        }

        // create a provider by type.
        IExcProvider *provider = createProvider(type);
        assert(provider);
        if (_provider != nullptr) {
            _deletedProviders.add(_provider);
            _provider = provider;
            if (_deletedProviders.count() > 1) {
                _deletedProviders.removeAt(0);
            }
        } else {
            _provider = provider;
        }

        response.addRange(HttpCode::okCode());
        return true;
    } else {
        // Can not find exchange type.
        response.addRange(HttpCode::at(CannotFindExchangeType));
    }

    return false;
}

// Labels
bool ExcService::getLabels(const SqlSelectFilter &filter, DataTable &table) {
    auto esp = dynamic_cast<ExcSimProvider *>(_provider);
    if (esp != nullptr) {
        return esp->getLabels(filter, table);
    }
    return false;
}

bool ExcService::getLabel(const StringMap &request, StringMap &response) {
    auto esp = dynamic_cast<ExcSimProvider *>(_provider);
    if (esp != nullptr) {
        return esp->getLabel(request, response);
    }
    response.addRange(HttpCode::at(ExchangeTypeNotSimulator));
    return false;
}

bool ExcService::addLabel(const StringMap &request, StringMap &response) {
    auto esp = dynamic_cast<ExcSimProvider *>(_provider);
    if (esp != nullptr) {
        return esp->addLabel(request, response);
    }
    response.addRange(HttpCode::at(ExchangeTypeNotSimulator));
    return false;
}

bool ExcService::removeLabel(const StringMap &request, StringMap &response) {
    auto esp = dynamic_cast<ExcSimProvider *>(_provider);
    if (esp != nullptr) {
        return esp->removeLabel(request, response);
    }
    response.addRange(HttpCode::at(ExchangeTypeNotSimulator));
    return false;
}

bool ExcService::updateLabel(const StringMap &request, StringMap &response) {
    auto esp = dynamic_cast<ExcSimProvider *>(_provider);
    if (esp != nullptr) {
        return esp->updateLabel(request, response);
    }
    response.addRange(HttpCode::at(ExchangeTypeNotSimulator));
    return false;
}

// Tables
bool ExcService::getTables(const SqlSelectFilter &filter, DataTable &table) {
    auto esp = dynamic_cast<ExcSimProvider *>(_provider);
    if (esp != nullptr) {
        return esp->getTables(filter, table);
    }
    return false;
}

bool ExcService::getTable(const StringMap &request, StringMap &response) {
    auto esp = dynamic_cast<ExcSimProvider *>(_provider);
    if (esp != nullptr) {
        return esp->getTable(request, response);
    }
    response.addRange(HttpCode::at(ExchangeTypeNotSimulator));
    return false;
}

bool ExcService::addTable(const StringMap &request, StringMap &response) {
    auto esp = dynamic_cast<ExcSimProvider *>(_provider);
    if (esp != nullptr) {
        return esp->addTable(request, response);
    }
    response.addRange(HttpCode::at(ExchangeTypeNotSimulator));
    return false;
}

bool ExcService::removeTable(const StringMap &request, StringMap &response) {
    auto esp = dynamic_cast<ExcSimProvider *>(_provider);
    if (esp != nullptr) {
        return esp->removeTable(request, response);
    }
    response.addRange(HttpCode::at(ExchangeTypeNotSimulator));
    return false;
}

bool ExcService::updateTable(const StringMap &request, StringMap &response) {
    auto esp = dynamic_cast<ExcSimProvider *>(_provider);
    if (esp != nullptr) {
        return esp->updateTable(request, response);
    }
    response.addRange(HttpCode::at(ExchangeTypeNotSimulator));
    return false;
}

// Buttons
bool ExcService::getButtons(const SqlSelectFilter &filter, DataTable &table) {
    auto esp = dynamic_cast<ExcSimProvider *>(_provider);
    if (esp != nullptr) {
        return esp->getButtons(filter, table);
    }
    return false;
}

bool ExcService::getButton(const StringMap &request, StringMap &response) {
    auto esp = dynamic_cast<ExcSimProvider *>(_provider);
    if (esp != nullptr) {
        return esp->getButton(request, response);
    }
    response.addRange(HttpCode::at(ExchangeTypeNotSimulator));
    return false;
}

bool ExcService::addButton(const StringMap &request, StringMap &response) {
    auto esp = dynamic_cast<ExcSimProvider *>(_provider);
    if (esp != nullptr) {
        return esp->addButton(request, response);
    }
    response.addRange(HttpCode::at(ExchangeTypeNotSimulator));
    return false;
}

bool ExcService::removeButton(const StringMap &request, StringMap &response) {
    auto esp = dynamic_cast<ExcSimProvider *>(_provider);
    if (esp != nullptr) {
        return esp->removeButton(request, response);
    }
    response.addRange(HttpCode::at(ExchangeTypeNotSimulator));
    return false;
}

bool ExcService::updateButton(const StringMap &request, StringMap &response) {
    auto esp = dynamic_cast<ExcSimProvider *>(_provider);
    if (esp != nullptr) {
        return esp->updateButton(request, response);
    }
    response.addRange(HttpCode::at(ExchangeTypeNotSimulator));
    return false;
}