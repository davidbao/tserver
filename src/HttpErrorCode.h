//
//  HttpErrorCode.h
//  tserver
//
//  Created by baowei on 2023/2/21.
//  Copyright (c) 2023 com. All rights reserved.
//

#ifndef TSERVER_HTTPERRORCODE_H
#define TSERVER_HTTPERRORCODE_H

#include "http/HttpContent.h"

using namespace Http;

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
    ExchangeTypeInvalid = 91,       // The current exchange type is invalid.
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
                                       {ExchangeTypeInvalid,    "The current exchange type is invalid."},
                               });
    }
};

#endif //TSERVER_HTTPERRORCODE_H