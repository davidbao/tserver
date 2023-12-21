//
//  ExcDbProvider.h
//  tserver
//
//  Created by baowei on 2022/12/16.
//  Copyright (c) 2022 com. All rights reserved.
//

#ifndef TSERVER_EXCDBPROVIDER_H
#define TSERVER_EXCDBPROVIDER_H

#include "system/ServiceFactory.h"
#include "database/SqlSelectFilter.h"
#include "database/SqlConnection.h"
#include "ExcDbContent.h"
#include "ExcDbSqlStorage.h"
#include "../tasks/Crontab.h"

using namespace Database;

class ExcDbProvider : public IExcProvider {
public:
    ExcDbProvider();

    ~ExcDbProvider() override;

    FetchResult getLabelValues(const String &labelName, const StringArray &tagNames,
                               const SqlSelectFilter &filter, VariantMap &values) override;

    FetchResult getTableValues(const String &tableName, const StringArray &colNames,
                               const SqlSelectFilter &filter, DataTable &table) override;

    FetchResult execButton(const String &buttonName, ExecType type, const StringMap &params, VariantMap &results) override;

private:
    SqlConnection *connection() const;

    bool loadData();

private:
    static void createSqlFile(const String &fileName, const String &sql);

    String getSql(const String &name, ExcSqlType type);

    static String getSchema();

    static String updateSql(const SqlSelectFilter &filter, const String &sql);

    FetchResult execByArgument(Crontab &crontab, const String &buttonName, const StringMap &params, VariantMap &results);

    FetchResult execByTempTable(Crontab &crontab, const String &buttonName, const StringMap &params, VariantMap &results);

private:
    bool _logSqlInfo;

    IExcDbSqlStorage *_storage;

private:
#define LogPrefix DatabasePrefix "log."
};

#endif //TSERVER_EXCDBPROVIDER_H
