//
//  ExcSimStorage.h
//  tserver
//
//  Created by baowei on 2023/4/3.
//  Copyright (c) 2023 com. All rights reserved.
//

#ifndef TSERVER_EXCSIMSTORAGE_H
#define TSERVER_EXCSIMSTORAGE_H

#include "ExcSimContext.h"
#include "database/DbClient.h"

using namespace Database;

class ExcSimFile : public IExcSimStorage {
public:
    ExcSimFile() = default;

    bool load() override;

    // label
    bool getLabel(const String &name, Label &label) override;

    bool getLabels(const SqlSelectFilter &filter, DataTable &table) override;

    bool addLabel(const StringMap &request, StringMap &response) override;

    bool updateLabel(const StringMap &request, StringMap &response) override;

    bool removeLabel(const StringMap &request, StringMap &response) override;

    // tabel
    bool getTable(const String &name, Table &table) override;

    bool getTables(const SqlSelectFilter &filter, DataTable &table) override;

    bool addTable(const StringMap &request, StringMap &response) override;

    bool updateTable(const StringMap &request, StringMap &response) override;

    bool removeTable(const StringMap &request, StringMap &response) override;

private:
    static bool saveFile(const YmlNode::Properties &properties);

private:
    YmlNode::Properties _properties;
};

class ExcSimDatabase : public IExcSimStorage {
public:
    ExcSimDatabase();

    ~ExcSimDatabase() override;

    bool load() override;

    // label
    bool getLabel(const String &name, Label &label) override;

    bool getLabels(const SqlSelectFilter &filter, DataTable &table) override;

    bool addLabel(const StringMap &request, StringMap &response) override;

    bool updateLabel(const StringMap &request, StringMap &response) override;

    bool removeLabel(const StringMap &request, StringMap &response) override;

    // tabel
    bool getTable(const String &name, Table &table) override;

    bool getTables(const SqlSelectFilter &filter, DataTable &table) override;

    bool addTable(const StringMap &request, StringMap &response) override;

    bool updateTable(const StringMap &request, StringMap &response) override;

    bool removeTable(const StringMap &request, StringMap &response) override;

private:
    bool getLabelId(const String &name, uint64_t &id);

    bool containsLabel(const String &name);

    bool getTableId(const String &name, uint64_t &id);

    bool containsTable(const String &name);

private:
    static String getTablePrefix();

    static String getTableName(const String &tableName);

private:
    DbClient *_dbClient;
};

#endif //TSERVER_EXCSIMSTORAGE_H
