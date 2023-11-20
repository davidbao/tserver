//
//  ExcDbContent.h
//  tserver
//
//  Created by baowei on 2023/11/16.
//  Copyright (c) 2023 com. All rights reserved.
//

#ifndef TSERVER_EXCDBCONTENT_H
#define TSERVER_EXCDBCONTENT_H

#include "ExcContext.h"
#include "database/SqlConnection.h"

using namespace Database;

#define SqlPrefix DatabasePrefix "sql."

enum ExcSqlType {
    LabelSql = 0,
    TableQuerySql = 1,
    TableCountSql = 2
};

class IExcDbSqlStorage {
public:
    IExcDbSqlStorage() = default;

    virtual ~IExcDbSqlStorage() = default;

    virtual bool load() = 0;

    virtual String getSql(const String &name, ExcSqlType type) = 0;
};

#endif //TSERVER_EXCDBCONTENT_H
