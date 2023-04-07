//
//  ExcSimProvider.cpp
//  tserver
//
//  Created by baowei on 2022/12/17.
//  Copyright (c) 2022 com. All rights reserved.
//

#include "system/ServiceFactory.h"
#include "system/Math.h"
#include "IO/Path.h"
#include "ExcSimProvider.h"
#include "../HttpErrorCode.h"
#include "ExcSimStorage.h"
#include "ExcSimCache.h"

using namespace Config;
using namespace IO;
using namespace System;

ExcSimProvider::ExcSimProvider() : _storage(nullptr) {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto *cs = factory->getService<IConfigService>();
    assert(cs);

    String type;
    cs->getProperty("summer.exchange.type", type);
    if (String::equals(type, "simulator", true)) {
        // It's a simulator, so do initialization.
        _cache = new ExcSimCacheMemory();

        loadData();
    }
}

ExcSimProvider::~ExcSimProvider() {
    delete _storage;
    _storage = nullptr;

    delete _cache;
    _cache = nullptr;
}

FetchResult ExcSimProvider::getLabelValues(const String &labelName, const StringArray &tagNames,
                                           const SqlSelectFilter &filter, VariantMap &values) {
    if (_storage != nullptr) {
        Label label;
        if (_storage->getLabel(labelName, label)) {
            for (size_t j = 0; j < label.tags.count(); j++) {
                const Tag &tag = label.tags[j];
                if (tagNames.isEmpty() || tagNames.contains(tag.name)) {
                    values.add(tag.name, tag.getValue(&label, filter));
                }
            }
            return FetchResult::Succeed;
        }
    }
    return FetchResult::RowCountError;
}

FetchResult ExcSimProvider::getTableValues(const String &tableName, const StringArray &colNames,
                                           const SqlSelectFilter &filter, DataTable &dataTable) {
    if (_storage != nullptr) {
        Table table;
        if (_storage->getTable(tableName, table)) {
            Columns columns;
            table.getColumns(colNames, columns);
            if (columns.count() == 0) {
                return FetchResult::ColumnError;
            }

            // initial the colNames of table.
            for (size_t j = 0; j < columns.count(); ++j) {
                const Column &col = columns[j];
                DataColumn column(col.name, DbValue::toTypeStr(col.type()));
                dataTable.addColumn(column);
            }

            // add rows.
            int start = table.rowCount < filter.pageSize() ? 0 : filter.offset();
            int next = start + filter.pageSize();
            int end = table.rowCount < filter.pageSize() ?
                      table.rowCount :
                      (next < table.rowCount ? next : table.rowCount);
            for (int row = start; row < end; ++row) {
                DataRow dataRow;
                for (size_t j = 0; j < columns.count(); ++j) {
                    const Column &col = columns[j];
                    dataRow.addCell(DataCell(dataTable.columns()[col.name], col.getCellValue(&table, filter, row)));
                }
                dataTable.addRow(dataRow);
            }

            // initial the total count of table.
            dataTable.setTotalCount(Math::min(table.rowCount, (int) dataTable.rowCount()));

            // sort
            dataTable.sort(filter.orderBy());

            return FetchResult::Succeed;
        }
    }
    return FetchResult::RowCountError;
}

bool ExcSimProvider::loadData() {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto *cs = factory->getService<IConfigService>();
    assert(cs);

    String type;
    cs->getProperty(SimulatorPrefix "type", type);
    if (String::equals(type, "file", true)) {
        _storage = new ExcSimFile();
        return _storage->load();
    } else if (String::equals(type, "database", true)) {
        _storage = new ExcSimDatabase();
        return _storage->load();
    }
    return false;
}

bool ExcSimProvider::getLabels(const SqlSelectFilter &filter, DataTable &table) {
    return _storage != nullptr && _storage->getLabels(filter, table);
}

bool ExcSimProvider::getLabel(const StringMap &request, StringMap &response) {
    if (_storage != nullptr) {
        Label label;
        String name = request["name"];
        if (_storage->getLabel(name, label)) {
            response["code"] = "0";
            response["data"] = label.toJsonNode().toString();
            return true;
        }
        response.addRange(HttpCode::at(CannotFindLabel));
        return false;
    }
    response.addRange(HttpCode::at(CannotFindSimType));
    return false;
}

bool ExcSimProvider::addLabel(const StringMap &request, StringMap &response) {
    if (_storage != nullptr) {
        return _storage->addLabel(request, response);
    }
    response.addRange(HttpCode::at(CannotFindSimType));
    return false;
}

bool ExcSimProvider::removeLabel(const StringMap &request, StringMap &response) {
    if (_storage != nullptr) {
        return _storage->removeLabel(request, response);
    }
    response.addRange(HttpCode::at(CannotFindSimType));
    return false;
}

bool ExcSimProvider::updateLabel(const StringMap &request, StringMap &response) {
    if (_storage != nullptr) {
        return _storage->updateLabel(request, response);
    }
    response.addRange(HttpCode::at(CannotFindSimType));
    return false;
}

bool ExcSimProvider::getTables(const SqlSelectFilter &filter, DataTable &table) {
    return _storage != nullptr && _storage->getTables(filter, table);
}

bool ExcSimProvider::getTable(const StringMap &request, StringMap &response) {
    if (_storage != nullptr) {
        Table table;
        String name = request["name"];
        if (_storage->getTable(name, table)) {
            response["code"] = "0";
            response["data"] = table.toJsonNode().toString();
            return true;
        }
        response.addRange(HttpCode::at(CannotFindTable));
        return false;
    }
    response.addRange(HttpCode::at(CannotFindSimType));
    return false;
}

bool ExcSimProvider::addTable(const StringMap &request, StringMap &response) {
    if (_storage != nullptr) {
        return _storage->addTable(request, response);
    }
    response.addRange(HttpCode::at(CannotFindSimType));
    return false;
}

bool ExcSimProvider::removeTable(const StringMap &request, StringMap &response) {
    if (_storage != nullptr) {
        return _storage->removeTable(request, response);
    }
    response.addRange(HttpCode::at(CannotFindSimType));
    return false;
}

bool ExcSimProvider::updateTable(const StringMap &request, StringMap &response) {
    if (_storage != nullptr) {
        return _storage->updateTable(request, response);
    }
    response.addRange(HttpCode::at(CannotFindSimType));
    return false;
}
