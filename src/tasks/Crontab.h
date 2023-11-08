//
// Created by baowei on 2023/8/21.
//

#ifndef TSERVER_CRONTAB_H
#define TSERVER_CRONTAB_H

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

class Execution;
class Schedule;

class Crontab : public IEvaluation<Crontab>, public IEquatable<Crontab> {
public:
    String name;
    bool enable;
    StringArray resultNames;

    Crontab();

    explicit Crontab(const String &name, Schedule *schedule, Execution *execution, const StringArray &resultNames);

    Crontab(const Crontab &other);

    ~Crontab() override;

    bool equals(const Crontab &other) const override;

    void evaluates(const Crontab &other) override;

    Crontab &operator=(const Crontab &other);

    const Schedule *schedule() const;

    const Execution *execution() const;

    bool isTimeUp();

    bool execute();

    DataRow toDataRow(const DataTable &table) const;

    JsonNode toJsonNode() const;

    void removeFile();

    bool isValid() const;

    bool hasResult() const;

    bool findName(const String &n) const;

    void updateYmlProperties(YmlNode::Properties &properties, int pos) const;

    static void removeYmlProperties(YmlNode::Properties &properties, int pos);

    String toInsertSqlStr(const String &prefix) const;

    String toReplaceSqlStr(const String &prefix) const;

    void addParam(const String &param);

public:
    static bool parse(const StringMap &request, Crontab &crontab);

    static bool parse(const YmlNode::Properties &properties, int pos, Crontab &crontab);

    static bool parse(const DataRow &row, Crontab &crontab);

    static String toSelectSqlStr(const String &prefix, const String &name);

    static String toSelectSqlStr(const String &prefix, int offset);

    static String toSelectSqlStr(const String &prefix, const SqlSelectFilter &filter);

    static String toCountSqlStr(const String &prefix, const SqlSelectFilter &filter);

    static String toDeleteSqlStr(const String &prefix, const String &taskName);

private:
    static bool parseSchedule(const String &str, Schedule *&schedule);

    static bool parseExecution(const String &str, Execution *&execution);

    static bool parseResult(const String &str, StringArray &results);

    static String getTableName(const String &prefix, const String &tableName);

private:
    Schedule *_schedule;
    Execution *_execution;
};

typedef List<Crontab> Crontabs;

#endif //TSERVER_CRONTAB_H
