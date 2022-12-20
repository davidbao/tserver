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

using namespace Microservice;

DataService::DataService() : _provider(nullptr) {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    factory->addService<DataService>(this);
}

DataService::~DataService() {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    factory->removeService<DataService>();

    delete _provider;
    _provider = nullptr;
}

bool DataService::initialize() {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    IConfigService *cs = factory->getService<IConfigService>();
    assert(cs);

    String type;
    cs->getProperty("summer.exchange.type", type);
    if (String::equals(type, "simulator", true)) {
        _provider = new SimulatorService();
    } else if (String::equals(type, "database", true)) {
        _provider = new StorageService();
    }

    return true;
}

bool DataService::unInitialize() {
    delete _provider;
    _provider = nullptr;

    return true;
}

FetchResult DataService::getLabelValues(const JsonNode &request, JsonNode &response) {
    if (_provider == nullptr) {
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
                StringMap values;
                FetchResult result = _provider->getLabelValues(name, tags, values);
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
            // It's not contains tags.
            return FetchResult::JsonError;
        }
    }

    return FetchResult::JsonError;
}

FetchResult DataService::getTableValues(const JsonNode &request, JsonNode &response) {
    if (_provider == nullptr) {
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
                SqlSelectFilter filter;
                if (SqlSelectFilter::parse(node["condition"].toString(), filter)) {
                    DataTable table("rows");
                    FetchResult result = _provider->getTableValues(name, filter, table);
                    JsonNode iNode("item");
                    iNode.add(JsonNode("name", name));
                    iNode.add(JsonNode("errorCode", (int) result));
                    if (result == FetchResult::Succeed) {
                        iNode.add(JsonNode("dataTotal", ((Int32) table.totalCount()).toString()));
                        iNode.add(JsonNode("maxPageNum",
                                           filter.pageSize() > 0 ? ((Int32) (table.totalCount() / filter.pageSize() +
                                                                             1)).toString() : "0"));
                        iNode.add(JsonNode("pageCount", ((Int32) filter.pageSize()).toString()));
                        iNode.add(JsonNode("pageNum", ((Int32) filter.page()).toString()));
                        iNode.add(JsonNode("pageStart", ((Int32) (filter.offset())).toString()));

                        JsonNode dataNode;
                        table.toJsonNode(dataNode, "g");
                        iNode.add(dataNode);
                    }
                    tNode.add(iNode);
                }
                response = tNode;

                return FetchResult::Succeed;
            }
        }
    }

    return FetchResult::JsonError;
}
