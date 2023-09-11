//
//  ExcService.h
//  tserver
//
//  Created by baowei on 2022/12/17.
//  Copyright (c) 2022 com. All rights reserved.
//

#ifndef TSERVER_EXCSERVICE_H
#define TSERVER_EXCSERVICE_H

#include "system/ServiceFactory.h"
#include "exchanges/ExcContext.h"

using namespace System;

class ExcService : public IService {
public:
    ExcService();

    ~ExcService() override;

    bool initialize();

    bool unInitialize();

    FetchResult getLabelValues(const JsonNode &request, JsonNode &response);

    FetchResult getTableValues(const JsonNode &request, JsonNode &response);

    FetchResult execButton(const JsonNode &request, JsonNode &response);

    bool getType(const StringMap &request, StringMap &response);

    bool setType(const StringMap &request, StringMap &response);

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
    String type() const;

    IExcProvider *provider() const;

private:
    static IExcProvider *createProvider(const String &type);

    static bool isTypeValid(const String &type);

private:
    IExcProvider *_provider;

    PList<IExcProvider> _deletedProviders;
};


#endif //TSERVER_EXCSERVICE_H
