//
//  TaskD5kProvider.cpp
//  tserver
//
//  Created by baowei on 2022/12/16.
//  Copyright (c) 2022 com. All rights reserved.
//

#include "TaskD5kProvider.h"
#include "diag/Trace.h"
#include "microservice/DataSourceService.h"
#include "system/Random.h"

using namespace Diag;
using namespace Microservice;

TaskD5kProvider::TaskD5kProvider(const D5kSource *ds) : ITaskProvider(ds) {
}

TaskD5kProvider::~TaskD5kProvider() = default;

bool TaskD5kProvider::getValue(const Table &table, DataTable &t) {
    auto ds = dynamic_cast<const D5kSource *>(_dataSource);
    int tableNo, contextNo;
    if (D5kSource::parseTableStyle(table.style, tableNo, contextNo)) {
        CTableOp tableOp;
        int result = tableOp.Open(ds->appNo, tableNo, (short) contextNo);
        if (result < 0) {
            return false;
        }

        t.addColumns(table.columns.toColumns());

        DataRow row;
        for (size_t j = 0; j < t.columnCount(); ++j) {
            const DataColumn &c = t.columns()[j];
            Column column;
            if (table.columns.getColumn(c.name(), column)) {
                String type, sql;
                if (D5kSource::parseColumnStyle(column.style, type, sql)) {
                    CBuffer buffer;
#ifdef MAC_OS
                    DbValue value(DbValue::fromTypeStr(type), Random::getRandValue(1, 100));
                    buffer.setValue(value);
#endif
                    result = tableOp.SqlGet(sql, buffer);
                    if (result >= 0) {
                        row.addCell(DataCell(c, getValue(buffer, type)));
                    }
                } else {
                    if (!column.style.isEmpty()) {
                        Trace::error(
                                String::format("Can not parse column style! name: %s", column.name.c_str()));
                    }
                }
            }
        }
        if (row.cellCount() > 0) {
            t.addRow(row);
            return true;
        }
    } else {
        if (!table.style.isEmpty()) {
            Trace::error(String::format("Can not parse table style! name: %s", table.name.c_str()));
        }
    }
    return false;
}

DbValue TaskD5kProvider::getValue(const CBuffer &buffer, const String &type) {
    if (buffer.GetLength() > 0) {
        DbValue::Type t = DbValue::fromTypeStr(type);
        auto v = (const D5kValue *) buffer.GetBufPtr();
        if (String::equals(type, "int", true)) {
            return DbValue(t, v->nValue);
        } else if (String::equals(type, "long", true)) {
            return DbValue(t, v->lValue);
        } else if (String::equals(type, "float", true)) {
            return DbValue(t, v->fValue);
        } else if (String::equals(type, "uchar", true)) {
            return DbValue(t, v->ucValue);
        } else if (String::equals(type, "string", true) ||
                   String::equals(type, "vstring", true) ||
                   String::equals(type, "varchar", true)) {
            return DbValue(t, v->strValue);
        }
    }
    return DbValue::NullValue;
}
