//
//  StorageService.h
//  tserver
//
//  Created by baowei on 2022/12/16.
//  Copyright © 2022 com. All rights reserved.
//

#ifndef TSERVER_STORAGESERVICE_H
#define TSERVER_STORAGESERVICE_H

#include "system/ServiceFactory.h"
#include "database/SqlSelectFilter.h"
#include "database/DbClient.h"
#include "DataContext.h"

using namespace Database;

class StorageService : public IDataProvider {
public:
    StorageService();

    ~StorageService() override;

    FetchResult getLabelValues(const String &labelName, const StringArray &tagNames, StringMap &values) override;

    FetchResult getTableValues(const String &tableName, const SqlSelectFilter &filter, DataTable &table) override;

private:
    DbClient *dbClient() const;

private:
    static void createSqlFile(const String &fileName, const String &sql);

    // sqlIndex: 0 is label sql; 1 is table query sql; 2 is table count sql.
    static String getSqlFileName(const String &name, int sqlIndex);

    static String getSql(const String &name, int sqlIndex);

    static String getTablePrefix();
};


#endif //TSERVER_STORAGESERVICE_H
