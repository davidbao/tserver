//
//  ExcDbSqlStorage.h
//  tserver
//
//  Created by baowei on 2023/11/16.
//  Copyright (c) 2023 com. All rights reserved.
//

#ifndef TSERVER_EXCDBSQLSTORAGE_H
#define TSERVER_EXCDBSQLSTORAGE_H

#include "ExcDbContent.h"
#include "database/SqlConnection.h"

using namespace Database;

class ExcDbSqlFile : public IExcDbSqlStorage {
public:
    ExcDbSqlFile() = default;

    bool load() override;

    String getSql(const String &name, ExcSqlType type) override;

private:
    String getSqlFileName(const String &name, ExcSqlType type);

    String fileName() const;

    void rescan();

private:
    YmlNode::Properties _properties;

    DateTime _modifyTime;

private:
#define DbLabelPrefix "labels[%d]."
#define DbTablePrefix "tables[%d]."
};

class ExcDbSqlDatabase : public IExcDbSqlStorage {
public:
    ExcDbSqlDatabase() = default;

    bool load() override;

    String getSql(const String &name, ExcSqlType type) override;

private:
};

#endif //TSERVER_EXCDBSQLSTORAGE_H
