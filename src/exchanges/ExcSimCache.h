//
//  ExcSimCache.h
//  tserver
//
//  Created by baowei on 2023/4/3.
//  Copyright (c) 2023 com. All rights reserved.
//

#ifndef TSERVER_EXCSIMCACHE_H
#define TSERVER_EXCSIMCACHE_H

#include "ExcSimContext.h"

class ExcSimCacheMemory : public IExcSimCache {
public:
    ExcSimCacheMemory();

    ~ExcSimCacheMemory() override;

    String getValue(const String &key) override;

    bool setValue(const String &key, const String &value) override;

private:
    Dictionary<String, String> _cache;
};


#endif //TSERVER_EXCSIMCACHE_H
