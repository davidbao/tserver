//
//  DataService.h
//  tserver
//
//  Created by baowei on 2022/12/17.
//  Copyright © 2022 com. All rights reserved.
//

#ifndef TSERVER_DATASERVICE_H
#define TSERVER_DATASERVICE_H

#include "system/ServiceFactory.h"
#include "DataContext.h"

using namespace System;

class DataService : public IService {
public:
    DataService();

    ~DataService() override;

    bool initialize();

    bool unInitialize();

    FetchResult getLabelValues(const JsonNode &request, JsonNode &response);

    FetchResult getTableValues(const JsonNode &request, JsonNode &response);

    bool getType(const StringMap &request, StringMap &response);

    bool setType(const StringMap &request, StringMap &response);

private:
    String type() const;

    IDataProvider *createProvider(const String &type);

    IDataProvider *provider() const;

private:
    static bool isTypeValid(const String &type);

private:
    IDataProvider *_provider;

    PList<IDataProvider> _deletedProviders;
};


#endif //TSERVER_DATASERVICE_H
