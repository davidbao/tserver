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

bool TaskDbProvider::getValue(const Table &table, DataTable &t) {
    if (_dbClient != nullptr) {
        const String &sql = table.style["sql"];
        if (!sql.isNullOrEmpty()) {
            return _dbClient->executeSqlQuery(sql, t);
        } else {
            return getValue(table.columns, t);
        }
    }
    return false;
}

bool TaskDbProvider::getValue(const Columns &columns, DataTable &t) {
    t.addColumns(columns.toColumns());

    DataRow row;
    for (size_t i = 0; i < t.columnCount(); ++i) {
        const DataColumn &c = t.columns()[i];
        Column column;
        if (columns.getColumn(c.name(), column)) {
            if (String::equals(column.registerStr, "SnowFlake", true)) {
                row.addCell(DataCell(c, DbClient::generateSnowFlakeId()));
            } else if (String::equals(column.registerStr, "Uuid", true)) {
                row.addCell(DataCell(c, Uuid::generate().toString()));
            } else {
                const String &sql = column.style["sql"];
                DataTable dataTable;
                if (_dbClient->executeSqlQuery(sql, dataTable) &&
                    dataTable.columnCount() == 1 && dataTable.rowCount() == 1) {
                    const DataCell &cell = dataTable.rows()[0].cells()[0];
                    row.addCell(DataCell(c, cell.value()));
                } else {
                    row.addCell(DataCell(c, DbValue::NullValue));
                }
            }
        }
    }
    if (row.cellCount() > 0) {
        t.addRow(row);
        return true;
    }
    return false;
}
