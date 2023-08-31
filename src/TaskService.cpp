//
//  TaskService.cpp
//  tserver
//
//  Created by baowei on 2022/12/16.
//  Copyright (c) 2022 com. All rights reserved.
//

#include "configuration/ConfigService.h"
#include "diag/Trace.h"
#include "IO/Path.h"
#include "IO/File.h"
#include "IO/Directory.h"
#include "system/Math.h"
#include "crypto/Md5Provider.h"
#include "IO/Zip.h"
#include "TaskService.h"
#include "HttpErrorCode.h"
#include "tasks/TaskContext.h"
#include "tasks/TaskStorage.h"
#include "tasks/TaskCache.h"
#include "tasks/Execution.h"

using namespace IO;
using namespace Config;
using namespace Crypto;
using namespace Diag;

TaskService::TaskService() : _storage(nullptr), _cache(nullptr) {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    factory->addService<TaskService>(this);
}

TaskService::~TaskService() {
    delete _storage;
    _storage = nullptr;

    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    factory->removeService<TaskService>();
}

bool TaskService::initialize() {
    loadData();

    _cache = new TaskCacheMemory();

    _timerService.initialize(_storage);

    _dbService.initialize();

    return true;
}

bool TaskService::unInitialize() {
    _timerService.unInitialize();

    _dbService.unInitialize();

    return true;
}

bool TaskService::loadData() {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto *cs = factory->getService<IConfigService>();
    assert(cs);

    String type;
    cs->getProperty(TaskPrefix "type", type);
    if (String::equals(type, "file", true)) {
        Trace::info("Load task from file.");
        _storage = new TaskFile();
    } else if (String::equals(type, "database", true)) {
        Trace::info("Load task from database.");
        _storage = new TaskDatabase();
    }
    return _storage != nullptr && _storage->load();
}

bool TaskService::getTasks(const SqlSelectFilter &filter, DataTable &table) {
    return _storage != nullptr && _storage->getTasks(filter, table);
}

bool TaskService::getTask(const StringMap &request, StringMap &response) {
    if (_storage != nullptr) {
        Crontab crontab;
        String name = request["name"];
        if (_storage->getTask(name, crontab)) {
            response["code"] = "0";
            response["data"] = crontab.toJsonNode().toString();
            return true;
        }
        response.addRange(HttpCode::at(CannotFindTask));
        return false;
    }
    response.addRange(HttpCode::at(CannotFindTaskType));
    return false;
}

bool TaskService::addTask(const StringMap &request, StringMap &response) {
    if (_storage != nullptr) {
        return _storage->addTask(request, response);
    }
    response.addRange(HttpCode::at(CannotFindSimType));
    return false;
}

bool TaskService::addTaskFile(const StringMap &request, StringMap &response) {
    if (_storage != nullptr) {
        String name = request["name"];
        String md5 = request["md5"];
        String fullFileName = request["fullFileName"];

        // find file name.
        Crontab current;
        if (!_storage->getTask(name, current)) {
            // Can not find task by name.
            response.addRange(HttpCode::at(CannotFindTask));
            return false;
        }

        // file not exist?
        if (!File::exists(fullFileName)) {
            // Can not find the upload file.
            response.addRange(HttpCode::at(CannotFindFile));
            return false;
        }

        // check md5.
        if (!md5.isNullOrEmpty()) {
            String actual;
            Md5Provider md5p;
            if (md5p.computeFileHash(fullFileName, actual)) {
                if (!String::equals(actual, md5, true)) {
                    // Failed to verify the upload file md5.
                    response.addRange(HttpCode::at(FailedToVerifyMd5));
                    return false;
                }
            }
        }

        // check task file path.
        String file = current.execution()->currentFile();
        String path = Path::getDirectoryName(file);
        if (!Directory::exists(path)) {
            Directory::createDirectory(path);
        }

        // extract or copy file, check zip first.
        Zip zip(fullFileName);
        if (zip.isValid()) {
            if (!Zip::extract(fullFileName, path)) {
                if (Directory::exists(path)) {
                    Directory::deleteDirectory(path);
                }
                // Can not extract the zip file.
                response.addRange(HttpCode::at(CannotExtractZip));
                return false;
            }
        } else {
            // not a zip file.
            if (!File::copy(fullFileName, file, true)) {
                // Can not copy the file.
                response.addRange(HttpCode::at(CannotCopyApp));
                return false;
            }
        }

#ifndef WIN32
        // grant file permission.
        StringArray files;
        Directory::getFiles(path, "*", SearchOption::AllDirectories, files);
        for (size_t i = 0; i < files.count(); ++i) {
            File::chmod(files[i]);
        }
#endif

        response.addRange(HttpCode::okCode());
        return true;
    }
    response.addRange(HttpCode::at(CannotFindSimType));
    return false;
}

bool TaskService::removeTask(const StringMap &request, StringMap &response) {
    if (_storage != nullptr) {
        return _storage->removeTask(request, response);
    }
    response.addRange(HttpCode::at(CannotFindSimType));
    return false;
}

bool TaskService::updateTask(const StringMap &request, StringMap &response) {
    if (_storage != nullptr) {
        return _storage->updateTask(request, response);
    }
    response.addRange(HttpCode::at(CannotFindSimType));
    return false;
}
