//
//  ExcDbSqlStorage.cpp
//  tserver
//
//  Created by baowei on 2023/11/16.
//  Copyright (c) 2023 com. All rights reserved.
//

#include "ExcDbSqlStorage.h"
#include "IO/Path.h"
#include "IO/File.h"
#include "configuration/ConfigService.h"
#include "system/Application.h"
#include "system/Environment.h"

using namespace Config;
using namespace System;

bool ExcDbSqlFile::load() {
    // load yml file.
    String fileName = this->fileName();
    Locker locker(&_properties);
    if (YmlNode::loadFile(fileName, _properties)) {
        File::getModifyTime(fileName, _modifyTime);
        return true;
    }
    return false;
}

String ExcDbSqlFile::getSql(const String &name, ExcSqlType type) {
    String fileName = getSqlFileName(name, type);
    if (File::exists(fileName)) {
        FileStream fs(fileName, FileMode::FileOpenWithoutException, FileAccess::FileRead);
        String sql;
        fs.readToEnd(sql);
        return sql;
    }
    return String::Empty;
}

String ExcDbSqlFile::getSqlFileName(const String &name, ExcSqlType type) {
    Locker locker(&_properties);

    rescan();

    if (_properties.count() > 0) {
        ServiceFactory *factory = ServiceFactory::instance();
        assert(factory);
        auto *cs = factory->getService<IConfigService>();
        assert(cs);

        String subPath;
        _properties.at("path", subPath);

        String sqlFileName;
        if (type == LabelSql) {
            for (int i = 0; i < MaxLabelCount; i++) {
                String n;
                if (!_properties.at(String::format(DbLabelPrefix "name", i), n)) {
                    break;
                }
                if (name == n) {
                    if (_properties.at(String::format(DbLabelPrefix "sql", i), sqlFileName)) {
                        break;
                    }
                }
            }
        } else if (type == TableQuerySql || type == TableCountSql) {
            for (int i = 0; i < MaxTableCount; i++) {
                String n;
                if (!_properties.at(String::format(DbTablePrefix "name", i), n)) {
                    break;
                }
                if (name == n) {
                    const char *fmt = type == TableQuerySql ? (DbTablePrefix "querySql") : (DbTablePrefix "countSql");
                    if (_properties.at(String::format(fmt, i), sqlFileName)) {
                        break;
                    }
                }
            }
        }

        const String appPath = Path::getAppPath();
        String fileName = Path::combine(Path::combine(appPath, subPath), sqlFileName);
        if (File::exists(fileName)) {
            return fileName;
        } else {
            Application *app = Application::instance();
            assert(app);
            fileName = Path::combine(Path::combine(app->rootPath(), subPath), sqlFileName);
            if (File::exists(fileName)) {
                return fileName;
            }
        }
    }
    return String::Empty;
}

void ExcDbSqlFile::rescan() {
    static uint64_t tick = Environment::getTickCount();
    if (Environment::getTickCount() - tick >= 30 * 1000) {
        tick = Environment::getTickCount();

        String fileName = this->fileName();

        DateTime modifyTime;
        File::getModifyTime(fileName, modifyTime);
        if (_modifyTime != modifyTime) {
            // reload.
            Trace::info(String::format("The file'%s' is changed, so reload it.",
                                       Path::getFileName(fileName).c_str()));
            YmlNode::Properties properties;
            if (YmlNode::loadFile(fileName, properties)) {
                _properties = properties;
                File::getModifyTime(fileName, _modifyTime);
            }
        }
    }
}

String ExcDbSqlFile::fileName() const {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto *cs = factory->getService<IConfigService>();
    assert(cs);

    String path = Application::instance()->rootPath();
    String name = cs->getProperty(SqlPrefix "file.name");
    String fileName = Path::combine(path, String::format("%s.yml", name.c_str()));
    return fileName;
}

bool ExcDbSqlDatabase::load() {
    return false;
}

String ExcDbSqlDatabase::getSql(const String &name, ExcSqlType type) {
    return String::Empty;
}
