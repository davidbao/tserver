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
#include "http/HttpContent.h"
#include "database/DataTable.h"
#include "database/SqlSelectFilter.h"

using namespace Data;
using namespace Http;
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

enum HttpErrorCode {
    // simulator
    CannotFindLabel = 50,           // Can not find label by name.
    DuplicateName = 51,             // Duplicate name.
    FailedToSave = 52,              // Failed to save config file.
    CannotFindTable = 53,           // Can not find table by name.

    // task
    CannotFindTask = 60,            // Can not find task by name.
    CannotFindFile = 61,            // Can not find the upload file.
    FailedToVerifyMd5 = 62,         // Failed to verify the upload file md5.
    CannotExtractZip = 63,          // Can not extract the zip file.
    CannotCopyApp = 64,             // Can not copy the app file.
    CycleInvalid = 65,              // Cycle interval is invalid.
    TimeInvalid = 66,               // Time interval is invalid.
    RepeatInvalid = 67,             // Repeat type is invalid.
    CannotFindTaskType = 68,        // Can not find task type.

    // web
    CannotFindWWWBundlePath = 81,   // Can not find www bundle path.
    NotAZipFile = 82,               // The uploaded file is not a zip file.

    // exchange
    CannotFindExchangeType = 90,    // Can not find exchange type.
    ExchangeTypeInvalid = 91,       // The current type is invalid.

    // catalog
};

class HttpRegisters {
public:
    static void registerCodes() {
        HttpCode::registerCode({
                                       {CannotFindLabel,        "Can not find label by name."},
                                       {DuplicateName,          "Duplicate name."},
                                       {FailedToSave,           "Failed to save config file."},
                                       {CannotFindTable,        "Can not find table by name."},
                                       {CannotFindTask,         "Can not find task by name."},
                                       {CannotFindFile,         "Can not find the upload file."},
                                       {FailedToVerifyMd5,      "Failed to verify the upload file md5."},
                                       {CannotExtractZip,       "Can not extract the zip file."},
                                       {CannotCopyApp,          "Can not copy the app file."},
                                       {CycleInvalid,           "Cycle interval is invalid."},
                                       {TimeInvalid,            "Time interval is invalid."},
                                       {RepeatInvalid,          "Repeat type is invalid."},
                                       {CannotFindTaskType,     "Can not find task type."},
                                       {CannotFindExchangeType, "Can not find exchange type."},
                                       {ExchangeTypeInvalid,    "The current type is invalid."},
                               });
    }
};

#endif //TSERVER_DATACONTEXT_H
