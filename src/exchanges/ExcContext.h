//
//  ExcContext.h
//  tserver
//
//  Created by baowei on 2022/12/17.
//  Copyright (c) 2022 com. All rights reserved.
//

#ifndef TSERVER_EXCCONTEXT_H
#define TSERVER_EXCCONTEXT_H

#include "json/JsonNode.h"
#include "http/HttpContent.h"
#include "database/DataTable.h"
#include "database/SqlSelectFilter.h"
#include <cassert>

using namespace Data;
using namespace Http;
using namespace Database;
using namespace System;

enum FetchResult {
    Succeed = 0,
    ConfigError = 1,
    ExecFailed = 2,
    RowCountError = 3,
    TagError = 4,
    ColumnError = 5,
    NodeNotFound = 6,
    DbError = 7,
    LabelNotFound = 8,
    TableNotFound = 9
};

#define ExcPrefix "summer.exchange."

class IExcProvider {
public:
    IExcProvider() = default;

    virtual ~IExcProvider() = default;

    virtual FetchResult getLabelValues(const String &labelName, const StringArray &tagNames,
                                       const SqlSelectFilter &filter, VariantMap &values) = 0;

    virtual FetchResult getTableValues(const String &tableName, const StringArray &colNames,
                                       const SqlSelectFilter &filter, DataTable &table) = 0;
};

#endif //TSERVER_EXCCONTEXT_H
