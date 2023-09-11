//
//  ExcSimProvider.h
//  tserver
//
//  Created by baowei on 2022/12/17.
//  Copyright (c) 2022 com. All rights reserved.
//

#ifndef TSERVER_EXCSIMPROVIDER_H
#define TSERVER_EXCSIMPROVIDER_H

#include "configuration/ConfigService.h"
#include "ExcContext.h"
#include "ExcSimContext.h"

using namespace Data;
using namespace System;
using namespace Config;

class ExcSimProvider : public IExcProvider {
public:
    ExcSimProvider();

    ~ExcSimProvider() override;

    FetchResult getLabelValues(const String &labelName, const StringArray &tagNames,
                               const SqlSelectFilter &filter, VariantMap &values) override;

    FetchResult getTableValues(const String &tableName, const StringArray &colNames,
                               const SqlSelectFilter &filter, DataTable &dataTable) override;

    FetchResult execButton(const String &buttonName, const StringMap &params, VariantMap &results) override;

    // Labels
    bool getLabels(const SqlSelectFilter &filter, DataTable &table);

    bool getLabel(const StringMap &request, StringMap &response);

    bool addLabel(const StringMap &request, StringMap &response);

    bool removeLabel(const StringMap &request, StringMap &response);

    bool updateLabel(const StringMap &request, StringMap &response);

    // Tables
    bool getTables(const SqlSelectFilter &filter, DataTable &table);

    bool getTable(const StringMap &request, StringMap &response);

    bool addTable(const StringMap &request, StringMap &response);

    bool removeTable(const StringMap &request, StringMap &response);

    bool updateTable(const StringMap &request, StringMap &response);

    // Buttons
    bool getButtons(const SqlSelectFilter &filter, DataTable &table);

    bool getButton(const StringMap &request, StringMap &response);

    bool addButton(const StringMap &request, StringMap &response);

    bool removeButton(const StringMap &request, StringMap &response);

    bool updateButton(const StringMap &request, StringMap &response);

private:
    bool loadData();

private:
    IExcSimStorage *_storage;

    IExcSimCache *_cache;
};


#endif //TSERVER_EXCSIMPROVIDER_H
