//
//  TaskCache.cpp
//  tserver
//
//  Created by baowei on 2023/4/7.
//  Copyright (c) 2023 com. All rights reserved.
//

#include "TaskCache.h"

TaskCacheMemory::TaskCacheMemory() {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    factory->addService<ITaskCache>(this);
}

TaskCacheMemory::~TaskCacheMemory() {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    factory->removeService<ITaskCache>();
}

String TaskCacheMemory::getValue(const String &key) {
    Locker locker(&_cache);
    return _cache[key];
}

bool TaskCacheMemory::setValue(const String &key, const String &value) {
    Locker locker(&_cache);
    return _cache.set(key, value);
}