//
//  SimulatorService.h
//  tserver
//
//  Created by baowei on 2022/12/17.
//  Copyright © 2022 com. All rights reserved.
//

#ifndef TSERVER_SIMULATORSERVICE_H
#define TSERVER_SIMULATORSERVICE_H

#include "DataContext.h"
#include "data/Variant.h"
#include "thread/Timer.h"
#include "system/ServiceFactory.h"
#include "configuration/ConfigService.h"

using namespace Data;
using namespace System;
using namespace Config;

class SimulatorService : public IDataProvider, public IConfigService {
public:
    class Label;

    class Tag : public IEvaluation<Tag>, public IEquatable<Tag> {
    public:
        String name;
        String registerStr;
        String style;
        Variant value;

        explicit Tag(const String &registerStr = String::Empty);

        Tag(const Tag &tag);

        bool equals(const Tag &other) const override;

        void evaluates(const Tag &other) override;

        void runOnce(const Label *label);

        String getValue(const SqlSelectFilter &filter) const;

    private:
        static bool parseDoubleStyle(const String &style, double &minValue, double &maxValue, double &step);

    private:
        double _oldValue;
    };

    typedef List<Tag> Tags;

    class Label : public IEvaluation<Label>, public IEquatable<Label> {
    public:
        String name;
        double minValue;
        double maxValue;
        double step;
        TimeSpan interval;
        Tags tags;

        Label();

        Label(const Label &label);

        bool equals(const Label &other) const override;

        void evaluates(const Label &other) override;

        bool isTimeUp();

        DataRow toDataRow(const DataTable &table) const;

        String toRangeStr() const;

        String toTagsStr() const;

        bool findName(const String &n) const;

        bool findTagName(const String &tagNames) const;

        JsonNode toJsonNode() const;

    public:
        static bool parseRange(const String &str, double &minValue, double &maxValue);

        static bool parseTags(const String &str, Tags &tags);

    private:
        uint32_t _tick;
    };

    typedef List<Label> Labels;

    class Table;

    class Column : public IEvaluation<Column>, public IEquatable<Column> {
    public:
        String name;
        String registerStr;
        String style;
        Variant value;

        Column();

        Column(const String &name, const String &registerStr, const String &style);

        Column(const Column &Column);

        bool equals(const Column &other) const override;

        void evaluates(const Column &other) override;

        String getValue(const Table *table, const SqlSelectFilter &filter, int row) const;

    public:
        static bool parseStyle(const String &str, String &style);

    private:
        static bool parseDoubleStyle(const String &style, double &minValue, double &maxValue, double &step);
    };

    typedef List<Column> Columns;

    class Table : public IEvaluation<Table>, public IEquatable<Table> {
    public:
        String name;
        double minValue;
        double maxValue;
        double step;
        int rowCount;
        Columns columns;

        Table();

        Table(const Table &table);

        bool equals(const Table &other) const override;

        void evaluates(const Table &other) override;

        DataRow toDataRow(const DataTable &table) const;

        String toRangeStr() const;

        String toColumnsStr() const;

        bool findName(const String &n) const;

        bool findColumnName(const String &columnNames) const;

        JsonNode toJsonNode() const;

    public:
        static bool parseRange(const String &str, double &minValue, double &maxValue);

        static bool parseColumns(const String &str, Columns &columns);
    };

    typedef List<Table> Tables;

    SimulatorService();

    ~SimulatorService() override;

    FetchResult getLabelValues(const String &labelName, const StringArray &tagNames,
                               const SqlSelectFilter &filter, StringMap &values) override;

    FetchResult getTableValues(const String &tableName, const StringArray &columns,
                               const SqlSelectFilter &filter, DataTable &dataTable) override;

    // Labels
    bool getLabels(const SqlSelectFilter &filter, DataTable &table);

    bool getLabel(const StringMap &request, StringMap &response);

    bool addLabel(const StringMap &request, StringMap &response);

    bool removeLabel(const StringMap &request, StringMap &response);

    bool updateLabel(const StringMap &request, StringMap &response);

    // Tables
    bool getTables(const SqlSelectFilter &filter, DataTable &table);

    bool getTable(const StringMap &request, StringMap &response);

    bool addTable(const StringMap &request, StringMap &response);

    bool removeTable(const StringMap &request, StringMap &response);

    bool updateTable(const StringMap &request, StringMap &response);

private:
    const YmlNode::Properties &properties() const override final;

    bool setProperty(const String &key, const String &value) override final;

    bool updateConfigFile(const YmlNode::Properties &properties) override final;

    bool loadSimulatorData();

    void initLabels();

    void initTables();

    void startSimulator();

    void stopSimulator();

    void labelTimeUp();

    bool addOrUpdateLabel(const StringMap &request, StringMap &response, int position = -1);

    bool addOrUpdateTable(const StringMap &request, StringMap &response, int position = -1);

private:
    static void updateYmlProperties(const Label &label, int position, YmlNode::Properties &properties);

    static void updateLabelYmlProperties(bool enable, int position, YmlNode::Properties &properties);

    static void updateYmlProperties(const Table &label, int position, YmlNode::Properties &properties);

    static void updateTableYmlProperties(bool enable, int position, YmlNode::Properties &properties);

private:
#define maxLabelCount 1000
#define labelPrefix "labels[%d]."
#define maxTagCount 1000
#define tagPrefix labelPrefix "tags[%d]."

#define maxTableCount 1000
#define tablePrefix "tables[%d]."
#define maxColumnCount 100
#define columnPrefix tablePrefix "columns[%d]."

    Labels _labels;
    Timer *_timer;

    Tables _tables;

    YmlNode::Properties _properties;
};


#endif //TSERVER_SIMULATORSERVICE_H
