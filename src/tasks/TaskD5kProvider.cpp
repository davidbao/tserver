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

bool TaskD5kProvider::getValue(const Column &column, DbValue &value) {
    auto ds = dynamic_cast<const D5kSource *>(_dataSource);
    int tableNo, contextNo;
    if (D5kSource::parseTableStyle(column.style, tableNo, contextNo)) {
        CTableOp tableOp;
        int result = tableOp.Open(ds->appNo, tableNo, (short) contextNo);
        if (result < 0) {
            Trace::error(String::format("Can not open table! appNo: %d, contextNo: %d", tableNo, contextNo));
            return false;
        }

        String type, sql;
        if (D5kSource::parseColumnStyle(column.style, type, sql)) {
            CBuffer buffer;
#ifndef HAS_D5000
            buffer.setValue(DbValue(DbValue::fromTypeStr(type), Random::getRandValue(1, 100)));
#endif
            result = tableOp.SqlGet(sql, buffer);
            if (result >= 0) {
                return getValue(buffer, type, value);
            }
        } else {
            Trace::error(String::format("Can not parse style! column name: %s", column.name.c_str()));
        }
    } else {
        Trace::error(String::format("Can not parse style! column name: %s", column.name.c_str()));
    }
    return false;
}

bool TaskD5kProvider::getValue(const Table &table, DataTable &dataTable) {
    return false;
}

bool TaskD5kProvider::getValue(const Style &value, Variable::Values &values) {
    return false;
}

bool TaskD5kProvider::getValue(const CBuffer &buffer, const String &type, DbValue &value) {
    if (buffer.GetLength() > 0) {
        DbValue::Type t = DbValue::fromTypeStr(type);
        auto v = (const D5kValue *) buffer.GetBufPtr();
        if (String::equals(type, "int", true)) {
            value = DbValue(t, v->nValue);
            return true;
        } else if (String::equals(type, "long", true)) {
            value = DbValue(t, v->lValue);
            return true;
        } else if (String::equals(type, "float", true)) {
            value = DbValue(t, v->fValue);
            return true;
        } else if (String::equals(type, "uchar", true)) {
            value = DbValue(t, v->ucValue);
            return true;
        } else if (String::equals(type, "string", true) ||
                   String::equals(type, "vstring", true) ||
                   String::equals(type, "varchar", true)) {
            value = DbValue(t, v->strValue);
            return true;
        }
    }
    return false;
}
