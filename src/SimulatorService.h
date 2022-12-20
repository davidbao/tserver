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

using namespace Common;

class SimulatorService : public IDataProvider {
public:
    class Label;

    class Tag : public IEvaluation<Tag>, public IEquatable<Tag> {
    public:
        String name;
        String registerStr;
        Variant value;

        Tag(const String &registerStr = String::Empty);

        Tag(const Tag &tag);

        bool equals(const Tag &other) const override;

        void evaluates(const Tag &other) override;

        void runOnce(const Label *label);
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

    private:
        uint32_t _tick;
    };

    typedef List<Label> Labels;

    class Table;

    class Column : public IEvaluation<Column>, public IEquatable<Column> {
    public:
        String name;
        String registerStr;
        Variant value;

        Column();

        Column(const String &name, const String &registerStr);

        Column(const Column &Column);

        bool equals(const Column &other) const override;

        void evaluates(const Column &other) override;

        String getValue(const Table *table, const SqlSelectFilter &filter, int row) const;
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
    };

    typedef List<Table> Tables;

    SimulatorService();

    ~SimulatorService() override;

    FetchResult getLabelValues(const String &labelName, const StringArray &tagNames, StringMap &values) override;

    FetchResult getTableValues(const String &tableName, const SqlSelectFilter &filter, DataTable &dataTable) override;

private:
    void initLabels();

    void initTables();

    void startSimulator();

    void stopSimulator();

    void labelTimeUp();

private:
    Labels _labels;
    Timer *_timer;

    Tables _tables;
};


#endif //TSERVER_SIMULATORSERVICE_H
