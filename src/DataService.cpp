//
//  DataService.cpp
//  tserver
//
//  Created by baowei on 2022/12/17.
//  Copyright © 2022 com. All rights reserved.
//

#include "configuration/ConfigService.h"
#include "DataService.h"
#include "SimulatorService.h"
#include "StorageService.h"

using namespace Config;

DataService::DataService() : _provider(nullptr) {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    factory->addService<DataService>(this);

    HttpRegisters::registerCodes();
}

DataService::~DataService() {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    factory->removeService<DataService>();

    if (_provider != nullptr) {
        delete _provider;
        _provider = nullptr;
    }
}

bool DataService::initialize() {
    _provider = createProvider(type());

    return true;
}

bool DataService::unInitialize() {
    if (_provider != nullptr) {
        delete _provider;
        _provider = nullptr;
    }

    return true;
}

String DataService::type() const {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto *cs = factory->getService<IConfigService>();
    assert(cs);

    String type;
    cs->getProperty("summer.exchange.type", type);
    return isTypeValid(type) ? type : String::Empty;
}

bool DataService::isTypeValid(const String &type) {
    if (String::equals(type, "simulator", true) ||
        String::equals(type, "database", true)) {
        return true;
    }
    return false;
}

IDataProvider *DataService::createProvider(const String &type) {
    IDataProvider *provider = nullptr;
    if (String::equals(type, "simulator", true)) {
        provider = new SimulatorService();
    } else if (String::equals(type, "database", true)) {
        provider = new StorageService();
    }
    return provider;
}

IDataProvider *DataService::provider() const {
    return _provider;
}

FetchResult DataService::getLabelValues(const JsonNode &request, JsonNode &response) {
    IDataProvider *provider = this->provider();
    if (provider == nullptr) {
        return FetchResult::ConfigError;
    }

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
                StringMap values;
                FetchResult result = provider->getLabelValues(name, tags, filter, values);
                JsonNode iNode("item");
                iNode.add(JsonNode("name", name));
                iNode.add(JsonNode("errorCode", (int) result));
                if (result == FetchResult::Succeed) {
                    JsonNode tagsNode("tags", values);
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

FetchResult DataService::getTableValues(const JsonNode &request, JsonNode &response) {
    IDataProvider *provider = this->provider();
    if (provider == nullptr) {
        return FetchResult::ConfigError;
    }

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

                        JsonNode dataNode;
                        table.toJsonNode(dataNode, "g");
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

bool DataService::getType(const StringMap &request, StringMap &response) {
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

bool DataService::setType(const StringMap &request, StringMap &response) {
    String type = request["type"];
    if (isTypeValid(type)) {
        // update profile yml file.
        ServiceFactory *factory = ServiceFactory::instance();
        assert(factory);
        auto *cs = factory->getService<IConfigService>();
        assert(cs);

        YmlNode::Properties properties;
        properties.add("summer.exchange.type", type);
        if (!cs->updateConfigFile(properties)) {
            // Failed to save config file.
            response.addRange(HttpCode::at(FailedToSave));
            return false;
        }

        // create a provider by type.
        IDataProvider *provider = createProvider(type);
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
