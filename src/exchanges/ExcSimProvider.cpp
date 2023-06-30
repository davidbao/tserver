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
#include "diag/Trace.h"
#include "ExcSimProvider.h"
#include "../HttpErrorCode.h"
#include "ExcSimStorage.h"
#include "ExcSimCache.h"

using namespace Config;
using namespace IO;
using namespace System;
using namespace Diag;

ExcSimProvider::ExcSimProvider() : _storage(nullptr) {
    Trace::info("The exchange type is simulator.");

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
            Tags tags;
            label.getTags(tagNames, tags);
            if (tags.count() == 0) {
                Trace::error(String::format("Can not find label tags, label name: %s", labelName.c_str()));
                return FetchResult::TagError;
            }

            for (size_t i = 0; i < tags.count(); i++) {
                const Tag &tag = tags[i];
                values.add(tag.name, tag.getValue(&label, filter));
            }
            return FetchResult::Succeed;
        } else {
            Trace::error(String::format("Can not find a label, name: %s", labelName.c_str()));
            return FetchResult::LabelNotFound;
        }
    }

    Trace::error("Can not find simulator config!");
    return FetchResult::ConfigError;
}

FetchResult ExcSimProvider::getTableValues(const String &tableName, const StringArray &colNames,
                                           const SqlSelectFilter &filter, DataTable &dataTable) {
    if (_storage != nullptr) {
        Table table;
        if (_storage->getTable(tableName, table)) {
            Columns columns;
            table.getColumns(colNames, columns);
            if (columns.count() == 0) {
                Trace::error(String::format("Can not find table columns, table name: %s", tableName.c_str()));
                return FetchResult::ColumnError;
            }

            // initial the colNames of table.
            for (size_t j = 0; j < columns.count(); ++j) {
                const Column &col = columns[j];
                DataColumn column(col.name, DbValue::toTypeStr(col.type()));
                dataTable.addColumn(column);
            }

            // add rows.
            int start = table.rowCount() < filter.pageSize() ? 0 : filter.offset();
            int next = start + filter.pageSize();
            int end = table.rowCount() < filter.pageSize() ?
                      table.rowCount() :
                      (next < table.rowCount() ? next : table.rowCount());
            for (int i = start; i < end; ++i) {
                bool validRow = true;
                DataRow dataRow;
                for (int j = 0; j < (int) columns.count(); ++j) {
                    const Column &col = columns[j];
                    Variant value = col.getCellValue(&table, filter, i, j);
                    if (value.isNullValue())
                        validRow = false;
                    dataRow.addCell(DataCell(dataTable.columns()[col.name], value));
                }
                if (validRow) {
                    dataTable.addRow(dataRow);
                }
            }

            // initial the total count of table.
            dataTable.setTotalCount(Math::min(table.rowCount(), (int) dataTable.rowCount()));

            // sort
//#ifdef DEBUG
//            printf("Before sort!\n");
//            for (int i = 0; i < dataTable.rowCount(); ++i) {
//                auto row = dataTable.rows()[i];
//                printf("%d: ", i);
//                for (int j = 0; j < row.cellCount(); ++j) {
//                    auto cell = row.cells()[j];
//                    printf("%s", cell.valueStr().c_str());
//                    printf("%s", ",");
//                }
//                printf("%s", "\n");
//            }
//#endif
            dataTable.sort(filter.orderBy());
//#ifdef DEBUG
//            printf("%s", "After sort!\n");
//            for (int i = 0; i < dataTable.rowCount(); ++i) {
//                auto row = dataTable.rows()[i];
//                printf("%d: ", i);
//                for (int j = 0; j < row.cellCount(); ++j) {
//                    auto cell = row.cells()[j];
//                    printf("%s", cell.valueStr().c_str());
//                    printf("%s", ",");
//                }
//                printf("%s", "\n");
//            }
//#endif

            return FetchResult::Succeed;
        } else {
            Trace::error(String::format("Can not find a table, name: %s", tableName.c_str()));
            return FetchResult ::TableNotFound;
        }
    }

    Trace::error("Can not find simulator config!");
    return FetchResult::ConfigError;
}

bool ExcSimProvider::loadData() {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto *cs = factory->getService<IConfigService>();
    assert(cs);

    String type;
    cs->getProperty(SimulatorPrefix "type", type);
    if (String::equals(type, "file", true)) {
        Trace::info("Load simulator from file.");
        _storage = new ExcSimFile();
    } else if (String::equals(type, "database", true)) {
        Trace::info("Load simulator from database.");
        _storage = new ExcSimDatabase();
    }
    return _storage != nullptr && _storage->load();
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
