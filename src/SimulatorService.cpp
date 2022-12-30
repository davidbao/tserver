//
//  SimulatorService.cpp
//  tserver
//
//  Created by baowei on 2022/12/17.
//  Copyright © 2022 com. All rights reserved.
//

#include "SimulatorService.h"
#include "system/ServiceFactory.h"
#include "configuration/ConfigService.h"
#include "thread/TickTimeout.h"
#include "system/Random.h"

using namespace Config;

SimulatorService::Tag::Tag(const String &registerStr) : registerStr(registerStr), value(Variant::Null) {
}

SimulatorService::Tag::Tag(const Tag &tag) {
    Tag::evaluates(tag);
}

bool SimulatorService::Tag::equals(const Tag &other) const {
    return this->name == other.name &&
           this->registerStr == other.registerStr &&
           this->value == other.value;
}

void SimulatorService::Tag::evaluates(const Tag &other) {
    this->name = other.name;
    this->registerStr = other.registerStr;
    this->value = other.value;
}

void SimulatorService::Tag::runOnce(const Label *label) {
    if (String::equals(registerStr, "increase", true)) {
        if (value.isNullType()) {
            value = Variant(Variant::Float64);
        }
        if (value.isNullValue()) {
            value = label->minValue;
        } else {
            double v = value;
            v += label->step;
            if (v > label->maxValue) {
                v = label->minValue;
            }
            value = v;
        }
    } else if (String::equals(registerStr, "decrease", true)) {
        if (value.isNullType()) {
            value = Variant(Variant::Float64);
        }
        if (value.isNullValue()) {
            value = label->maxValue;
        } else {
            double v = value;
            v -= label->step;
            if (v < label->minValue) {
                v = label->maxValue;
            }
            value = v;
        }
    } else if (String::equals(registerStr, "random", true)) {
        if (value.isNullType()) {
            value = Variant(Variant::Float64);
        }
        value = Random::getRandValue(label->minValue, label->maxValue);
    } else if (String::equals(registerStr, "uuid", true)) {
        if (value.isNullType()) {
            value = Variant(Variant::Text);
        }
        value = Uuid::generate().toString();
    } else {
        double v;
        if (Double::parse(registerStr, v)) {
            if (value.isNullType()) {
                value = Variant(Variant::Float64);
            }
            value = v;
        } else {
            if (value.isNullType()) {
                value = Variant(Variant::Text);
            }
            value = registerStr;
        }
    }
}

SimulatorService::Label::Label() : minValue(0), maxValue(100), step(1), _tick(0) {
}

SimulatorService::Label::Label(const Label &label) {
    Label::evaluates(label);
}

bool SimulatorService::Label::equals(const Label &other) const {
    return this->name == other.name &&
           this->minValue == other.minValue &&
           this->maxValue == other.maxValue &&
           this->step == other.step &&
           this->interval == other.interval &&
           this->tags == other.tags;
}

void SimulatorService::Label::evaluates(const Label &other) {
    this->name = other.name;
    this->minValue = other.minValue;
    this->maxValue = other.maxValue;
    this->step = other.step;
    this->interval = other.interval;
    this->tags = other.tags;
}

bool SimulatorService::Label::isTimeUp() {
    if (_tick == 0) {
        _tick = TickTimeout::getCurrentTickCount();
    }
    if (TickTimeout::isTimeout(_tick, interval)) {
        _tick = TickTimeout::getCurrentTickCount();
        return true;
    }
    return false;
}

SimulatorService::Column::Column() {
}

SimulatorService::Column::Column(const String &name, const String &registerStr, const String &style) {
    this->name = name;
    this->registerStr = registerStr;
    this->style = style;
    if (String::equals(registerStr, "time", true)) {
        this->value = Variant(ValueTypes::Text);
    } else if (String::equals(registerStr, "increase", true) ||
               String::equals(registerStr, "decrease", true) ||
               String::equals(registerStr, "random", true)) {
        this->value = Variant(ValueTypes::Float64);
    } else {
        double v;
        if (Double::parse(registerStr, v)) {
            this->value = Variant(ValueTypes::Float64);
        } else {
            this->value = Variant(ValueTypes::Text);
        }
    }
}

SimulatorService::Column::Column(const Column &Column) {
    Column::evaluates(Column);
}

bool SimulatorService::Column::equals(const Column &other) const {
    return this->name == other.name &&
           this->registerStr == other.registerStr &&
           this->style == other.style &&
           this->value == other.value;
}

void SimulatorService::Column::evaluates(const Column &other) {
    this->name = other.name;
    this->registerStr = other.registerStr;
    this->style = other.style;
    this->value = other.value;
}

String SimulatorService::Column::getValue(const Table *table, const SqlSelectFilter &filter, int row) const {
    String result;
    if (String::equals(registerStr, "increase", true)) {
        double v = table->minValue;
        v += table->step * row;
        result = Double(v).toString();
    } else if (String::equals(registerStr, "decrease", true)) {
        double v = table->maxValue;
        v -= table->step * row;
        result = Double(v).toString();
    } else if (String::equals(registerStr, "random", true)) {
        double v = Random::getRandValue(table->minValue, table->maxValue);
        result = Double(v).toString();
    } else if (String::equals(registerStr, "time", true)) {
        if (!style.isNullOrEmpty()) {
            DateTime minValue, maxValue;
            TimeSpan step;
            String format;
            StringArray texts;
            StringArray::parse(style, texts, ';');
            for (size_t i = 0; i < texts.count(); ++i) {
                char nameStr[255] = {0}, valueStr[255] = {0};
                int rValue = sscanf(texts[i].trim().c_str(), "%[a-z|A-Z|0-9]:%s", nameStr, valueStr);
                if (rValue >= 2) {
                    String name = String::trim(nameStr, '\'', '"', ' ');
                    String value = String::trim(valueStr, '\'', '"', ' ');
                    if (String::equals(name, "range", true)) {
                        if (String::equals(value, "today", true)) {
                            DateTime now = DateTime::now();
                            minValue = now.date();
                            maxValue = now.date().addDays(1);
//                            printf("today minValue: %s, maxValue: %s\n", minValue.toString().c_str(), maxValue.toString().c_str());
                        } else if (String::equals(value, "yesterday", true)) {
                            DateTime now = DateTime::now();
                            minValue = now.date().addDays(-1);
                            maxValue = now.date();
//                            printf("yesterday minValue: %s, maxValue: %s\n", minValue.toString().c_str(), maxValue.toString().c_str());
                        } else if (String::equals(value, "thismonth", true)) {
                            DateTime now = DateTime::now();
                            minValue = DateTime(now.year(), now.month(), 1);
                            maxValue = minValue.addMonths(1);
//                            printf("yesterday minValue: %s, maxValue: %s\n", minValue.toString().c_str(), maxValue.toString().c_str());
                        } else {
                            StringArray ranges;
                            StringArray::parse(value, ranges, ';');
                            if (ranges.count() == 2) {
                                DateTime::parse(ranges[0], minValue);
                                DateTime::parse(ranges[2], maxValue);
                            }
                        }
                    } else if (String::equals(name, "step", true)) {
                        TimeSpan::parse(value, step);
//                        printf("value: %s, step: %s\n", value.c_str(), step.toString().c_str());
                    } else if (String::equals(name, "format", true)) {
                        format = value;
                    }
                }
            }
            if (!(minValue == DateTime::MinValue && maxValue == DateTime::MaxValue &&
                  step == TimeSpan::Zero && !format.isNullOrEmpty())) {
                DateTime v = minValue.addTicks(step.ticks() * row);
                result = v.toString(format);
            }
        }/* else {
            DateTime timeFrom;
            DateTime::parse(filter.getValue(String::format("%s.from", name.c_str())), timeFrom);
            DateTime timeTo;
            DateTime::parse(filter.getValue(String::format("%s.to", name.c_str())), timeTo);
            if (timeTo > timeFrom) {
                TimeSpan interval = TimeSpan::fromTicks((timeTo.ticks() - timeFrom.ticks()) / table->rowCount);
                DateTime v = timeFrom.addTicks(interval.ticks() * row);
                result = v.toString();
            }
        }*/
    } else if (String::equals(registerStr, "array", true)) {
        StringArray texts;
        StringArray::parse(style, texts, ';');
        if (row < texts.count()) {
            result = String::trim(texts[row], '\'', '"', ' ');
        }
    } else if (String::equals(registerStr, "uuid", true)) {
        result = Uuid::generate().toString();
    } else {
        result = registerStr;
    }
    return result;
}

SimulatorService::Table::Table() : minValue(0), maxValue(100), step(1), rowCount(0) {
}

SimulatorService::Table::Table(const Table &Table) {
    Table::evaluates(Table);
}

bool SimulatorService::Table::equals(const Table &other) const {
    return this->name == other.name &&
           this->minValue == other.minValue &&
           this->maxValue == other.maxValue &&
           this->step == other.step &&
           this->rowCount == other.rowCount &&
           this->columns == other.columns;
}

void SimulatorService::Table::evaluates(const Table &other) {
    this->name = other.name;
    this->minValue = other.minValue;
    this->maxValue = other.maxValue;
    this->step = other.step;
    this->rowCount = other.rowCount;
    this->columns = other.columns;
}

SimulatorService::SimulatorService() : _timer(nullptr) {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    IConfigService *cs = factory->getService<IConfigService>();
    assert(cs);

    String type;
    cs->getProperty("summer.exchange.type", type);
    if (String::equals(type, "simulator", true)) {
        // It's a simulator, so initialize.
        initLabels();
        initTables();

        startSimulator();
    }
}

SimulatorService::~SimulatorService() {
    stopSimulator();
}

FetchResult SimulatorService::getLabelValues(const String &labelName, const StringArray &tagNames, StringMap &values) {
    Locker locker(&_labels);
    for (size_t i = 0; i < _labels.count(); i++) {
        const Label &label = _labels[i];
        if (labelName == label.name) {
            for (size_t j = 0; j < label.tags.count(); j++) {
                const Tag &tag = label.tags[j];
                if (tagNames.contains(tag.name)) {
                    values.add(tag.name, tag.value);
                }
            }
            break;
        }
    }
    return FetchResult::Succeed;
}

FetchResult
SimulatorService::getTableValues(const String &tableName, const SqlSelectFilter &filter, DataTable &dataTable) {
    Locker locker(&_tables);
    for (size_t i = 0; i < _tables.count(); i++) {
        const Table &table = _tables[i];
        if (tableName == table.name) {
            // initial the total count of table.
            dataTable.setTotalCount(table.rowCount);
            // initial the columns of table.
            for (size_t j = 0; j < table.columns.count(); ++j) {
                const Column &col = table.columns[j];
                DataColumn column(col.name, col.value.typeStr());
                dataTable.addColumn(column);
            }

            // add rows.
            int start = table.rowCount < filter.pageSize() ? 0 : filter.offset();
            int next = start + filter.pageSize();
            int end = table.rowCount < filter.pageSize() ? table.rowCount : (next < table.rowCount ? next
                                                                                                   : table.rowCount);
            for (int row = start; row < end; ++row) {
                DataRow dataRow;
                for (size_t j = 0; j < table.columns.count(); ++j) {
                    const Column &col = table.columns[j];
                    String value = col.getValue(&table, filter, row);
                    dataRow.addCell(DataCell(dataTable.columns()[col.name], value));
                }
                dataTable.addRow(dataRow);
            }
            break;
        }
    }

    return FetchResult::Succeed;
}

void SimulatorService::initLabels() {
    Locker locker(&_labels);
    _labels.clear();

    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    IConfigService *cs = factory->getService<IConfigService>();
    assert(cs);

#define maxLabelCount 1000
#define labelPrefix "summer.exchange.simulator.labels[%d]."
    for (int i = 0; i < maxLabelCount; i++) {
        Label label;
        if (!cs->getProperty(String::format(labelPrefix "name", i), label.name)) {
            break;
        }
        String range;
        cs->getProperty(String::format(labelPrefix "range", i), range);
        StringArray texts;
        Convert::splitStr(range, texts, '-');
        if (texts.count() == 2) {
            double minValue = 0, maxValue = 100;
            if (Double::parse(texts[0], minValue) &&
                Double::parse(texts[1], maxValue) && maxValue > minValue) {
                label.minValue = minValue;
                label.maxValue = maxValue;
            }
        }
        cs->getProperty(String::format(labelPrefix "step", i), label.step);
        cs->getProperty(String::format(labelPrefix "interval", i), label.interval);
#define maxTagCount 1000
#define tagPrefix labelPrefix "tags[%d]."
        for (int j = 0; j < maxTagCount; j++) {
            Tag tag;
            if (!cs->getProperty(String::format(tagPrefix "name", i, j), tag.name)) {
                break;
            }
            cs->getProperty(String::format(tagPrefix "register", i, j), tag.registerStr);
            label.tags.add(tag);
        }
        _labels.add(label);
    }
}

void SimulatorService::initTables() {
    Locker locker(&_tables);
    _tables.clear();

    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    IConfigService *cs = factory->getService<IConfigService>();
    assert(cs);

#define maxTableCount 1000
#define tablePrefix "summer.exchange.simulator.tables[%d]."
    for (int i = 0; i < maxTableCount; i++) {
        Table table;
        if (!cs->getProperty(String::format(tablePrefix "name", i), table.name)) {
            break;
        }
        String range;
        cs->getProperty(String::format(tablePrefix "range", i), range);
        StringArray texts;
        Convert::splitStr(range, texts, '-');
        if (texts.count() == 2) {
            double minValue = 0, maxValue = 100;
            if (Double::parse(texts[0], minValue) &&
                Double::parse(texts[1], maxValue) && maxValue > minValue) {
                table.minValue = minValue;
                table.maxValue = maxValue;
            }
        }
        cs->getProperty(String::format(tablePrefix "step", i), table.step);
        cs->getProperty(String::format(tablePrefix "rowCount", i), table.rowCount);
#define maxColumnCount 100
#define columnPrefix tablePrefix "columns[%d]."
        for (int j = 0; j < maxColumnCount; j++) {
            String name, registerStr, style;
            if (!cs->getProperty(String::format(columnPrefix "name", i, j), name)) {
                break;
            }
            cs->getProperty(String::format(columnPrefix "register", i, j), registerStr);
            cs->getProperty(String::format(columnPrefix "style", i, j), style);
            Column column(name, registerStr, style);
            table.columns.add(column);
        }
        _tables.add(table);
    }
}

void SimulatorService::startSimulator() {
    if (_timer != nullptr) {
        return;
    }

    static const TimeSpan interval = TimeSpan::fromSeconds(1);
    _timer = new Timer("simulator.timer",
                       TimerCallback<SimulatorService>(this, &SimulatorService::labelTimeUp),
                       interval, interval);
}

void SimulatorService::stopSimulator() {
    delete _timer;
    _timer = nullptr;
}

void SimulatorService::labelTimeUp() {
    Locker locker(&_labels);
    for (size_t i = 0; i < _labels.count(); i++) {
        Label &group = _labels[i];
        if (group.isTimeUp()) {
            for (size_t j = 0; j < group.tags.count(); j++) {
                Tag &tag = group.tags[j];
                tag.runOnce(&group);
//                printf("tag('%s.%s') value: %s\n", group.name.c_str(), tag.name.c_str(), tag.value.valueStr().c_str());
            }
        }
    }
}
