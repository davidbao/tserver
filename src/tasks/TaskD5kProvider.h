//
//  TaskD5kProvider.h
//  tserver
//
//  Created by baowei on 2022/12/16.
//  Copyright (c) 2022 com. All rights reserved.
//

#ifndef TSERVER_TASKPD5KROVIDER_H
#define TSERVER_TASKPD5KROVIDER_H

#include "system/ServiceFactory.h"
#include "database/SqlSelectFilter.h"
#include "database/DbClient.h"
#include "TaskContext.h"

#ifdef HAS_D5000
#include "odb_api/odb_tableop.h"
using namespace ODB;
#endif

using namespace Database;
using namespace System;

#ifndef HAS_D5000

class CBuffer {
public:
    CBuffer() = default;

    ~CBuffer() = default;

    inline char *GetBufPtr() const {
        return (char *) &_value.value();
    }

    inline int GetLength() const {
        return (int) _value.valueSize();
    }

    void setValue(const DbValue &value) {
        _value = value;
    }

private:
    DbValue _value;
};

class CTableOp {
public:
    CTableOp() = default;

    ~CTableOp() = default;

    int Open(const int app_no, const int table_no, const short context_no = 0, bool is_map = false) {
        return 0;
    }

    int SqlGet(const char* str_sql, CBuffer& buf_base) {
        return 1;
    }
};

#endif

class TaskD5kProvider : public ITaskProvider {
public:
    union D5kValue {
        int8_t cValue;
        uint8_t ucValue;
        float fValue;
        int32_t nValue;
        int64_t lValue;
        char *strValue;

        D5kValue() : lValue(0) {
        }
    };

    explicit TaskD5kProvider(const D5kSource *ds);

    ~TaskD5kProvider() override;

    bool getValue(const Column &column, DbValue &value) override;

    bool getValue(const Table &table, DataTable &dataTable) override;

    bool getValue(const Style &value, Variable::Values &values) override;

private:
    static bool getValue(const CBuffer &buffer, const String &type, DbValue &value);
};

#endif //TSERVER_TASKD5KPROVIDER_H
