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
#include "ExcContext.h"

using namespace Database;

class ExcDbProvider : public IExcProvider {
public:
    ExcDbProvider();

    ~ExcDbProvider() override;

    FetchResult getLabelValues(const String &labelName, const StringArray &tagNames,
                               const SqlSelectFilter &filter, VariantMap &values) override;

    FetchResult getTableValues(const String &tableName, const StringArray &colNames,
                               const SqlSelectFilter &filter, DataTable &table) override;

    FetchResult execButton(const String &buttonName, const StringMap &params, VariantMap &results) override;

private:
    SqlConnection *connection() const;

private:
    static void createSqlFile(const String &fileName, const String &sql);

    // sqlIndex: 0 is label sql; 1 is table query sql; 2 is table count sql.
    static String getSqlFileName(const String &name, int sqlIndex);

    static String getSql(const String &name, int sqlIndex);

    static String getTablePrefix();

    static String updateSql(const SqlSelectFilter &filter, const String &sql);

private:
#define DatabasePrefix ExcPrefix "database."

#define MaxLabelCount 1000
#define DbLabelPrefix DatabasePrefix "labels[%d]."

#define MaxTableCount 1000
#define DbTablePrefix DatabasePrefix "tables[%d]."

#define LogPrefix DatabasePrefix "log."

private:
    bool _logSqlInfo;
};


#endif //TSERVER_EXCDBPROVIDER_H
