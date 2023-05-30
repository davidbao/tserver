//
//  ExcSimCache.cpp
//  tserver
//
//  Created by baowei on 2023/4/3.
//  Copyright (c) 2023 com. All rights reserved.
//

#include "ExcSimCache.h"

ExcSimCacheMemory::ExcSimCacheMemory() {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    factory->addService<IExcSimCache>(this);
}

ExcSimCacheMemory::~ExcSimCacheMemory() {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    factory->removeService<IExcSimCache>();
}

String ExcSimCacheMemory::getValue(const String &key) {
    Locker locker(&_cache);
    return _cache[key];
}

bool ExcSimCacheMemory::setValue(const String &key, const String &value) {
    Locker locker(&_cache);
    return _cache.set(key, value);
}