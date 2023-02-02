//
//  DataContext.h
//  tserver
//
//  Created by baowei on 2022/12/17.
//  Copyright © 2022 com. All rights reserved.
//

#ifndef TSERVER_DATACONTEXT_H
#define TSERVER_DATACONTEXT_H

#include "json/JsonNode.h"
#include "database/DataTable.h"
#include "database/SqlSelectFilter.h"
#include "system/Singleton.h"

using namespace Data;
using namespace Database;
using namespace System;

enum FetchResult {
    Succeed = 0,
    ConfigError = 1,
    ExecFailed = 2,
    RowCountError = 3,
    JsonError = 4
};

class IDataProvider {
public:
    IDataProvider() = default;

    virtual ~IDataProvider() = default;

    virtual FetchResult getLabelValues(const String &labelName, const StringArray &tagNames, StringMap &values) = 0;

    virtual FetchResult getTableValues(const String &tableName, const SqlSelectFilter &filter, DataTable &table) = 0;
};

class HttpCode {
public:
    enum Code {
        Success = 0,
        Unknown = 9
    };

    class Item {
    public:
        int code;
        String msg;

        Item() = default;
    };

    ~HttpCode() = default;

    void registerCode(int code, const String &msg);

    void registerCode(std::initializer_list<Item> list);

    String getMessage(int code) const;

    StringMap at(int code) const;

private:
    HttpCode();

    DECLARE_SINGLETON_CLASS(HttpCode);

public:
    static HttpCode *instance();

private:
    Dictionary<int, String> _codes;
};

#endif //TSERVER_DATACONTEXT_H
