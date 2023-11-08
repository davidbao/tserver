//
//  Execution.h
//  tserver
//
//  Created by baowei on 2023/8/21.
//  Copyright (c) 2023 com. All rights reserved.
//

#ifndef TSERVER_EXECUTION_H
#define TSERVER_EXECUTION_H

#include "data/PList.h"
#include "json/JsonNode.h"
#include "http/HttpContent.h"
#include "database/DataTable.h"
#include "database/SqlSelectFilter.h"
#include "system/ServiceFactory.h"
#include "data/Dictionary.h"
#include "../Style.h"
#include <cassert>

using namespace Data;
using namespace Http;
using namespace Database;
using namespace System;

class Execution : public IEvaluation<Execution>, public IEquatable<Execution>, public ICloneable<Execution> {
public:
    enum Result {
        Succeed = 0,
        FailedToStartProcess,
        NotFound,
        Timeout,
        FailedToExecuteSql,
    };

    Execution();

    explicit Execution(bool sync, const TimeSpan &timeout, const String &param = String::Empty);

    bool equals(const Execution &other) const override;

    void evaluates(const Execution &other) override;

    virtual String type() const = 0;

    bool sync() const;

    const TimeSpan &timeout() const;

    virtual Result execute() = 0;

    virtual JsonNode toJsonNode() const = 0;

    virtual void removeFile();

    virtual bool checkFile(const String &md5) const;

    virtual String currentFile() const = 0;

    static String currentPath();

    String toString() const;

    const String &param() const;

    void setParam(const String &param);

protected:
    bool _sync;
    TimeSpan _timeout;
    String _param;
};

class AppExecution : public Execution {
public:
    AppExecution() = default;

    explicit AppExecution(bool sync, const TimeSpan &timeout,
                          const String &app, const String &param);

    ~AppExecution() override = default;

    bool equals(const Execution &other) const override;

    void evaluates(const Execution &other) override;

    Execution *clone() const override;

    String type() const override;

    const String &app() const;

    Result execute() override;

    JsonNode toJsonNode() const override;

protected:
    String currentFile() const override;

private:
    String _app;
};

class SqlExecution : public Execution {
public:
    SqlExecution() = default;

    explicit SqlExecution(bool sync, const TimeSpan &timeout,
                          const String &text, bool sql);

    ~SqlExecution() override = default;

    bool equals(const Execution &other) const override;

    void evaluates(const Execution &other) override;

    Execution *clone() const override;

    String type() const override;

    const String &sql() const;

    const String &fileName() const;

    bool isFile() const;

    Result execute() override;

    JsonNode toJsonNode() const override;

protected:
    String currentFile() const override;

private:
    String _sql;
    String _fileName;
};

class PythonExecution : public Execution {
public:
    PythonExecution() = default;

    explicit PythonExecution(bool sync, const TimeSpan &timeout,
                             const String &script);

    explicit PythonExecution(bool sync, const TimeSpan &timeout,
                             const String &fileName, const String &param);

    ~PythonExecution() override = default;

    bool equals(const Execution &other) const override;

    void evaluates(const Execution &other) override;

    Execution *clone() const override;

    String type() const override;

    const String &script() const;

    const String &fileName() const;

    bool isFile() const;

    Result execute() override;

    JsonNode toJsonNode() const override;

protected:
    String currentFile() const override;

private:
    String _script;

    String _fileName;
};

#endif //TSERVER_EXECUTION_H
