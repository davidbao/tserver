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
#include "database/SqlConnection.h"

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

    // button
    bool getButton(const String &name, Button &button) override;

    bool getButtons(const SqlSelectFilter &filter, DataTable &table) override;

    bool addButton(const StringMap &request, StringMap &response) override;

    bool updateButton(const StringMap &request, StringMap &response) override;

    bool removeButton(const StringMap &request, StringMap &response) override;

private:
    String fileName() const;

    void rescan();

    bool saveFile(const YmlNode::Properties &properties);

private:
    YmlNode::Properties _properties;

    DateTime _modifyTime;
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

    // button
    bool getButton(const String &name, Button &button) override;

    bool getButtons(const SqlSelectFilter &filter, DataTable &table) override;

    bool addButton(const StringMap &request, StringMap &response) override;

    bool updateButton(const StringMap &request, StringMap &response) override;

    bool removeButton(const StringMap &request, StringMap &response) override;

private:
    bool getLabelId(const String &name, uint64_t &id);

    bool containsLabel(const String &name);

    bool getTableId(const String &name, uint64_t &id);

    bool containsTable(const String &name);

    bool getButtonId(const String &name, uint64_t &id);

    bool containsButton(const String &name);

private:
    static String getTablePrefix();

    static String getTableName(const String &tableName);

private:
    SqlConnection *_connection;
};

#endif //TSERVER_EXCSIMSTORAGE_H
