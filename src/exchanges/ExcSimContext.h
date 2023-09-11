//
//  ExcSimContext.h
//  tserver
//
//  Created by baowei on 2023/3/31.
//  Copyright (c) 2023 com. All rights reserved.
//

#ifndef TSERVER_EXCSIMCONTEXT_H
#define TSERVER_EXCSIMCONTEXT_H

#include "data/PList.h"
#include "system/ServiceFactory.h"
#include "database/DataTable.h"
#include "database/SqlSelectFilter.h"
#include "../Style.h"
#include "ExcContext.h"
#include <cassert>

using namespace Data;
using namespace Database;
using namespace System;

// for application.yml
#define SimulatorPrefix ExcPrefix "simulator."
#define SimDbPrefix SimulatorPrefix "database."

// for yml file.
#define MaxLabelCount 1000
#define LabelPrefix "labels[%d]."
#define MaxTagCount 1000
#define TagPrefix LabelPrefix "tags[%d]."

#define MaxTableCount 1000
#define TablePrefix "tables[%d]."
#define MaxColumnCount 100
#define ColumnPrefix TablePrefix "columns[%d]."
#define MaxRowCount 1000
#define RowPrefix TablePrefix "rows[%d]."

#define MaxButtonCount 1000
#define ButtonPrefix "buttons[%d]."
#define MaxResultCount 1000
#define ResultPrefix ButtonPrefix "results[%d]."

// for database.
#define LabelTableName "sim_label"
#define TagTableName "sim_tag"

#define TableTableName "sim_table"
#define ColumnTableName "sim_column"

#define ButtonTableName "sim_button"
#define ResultTableName "sim_result"

class Element;

class Item {
public:
    String name;
    String registerStr;
    Style style;

    Item() = default;

    explicit Item(const String &name, const String &registerStr, const Style &style);

    virtual ~Item() = default;

    void evaluates(const Item &other);

    bool equals(const Item &other) const;

protected:
    static bool parseRange(const Style &style, double &minValue, double &maxValue, double &step);

protected:
    Variant _value;
};

class Element {
public:
    String name;
    bool enable;
    double minValue;
    double maxValue;
    double step;

    Element();

    explicit Element(const String &name, double minValue, double maxValue, double step);

    virtual ~Element() = default;

    virtual DataRow toDataRow(const DataTable &table) const = 0;

    virtual String toValuesStr() const = 0;

    virtual bool findItemName(const StringArray &itemNames) const = 0;

    virtual JsonNode toJsonNode() const = 0;

    void evaluates(const Element &other);

    bool equals(const Element &other) const;

    bool findName(const String &n) const;

    String toRangeStr() const;

    bool findItemName(const String &itemNames) const;

public:
    static bool parseRange(const String &str, double &minValue, double &maxValue);
};

class Label;

class Tag : public Item, public IEvaluation<Tag>, public IEquatable<Tag> {
public:
    Tag() = default;

    explicit Tag(const String &name, const String &registerStr, const Style &style);

    Tag(const Tag &other);

    bool equals(const Tag &other) const override;

    void evaluates(const Tag &other) override;

    Variant getValue(const Element *element, const StringMap &filter) const;

    DbType type() const;
};

class Tags : public List<Tag> {
public:
    Tags();

    ~Tags() override;

    bool contains(const StringArray &names) const;

    bool atByName(const String &name, Tag &tag) const;
};

class Label : public Element, public IEvaluation<Label>, public IEquatable<Label> {
public:
    using Element::findItemName;

    Tags tags;

    Label() = default;

    explicit Label(const String &name, double minValue, double maxValue, double step);

    Label(const Label &other);

    DataRow toDataRow(const DataTable &table) const override;

    String toValuesStr() const override;

    bool findItemName(const StringArray &itemNames) const override;

    JsonNode toJsonNode() const override;

    bool equals(const Label &other) const override;

    void evaluates(const Label &other) override;

    void updateYmlProperties(YmlNode::Properties &properties, int pos) const;

    void removeYmlProperties(YmlNode::Properties &properties, int pos) const;

    String toInsertSqlStr(const String &prefix) const;

    String toReplaceSqlStr(const String &prefix) const;

    bool getTags(const StringArray &tagNames, Tags &values) const;

public:
    static bool parse(const StringMap &request, Label &label);

    static bool parse(const YmlNode::Properties &properties, int pos, Label &label);

    static bool parse(const DataRow &labelRow, const DataRows &tagRows, Label &label);

    static bool parse(const DataRow &row, Label &label);

    static String toSelectSqlStr(const String &prefix, const String &name);

    static String toSelectSqlStr(const String &prefix, const SqlSelectFilter &filter);

    static String toCountSqlStr(const String &prefix, const SqlSelectFilter &filter);

    static String toDeleteSqlStr(const String &prefix, const String &labelName);

private:
    static bool parseTags(const String &str, Tags &tags);

    static String getTableName(const String &prefix, const String &tableName);
};

typedef List<Label> Labels;

class Table;

class Column : public Item, public IEvaluation<Column>, public IEquatable<Column> {
public:
    Column() = default;

    Column(const String &name, const String &registerStr, const Style &style);

    Column(const Column &Column);

    Variant getCellValue(const Table *table, const SqlSelectFilter &filter, int row, int col) const;

    bool equals(const Column &other) const override;

    void evaluates(const Column &other) override;

    DbType type() const;
};

class Columns : public List<Column> {
public:
    Columns();

    ~Columns() override;

    bool contains(const StringArray &names) const;

    bool atByName(const String &name, Column &column) const;
};

class Row : public IEvaluation<Row>, public IEquatable<Row> {
public:
    Row() = default;

    explicit Row(const String &rowStr);

    explicit Row(const StringArray &cells);

    Row(const Row &row);

    bool equals(const Row &other) const override;

    void evaluates(const Row &other) override;

    const StringArray &cells() const;

    String toString() const;

private:
    StringArray _cells;
};

typedef List<Row> Rows;

class Table : public Element, public IEvaluation<Table>, public IEquatable<Table> {
public:
    using Element::findItemName;

    Columns columns;
    Rows rows;

    Table();

    Table(const Table &other);

    bool equals(const Table &other) const override;

    void evaluates(const Table &other) override;

    DataRow toDataRow(const DataTable &table) const override;

    String toValuesStr() const override;

    bool findItemName(const StringArray &itemNames) const override;

    JsonNode toJsonNode() const override;

    int rowCount() const;

    String toColumnsStr() const;

    String toRowsStr() const;

    bool getColumns(const StringArray &colNames, Columns &cols) const;

    void updateYmlProperties(YmlNode::Properties &properties, int pos) const;

    void removeYmlProperties(YmlNode::Properties &properties, int pos) const;

    String toInsertSqlStr(const String &prefix) const;

    String toReplaceSqlStr(const String &prefix) const;

public:
    static bool parse(const StringMap &request, Table &table);

    static bool parse(const YmlNode::Properties &properties, int pos, Table &table);

    static bool parse(const DataRow &row, Table &table);

    static String toSelectSqlStr(const String &prefix, const String &name);

    static String toSelectSqlStr(const String &prefix, const SqlSelectFilter &filter);

    static String toCountSqlStr(const String &prefix, const SqlSelectFilter &filter);

    static String toDeleteSqlStr(const String &prefix, const String &tableName);

private:
    static bool parseColumns(const String &str, Columns &columns);

    static bool parseRows(const String &str, Rows &rows);

    static String getTableName(const String &prefix, const String &tableName);

private:
    int _rowCount;
};

typedef List<Table> Tables;

typedef Tag Result;
typedef Tags Results;

class Button : public Element, public IEvaluation<Button>, public IEquatable<Button> {
public:
    using Element::findItemName;

    Results results;

    Button() = default;

    explicit Button(const String &name, double minValue, double maxValue, double step);

    Button(const Button &other);

    DataRow toDataRow(const DataTable &table) const override;

    String toValuesStr() const override;

    bool findItemName(const StringArray &itemNames) const override;

    JsonNode toJsonNode() const override;

    bool equals(const Button &other) const override;

    void evaluates(const Button &other) override;

    void updateYmlProperties(YmlNode::Properties &properties, int pos) const;

    void removeYmlProperties(YmlNode::Properties &properties, int pos) const;

    String toInsertSqlStr(const String &prefix) const;

    String toReplaceSqlStr(const String &prefix) const;

    bool getResults(const StringArray &resultNames, Results &values) const;

public:
    static bool parse(const StringMap &request, Button &button);

    static bool parse(const YmlNode::Properties &properties, int pos, Button &button);

    static bool parse(const DataRow &buttonRow, const DataRows &resultRows, Button &button);

    static bool parse(const DataRow &row, Button &button);

    static String toSelectSqlStr(const String &prefix, const String &name);

    static String toSelectSqlStr(const String &prefix, const SqlSelectFilter &filter);

    static String toCountSqlStr(const String &prefix, const SqlSelectFilter &filter);

    static String toDeleteSqlStr(const String &prefix, const String &buttonName);

private:
    static bool parseResults(const String &str, Results &results);

    static String getTableName(const String &prefix, const String &tableName);
};

typedef List<Button> Buttons;

class IExcSimCache : public IService {
public:
    IExcSimCache() = default;

    virtual String getValue(const String &key) = 0;

    virtual bool setValue(const String &key, const String &value) = 0;
};

class IExcSimStorage {
public:
    IExcSimStorage() = default;

    virtual ~IExcSimStorage() = default;

    virtual bool load() = 0;

    // label
    virtual bool getLabel(const String &name, Label &label) = 0;

    virtual bool getLabels(const SqlSelectFilter &filter, DataTable &table) = 0;

    virtual bool addLabel(const StringMap &request, StringMap &response) = 0;

    virtual bool updateLabel(const StringMap &request, StringMap &response) = 0;

    virtual bool removeLabel(const StringMap &request, StringMap &response) = 0;

    // table
    virtual bool getTable(const String &name, Table &table) = 0;

    virtual bool getTables(const SqlSelectFilter &filter, DataTable &table) = 0;

    virtual bool addTable(const StringMap &request, StringMap &response) = 0;

    virtual bool updateTable(const StringMap &request, StringMap &response) = 0;

    virtual bool removeTable(const StringMap &request, StringMap &response) = 0;

    // button
    virtual bool getButton(const String &name, Button &Button) = 0;

    virtual bool getButtons(const SqlSelectFilter &filter, DataTable &table) = 0;

    virtual bool addButton(const StringMap &request, StringMap &response) = 0;

    virtual bool updateButton(const StringMap &request, StringMap &response) = 0;

    virtual bool removeButton(const StringMap &request, StringMap &response) = 0;
};

#endif //TSERVER_EXCSIMCONTEXT_H
