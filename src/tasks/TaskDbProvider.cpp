//
//  TaskDbProvider.cpp
//  tserver
//
//  Created by baowei on 2022/12/16.
//  Copyright (c) 2022 com. All rights reserved.
//

#include "TaskDbProvider.h"
#include "microservice/DataSourceService.h"
#include "system/Random.h"

using namespace Microservice;

TaskDbProvider::TaskDbProvider(const DbSource *ds) : ITaskProvider(ds), _dbClient(nullptr) {
    _dbClient = DataSourceService::open(ds->url, ds->userName, ds->password);
}

TaskDbProvider::~TaskDbProvider() {
    if (_dbClient != nullptr) {
        delete _dbClient;
        _dbClient = nullptr;
    }
}

bool TaskDbProvider::getValue(const Column &column, DbValue &value) {
    if (_dbClient == nullptr) {
        return false;
    }

    const String &sql = column.style["sql"];
    DataTable dataTable;
    if (_dbClient->executeSqlQuery(sql, dataTable) &&
        dataTable.columnCount() == 1 && dataTable.rowCount() == 1) {
        const DataCell &cell = dataTable.rows()[0].cells()[0];
        value = cell.value();
        return true;
    }
    return false;
}

bool TaskDbProvider::getValue(const Table &table, DataTable &dataTable) {
    if (_dbClient == nullptr) {
        return false;
    }

    const String &sql = table.style["sql"];
    DataTable t;
    if (_dbClient->executeSqlQuery(sql, t) && t.rowCount() > 0) {
        for (size_t i = 0; i < t.rowCount(); ++i) {
            const DataRow &r = t.rows()[i];
            DataRow row;
            for (size_t j = 0; j < table.columns.count(); ++j) {
                const Column &c = table.columns[j];
                DataColumn column(c.name, DbValue::fromTypeStr(c.type), c.pkey);
                if (String::equals(c.registerStr, "SnowFlake", true)) {
                    row.addCell(DataCell(column, DbClient::generateSnowFlakeId()));
                } else if (String::equals(c.registerStr, "Uuid", true)) {
                    row.addCell(DataCell(column, Uuid::generate().toString()));
                } else {
                    const DataCell &cell = r.cells().at(c.name);
                    row.addCell(DataCell(column, cell.value()));
                }
            }
            dataTable.addRow(row);
        }
        return true;
    }
    return false;
}

bool TaskDbProvider::getValue(const Style &value, Variable::Values &values) {
    if (_dbClient == nullptr) {
        return false;
    }

    const String &sql = value["sql"];
    DataTable table;
    if (_dbClient->executeSqlQuery(sql, table) &&
        table.rowCount() > 0 && table.columnCount() > 0) {
        for (size_t i = 0; i < table.rowCount(); ++i) {
            const DataRow &row = table.rows()[i];
            // retrieved the first column.
            values.add(row.cells()[0].value());
        }
        return true;
    }
    return false;
}
