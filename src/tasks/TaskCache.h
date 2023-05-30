//
//  TaskCache.h
//  tserver
//
//  Created by baowei on 2023/4/7.
//  Copyright (c) 2023 com. All rights reserved.
//

#ifndef TSERVER_TASKCACHE_H
#define TSERVER_TASKCACHE_H

#include "TaskContext.h"

class TaskCacheMemory : public ITaskCache {
public:
    TaskCacheMemory();

    ~TaskCacheMemory() override;

    String getValue(const String &key) override;

    bool setValue(const String &key, const String &value) override;

private:
    Dictionary<String, String> _cache;
};


#endif //TSERVER_TASKCACHE_H
