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

using namespace Common;
using namespace Database;

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

    virtual FetchResult getLabelValues(const String& labelName, const StringArray &tagNames, StringMap& values) = 0;

    virtual FetchResult getTableValues(const String &tableName, const SqlSelectFilter &filter, DataTable &table) = 0;
};

#endif //TSERVER_DATACONTEXT_H
