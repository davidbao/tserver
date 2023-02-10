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
#include "system/Math.h"

using namespace Config;

SimulatorService::Tag::Tag(const String &registerStr) : registerStr(registerStr), value(Variant::Null),
                                                        _oldValue(Double::NaN) {
}

SimulatorService::Tag::Tag(const Tag &tag) : _oldValue(Double::NaN) {
    Tag::evaluates(tag);
}

bool SimulatorService::Tag::equals(const Tag &other) const {
    return this->name == other.name &&
           this->registerStr == other.registerStr &&
           this->style == other.style &&
           this->value == other.value;
}

void SimulatorService::Tag::evaluates(const Tag &other) {
    this->name = other.name;
    this->registerStr = other.registerStr;
    this->style = other.style;
    this->value = other.value;
}

void SimulatorService::Tag::runOnce(const Label *label) {
    double minValue, maxValue, step;
    if (!parseDoubleStyle(style, minValue, maxValue, step)) {
        minValue = label->minValue;
        maxValue = label->maxValue;
        step = label->step;
    }

    if (String::equals(registerStr, "increase", true)) {
        if (value.isNullType()) {
            value = Variant(Variant::Float64);
        }
        if (value.isNullValue()) {
            value = minValue;
        } else {
            double v = value;
            v += step;
            if (v > maxValue) {
                v = minValue;
            }
            value = v;
        }
    } else if (String::equals(registerStr, "decrease", true)) {
        if (value.isNullType()) {
            value = Variant(Variant::Float64);
        }
        if (value.isNullValue()) {
            value = maxValue;
        } else {
            double v = value;
            v -= step;
            if (v < minValue) {
                v = maxValue;
            }
            value = v;
        }
    } else if (String::equals(registerStr, "random", true)) {
        if (value.isNullType()) {
            value = Variant(Variant::Float64);
        }
        value = Random::getRandValue(minValue, maxValue);
    } else if (String::equals(registerStr, "sin", true)) {
        if (value.isNullType()) {
            value = Variant(Variant::Float64);
        }
        double oldValue = _oldValue;
        if (Double::isNaN(oldValue))
            oldValue = 0.0;
        else
            oldValue = oldValue + step;
        if (oldValue > 360.0)
            oldValue = 0.0;
        _oldValue = oldValue;
        double scope = maxValue - minValue + step;
        double v = minValue + scope * (Math::sin(oldValue) + 1.0) / 2.0;
        if (v < minValue)
            v = minValue;
        if (v > maxValue)
            v = maxValue;
        value = v;
    } else if (String::equals(registerStr, "cos", true)) {
        if (value.isNullType()) {
            value = Variant(Variant::Float64);
        }
        double oldValue = _oldValue;
        if (Double::isNaN(oldValue))
            oldValue = 0.0;
        else
            oldValue = oldValue + step;
        if (oldValue > 360.0)
            oldValue = 0.0;
        _oldValue = oldValue;
        double scope = maxValue - minValue + step;
        double v = minValue + scope * (Math::cos(oldValue) + 1.0) / 2.0;
        if (v < minValue)
            v = minValue;
        if (v > maxValue)
            v = maxValue;
        value = v;
    } else if (String::equals(registerStr, "onoff", true)) {
        if (value.isNullType()) {
            value = Variant(Variant::Bool);
        }
        value = !value;
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

String SimulatorService::Tag::getValue(const SqlSelectFilter &filter) const {
    if (String::equals(registerStr, "array", true)) {
        StringArray texts;
        StringArray::parse(style, texts, ';');
        if (texts.count() > 0) {
            const StringMap &values = filter.values();
            for (auto it = values.begin(); it != values.end(); ++it) {
                const String &k = it.key();
                const String &v = it.value();

                // equals
                if (k == name) {
                    for (size_t i = 0; i < texts.count(); i++) {
                        const String text = texts[i].trim();
                        if (text == v) {
                            return v;
                        }
                    }
                }
            }
        }
        return String::Empty;
    } else {
        return value.toString();
    }
}

bool SimulatorService::Tag::parseDoubleStyle(const String &style, double &minValue, double &maxValue, double &step) {
    if (style.isNullOrEmpty()) {
        return false;
    }

    bool result = true;
    StringArray texts;
    StringArray::parse(style, texts, ';');
    for (size_t i = 0; i < texts.count(); ++i) {
        char nameStr[255] = {0}, valueStr[255] = {0};
        int rValue = sscanf(texts[i].trim().c_str(), "%[a-z|A-Z|0-9]:%s", nameStr, valueStr);
        if (rValue >= 2) {
            String n = String::trim(nameStr, '\'', '"', ' ');
            String v = String::trim(valueStr, '\'', '"', ' ');
            if (String::equals(n, "range", true)) {
                StringArray ranges;
                StringArray::parse(v, ranges, '-');
                if (ranges.count() == 2) {
                    if (!Double::parse(ranges[0], minValue)) {
                        result = false;
                    }
                    if (!Double::parse(ranges[1], maxValue)) {
                        result = false;
                    }
                } else {
                    result = false;
                }
            } else if (String::equals(n, "step", true)) {
                if (!Double::parse(v, step)) {
                    result = false;
                }
            }
        } else {
            result = false;
        }
    }
    return result;
}

SimulatorService::Label::Label() : minValue(0), maxValue(100), step(1), _tick(0) {
}

SimulatorService::Label::Label(const Label &label) : minValue(0), maxValue(100), step(1), _tick(0) {
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

DataRow SimulatorService::Label::toDataRow(const DataTable &table) const {
    return DataRow({
                           DataCell(table.columns()["name"], name),
                           DataCell(table.columns()["range"], toRangeStr()),
                           DataCell(table.columns()["step"], step),
                           DataCell(table.columns()["interval"], interval),
                           DataCell(table.columns()["tags"], toTagsStr()),
                   });
}

String SimulatorService::Label::toRangeStr() const {
    return String::format("%s-%s", Double(minValue).toString().c_str(), Double(maxValue).toString().c_str());
}

String SimulatorService::Label::toTagsStr() const {
    JsonNode result("columns", JsonNode::TypeArray);
    for (size_t i = 0; i < tags.count(); ++i) {
        const Tag &tag = tags[i];
        JsonNode node;
        node.add(JsonNode("name", tag.name));
        node.add(JsonNode("register", tag.registerStr));
        node.add(JsonNode("style", tag.style));
        result.add(node);
    }
    return result.toString();
}

bool SimulatorService::Label::findName(const String &n) const {
    return n.isNullOrEmpty() || this->name.find(n) >= 0;
}

bool SimulatorService::Label::findTagName(const String &tagNames) const {
    if (tagNames.isNullOrEmpty()) {
        return true;
    }

    StringArray texts;
    StringArray::parse(tagNames, texts, ';');
    for (size_t i = 0; i < texts.count(); ++i) {
        const String &text = texts[i];
        for (size_t j = 0; j < tags.count(); ++j) {
            if (tags[j].name.find(text) >= 0) {
                return true;
            }
        }
    }
    return false;
}

JsonNode SimulatorService::Label::toJsonNode() const {
    JsonNode node;
    node.add(JsonNode("name", name));
    node.add(JsonNode("range", toRangeStr()));
    node.add(JsonNode("step", step));
    node.add(JsonNode("interval", interval));
    node.add(JsonNode("tags", toTagsStr()));
    return node;
}

bool SimulatorService::Label::parseRange(const String &str, double &minValue, double &maxValue) {
    StringArray texts;
    Convert::splitStr(str, texts, '-');
    if (texts.count() == 2) {
        double minV = 0, maxV = 100;
        if (Double::parse(texts[0], minV) &&
            Double::parse(texts[1], maxV) && maxV > minV) {
            minValue = minV;
            maxValue = maxV;
            return true;
        }
    }
    return false;
}

bool SimulatorService::Label::parseTags(const String &str, Tags &tags) {
    // str such like {"name":"tag1","register":"increase"};{"name":"tag2","register":"decrease"};{"name":"tag3","register":"random"}
    StringArray tagsTexts;
    Convert::splitStr(str, tagsTexts, ';');
    for (size_t i = 0; i < tagsTexts.count(); ++i) {
        JsonNode node;
        if (JsonNode::parse(tagsTexts[i], node)) {
            Tag tag;
            tag.name = node.getAttribute("name").trim();
            tag.registerStr = node.getAttribute("register").trim();
            tag.style = node.getAttribute("style").trim();
            tags.add(tag);
        }
    }
    return tags.count() > 0;
}

SimulatorService::Column::Column() = default;

SimulatorService::Column::Column(const String &name, const String &registerStr, const String &style) {
    this->name = name;
    this->registerStr = registerStr;
    this->style = style;
    if (String::equals(registerStr, "time", true) ||
        String::equals(registerStr, "array", true) ||
        String::equals(registerStr, "uuid", true)) {
        this->value = Variant(ValueTypes::Text);
    } else if (String::equals(registerStr, "increase", true) ||
               String::equals(registerStr, "decrease", true) ||
               String::equals(registerStr, "random", true) ||
               String::equals(registerStr, "sin", true) ||
               String::equals(registerStr, "cos", true)) {
        this->value = Variant(ValueTypes::Float64);
    } else if (String::equals(registerStr, "onoff", true)) {
        this->value = Variant(ValueTypes::Bool);
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
        double minValue, maxValue, step;
        if (!parseDoubleStyle(style, minValue, maxValue, step)) {
            minValue = table->minValue;
            maxValue = table->maxValue;
            step = table->step;
        }
        int maxValueRowCount = (int) Math::round((maxValue - minValue) / step + 0.5);
        double v = minValue;
        v += step * (row % maxValueRowCount);
        result = Double(v).toString();
    } else if (String::equals(registerStr, "decrease", true)) {
        double minValue, maxValue, step;
        if (!parseDoubleStyle(style, minValue, maxValue, step)) {
            minValue = table->minValue;
            maxValue = table->maxValue;
            step = table->step;
        }
        int maxValueRowCount = (int) Math::round((maxValue - minValue) / step + 0.5);
        double v = maxValue;
        v -= step * (row % maxValueRowCount);
        result = Double(v).toString();
    } else if (String::equals(registerStr, "random", true)) {
        double minValue, maxValue, step;
        if (!parseDoubleStyle(style, minValue, maxValue, step)) {
            minValue = table->minValue;
            maxValue = table->maxValue;
            step = table->step;
        }
        double v = Random::getRandValue(minValue, maxValue);
        result = Double(v).toString();
    } else if (String::equals(registerStr, "sin", true)) {
        double minValue, maxValue, step;
        if (!parseDoubleStyle(style, minValue, maxValue, step)) {
            minValue = table->minValue;
            maxValue = table->maxValue;
            step = table->step;
        }
        double v = minValue;
        v += step * row;
        double scope = maxValue - minValue + step;
        v = minValue + scope * (Math::sin(v) + 1.0) / 2.0;
        if (v < minValue)
            v = minValue;
        if (v > maxValue)
            v = maxValue;
        result = Double(v).toString();
    } else if (String::equals(registerStr, "cos", true)) {
        double minValue, maxValue, step;
        if (!parseDoubleStyle(style, minValue, maxValue, step)) {
            minValue = table->minValue;
            maxValue = table->maxValue;
            step = table->step;
        }
        double v = minValue;
        v += step * row;
        double scope = maxValue - minValue + step;
        v = minValue + scope * (Math::cos(v) + 1.0) / 2.0;
        if (v < minValue)
            v = minValue;
        if (v > maxValue)
            v = maxValue;
        result = Double(v).toString();
    } else if (String::equals(registerStr, "onoff", true)) {
        bool v = (row % 2) == 0;
        result = Boolean(v).toString();
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
                    String n = String::trim(nameStr, '\'', '"', ' ');
                    String v = String::trim(valueStr, '\'', '"', ' ');
                    if (String::equals(n, "range", true)) {
                        if (String::equals(v, "today", true)) {
                            DateTime now = DateTime::now();
                            minValue = now.date();
                            maxValue = now.date().addDays(1);
//                            printf("today minValue: %s, maxValue: %s\n", minValue.toString().c_str(), maxValue.toString().c_str());
                        } else if (String::equals(v, "yesterday", true)) {
                            DateTime now = DateTime::now();
                            minValue = now.date().addDays(-1);
                            maxValue = now.date();
//                            printf("yesterday minValue: %s, maxValue: %s\n", minValue.toString().c_str(), maxValue.toString().c_str());
                        } else if (String::equals(v, "thismonth", true)) {
                            DateTime now = DateTime::now();
                            minValue = DateTime(now.year(), now.month(), 1);
                            maxValue = minValue.addMonths(1);
//                            printf("yesterday minValue: %s, maxValue: %s\n", minValue.toString().c_str(), maxValue.toString().c_str());
                        } else {
                            StringArray ranges;
                            StringArray::parse(v, ranges, '-');
                            if (ranges.count() == 2) {
                                DateTime::parse(ranges[0], minValue);
                                DateTime::parse(ranges[1], maxValue);
                            }
                        }
                    } else if (String::equals(n, "step", true)) {
                        TimeSpan::parse(v, step);
//                        printf("v: %s, step: %s\n", v.c_str(), step.toString().c_str());
                    } else if (String::equals(n, "format", true)) {
                        format = v;
                    }
                }
            }
            if (!(minValue == DateTime::MinValue && maxValue == DateTime::MaxValue &&
                  step == TimeSpan::Zero && !format.isNullOrEmpty())) {
                DateTime v = minValue.addTicks(step.ticks() * row);
                result = v.toString(format);
            }
        }
    } else if (String::equals(registerStr, "array", true)) {
        StringArray texts;
        StringArray::parse(style, texts, ';');
        if (!filter.orderBy().isNullOrEmpty()) {
            StringArray orders;
            StringArray::parse(filter.orderBy(), orders, ' ');
            if (orders.count() == 2 && String::equals(orders[0].trim(), name, true)) {
                String order = orders[1].trim();
                if (String::equals(order, "asc", true)) {
                    texts.sort(true);
                } else if (String::equals(order, "desc", true)) {
                    texts.sort(false);
                }
            }
        }
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

bool SimulatorService::Column::parseStyle(const String &str, String &style) {
    JsonNode node;
    if (JsonNode::parse(str, node)) {
        StringArray names;
        node.getAttributeNames(names);
        for (size_t i = 0; i < names.count(); ++i) {
            const String &name = names[i];
            String value = node.getAttribute(name);
            if (!style.isNullOrEmpty()) {
                style.append(';');
            }
            style.append(String::format("%s:'%s'", name.c_str(), value.c_str()));
        }
        return true;
    } else {
        style = str;
        return true;
    }
}

bool SimulatorService::Column::parseDoubleStyle(const String &style, double &minValue, double &maxValue, double &step) {
    if (style.isNullOrEmpty()) {
        return false;
    }

    bool result = true;
    StringArray texts;
    StringArray::parse(style, texts, ';');
    for (size_t i = 0; i < texts.count(); ++i) {
        char nameStr[255] = {0}, valueStr[255] = {0};
        int rValue = sscanf(texts[i].trim().c_str(), "%[a-z|A-Z|0-9]:%s", nameStr, valueStr);
        if (rValue >= 2) {
            String n = String::trim(nameStr, '\'', '"', ' ');
            String v = String::trim(valueStr, '\'', '"', ' ');
            if (String::equals(n, "range", true)) {
                StringArray ranges;
                StringArray::parse(v, ranges, '-');
                if (ranges.count() == 2) {
                    if (!Double::parse(ranges[0], minValue)) {
                        result = false;
                    }
                    if (!Double::parse(ranges[1], maxValue)) {
                        result = false;
                    }
                } else {
                    result = false;
                }
            } else if (String::equals(n, "step", true)) {
                if (!Double::parse(v, step)) {
                    result = false;
                }
            }
        } else {
            result = false;
        }
    }
    return result;
}

SimulatorService::Table::Table() : minValue(0), maxValue(100), step(1), rowCount(0) {
}

SimulatorService::Table::Table(const Table &Table) : minValue(0), maxValue(100), step(1), rowCount(0) {
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

DataRow SimulatorService::Table::toDataRow(const DataTable &table) const {
    return DataRow({
                           DataCell(table.columns()["name"], name),
                           DataCell(table.columns()["range"], toRangeStr()),
                           DataCell(table.columns()["step"], step),
                           DataCell(table.columns()["rowCount"], rowCount),
                           DataCell(table.columns()["columns"], toColumnsStr()),
                   });
}

String SimulatorService::Table::toRangeStr() const {
    return String::format("%s-%s", Double(minValue).toString().c_str(), Double(maxValue).toString().c_str());
}

String SimulatorService::Table::toColumnsStr() const {
    JsonNode result("columns", JsonNode::TypeArray);
    for (size_t i = 0; i < columns.count(); ++i) {
        const Column &column = columns[i];
        JsonNode node;
        node.add(JsonNode("name", column.name));
        node.add(JsonNode("register", column.registerStr));
        node.add(JsonNode("style", column.style));
        result.add(node);
    }
    return result.toString();
}

bool SimulatorService::Table::findName(const String &n) const {
    return n.isNullOrEmpty() || this->name.find(n) >= 0;
}

bool SimulatorService::Table::findColumnName(const String &columnNames) const {
    if (columnNames.isNullOrEmpty()) {
        return true;
    }

    StringArray texts;
    StringArray::parse(columnNames, texts, ';');
    for (size_t i = 0; i < texts.count(); ++i) {
        const String &text = texts[i];
        for (size_t j = 0; j < columns.count(); ++j) {
            if (columns[j].name.find(text) >= 0) {
                return true;
            }
        }
    }

    return false;
}

JsonNode SimulatorService::Table::toJsonNode() const {
    JsonNode node;
    node.add(JsonNode("name", name));
    node.add(JsonNode("range", toRangeStr()));
    node.add(JsonNode("step", step));
    node.add(JsonNode("rowCount", rowCount));
    node.add(JsonNode("columns", toColumnsStr()));
    return node;
}

bool SimulatorService::Table::parseRange(const String &str, double &minValue, double &maxValue) {
    StringArray texts;
    Convert::splitStr(str, texts, '-');
    if (texts.count() == 2) {
        double minV = 0, maxV = 100;
        if (Double::parse(texts[0], minV) &&
            Double::parse(texts[1], maxV) && maxV > minV) {
            minValue = minV;
            maxValue = maxV;
            return true;
        }
    }
    return false;
}

bool SimulatorService::Table::parseColumns(const String &str, Columns &columns) {
    // str such like {"name":"time","register":"time","style":{"range":"today","step":"01:00:00","format":"G"}};
    // {"name":"tag1","register":"increase"};{"name":"tag2","register":"decrease"};
    StringArray columnsTexts;
    Convert::splitStr(str, columnsTexts, ';');
    for (size_t i = 0; i < columnsTexts.count(); ++i) {
        JsonNode node;
        if (JsonNode::parse(columnsTexts[i], node)) {
            Column column;
            column.name = node.getAttribute("name").trim();
            column.registerStr = node.getAttribute("register").trim();
            Column::parseStyle(node.getAttribute("style"), column.style);
            columns.add(column);
        }
    }
    return columns.count() > 0;
}

SimulatorService::SimulatorService() : _timer(nullptr) {
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    factory->addService<SimulatorService>(this);

    // initialize.
    auto *cs = factory->getService<IConfigService>();
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

    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    factory->removeService<SimulatorService>();
}

FetchResult SimulatorService::getLabelValues(const String &labelName, const StringArray &tagNames,
                                             const SqlSelectFilter &filter, StringMap &values) {
    Locker locker(&_labels);
    for (size_t i = 0; i < _labels.count(); i++) {
        const Label &label = _labels[i];
        if (labelName == label.name) {
            for (size_t j = 0; j < label.tags.count(); j++) {
                const Tag &tag = label.tags[j];
                if (tagNames.contains(tag.name)) {
                    values.add(tag.name, tag.getValue(filter));
                }
            }
            return FetchResult::Succeed;
        }
    }
    return FetchResult::RowCountError;
}

FetchResult
SimulatorService::getTableValues(const String &tableName, const StringArray &columns,
                                 const SqlSelectFilter &filter, DataTable &dataTable) {
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
            return FetchResult::Succeed;
        }
    }
    return FetchResult::RowCountError;
}

void SimulatorService::initLabels() {
    Locker locker(&_labels);
    _labels.clear();

    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto *cs = factory->getService<IConfigService>();
    assert(cs);

    for (int i = 0; i < maxLabelCount; i++) {
        Label label;
        if (!cs->getProperty(String::format(labelPrefix "name", i), label.name)) {
            break;
        }

        bool enable = true;
        cs->getProperty(String::format(labelPrefix "enable", i), enable);
        if (enable) {
            String range;
            cs->getProperty(String::format(labelPrefix "range", i), range);
            Label::parseRange(range, label.minValue, label.maxValue);
            cs->getProperty(String::format(labelPrefix "step", i), label.step);
            cs->getProperty(String::format(labelPrefix "interval", i), label.interval);
            for (int j = 0; j < maxTagCount; j++) {
                Tag tag;
                if (!cs->getProperty(String::format(tagPrefix "name", i, j), tag.name)) {
                    break;
                }
                cs->getProperty(String::format(tagPrefix "register", i, j), tag.registerStr);
                cs->getProperty(String::format(tagPrefix "style", i, j), tag.style);
                label.tags.add(tag);
            }
            _labels.add(label);
        }
    }
}

void SimulatorService::initTables() {
    Locker locker(&_tables);
    _tables.clear();

    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto *cs = factory->getService<IConfigService>();
    assert(cs);

    for (int i = 0; i < maxTableCount; i++) {
        Table table;
        if (!cs->getProperty(String::format(tablePrefix "name", i), table.name)) {
            break;
        }

        bool enable = true;
        cs->getProperty(String::format(tablePrefix "enable", i), enable);
        if (enable) {
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
        Label &label = _labels[i];
        if (label.isTimeUp()) {
            for (size_t j = 0; j < label.tags.count(); j++) {
                Tag &tag = label.tags[j];
                tag.runOnce(&label);
//                printf("tag('%s.%s') value: %s\n", label.name.c_str(), tag.name.c_str(), tag.value.valueStr().c_str());
            }
        }
    }
}

bool SimulatorService::getLabels(const SqlSelectFilter &filter, DataTable &table) {
    Locker locker(&_labels);
    Labels match;
    for (size_t i = 0; i < _labels.count(); ++i) {
        const Label &label = _labels[i];
        String name = filter.getValue("name");
        String tags = filter.getValue("tags");
        if (label.findName(name) && label.findTagName(tags)) {
            match.add(label);
        }
    }

    table.setName("label");
    table.setTotalCount((int) match.count());
    table.addColumns({
                             DataColumn("name", ValueTypes::Text, true),
                             DataColumn("range", ValueTypes::Text, false),
                             DataColumn("step", ValueTypes::Float64, false),
                             DataColumn("interval", ValueTypes::Timestamp, false),
                             DataColumn("tags", ValueTypes::Text, false)
                     });
    size_t offset = Math::max(0, filter.offset());
    size_t count = Math::min((int) match.count(), filter.offset() + filter.limit());
    for (size_t i = offset; i < count; ++i) {
        const Label &label = match[i];
        table.addRow(label.toDataRow(table));
    }

    return true;
}

bool SimulatorService::getLabel(const StringMap &request, StringMap &response) {
    {
        Locker locker(&_labels);
        String name = request["name"];
        for (size_t i = 0; i < _labels.count(); ++i) {
            const Label &label = _labels[i];
            if (label.name == name) {
                response["code"] = "0";
                response["data"] = label.toJsonNode().toString();
                return true;
            }
        }
    }

    // Can not find label by name.
    response.addRange(HttpCode::at(CannotFindLabel));
    return false;
}

bool SimulatorService::addLabel(const StringMap &request, StringMap &response) {
    Locker locker(&_labels);
    String name = request["name"];
    for (size_t i = 0; i < _labels.count(); ++i) {
        const Label &label = _labels[i];
        if (label.name == name) {
            // Duplicate name.
            response.addRange(HttpCode::at(DuplicateName));
            return false;
        }
    }
    return addOrUpdateLabel(request, response);
}

bool SimulatorService::removeLabel(const StringMap &request, StringMap &response) {
    StringArray names;
    StringArray::parse(request["name"], names, ';');

    // find label name.
    Locker locker(&_labels);
    bool found = false;
    Vector<int> positions;
    for (int i = (int) _labels.count() - 1; i >= 0; --i) {
        const Label &label = _labels[i];
        for (size_t j = 0; j < _labels.count(); ++j) {
            if (label.name == names[j]) {
                positions.add((int) i);
                _labels.removeAt(i);
                found = true;
                break;
            }
        }
    }
    if (!found) {
        // Can not find label by name.
        response.addRange(HttpCode::at(CannotFindLabel));
        return false;
    }

    // update profile yml file.
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto *cs = factory->getService<IConfigService>();
    assert(cs);

    YmlNode::Properties properties;
    for (size_t i = 0; i < positions.count(); ++i) {
        updateLabelYmlProperties(false, positions[i], properties);
    }
    if (!cs->updateConfigFile(properties)) {
        // Failed to save config file.
        response.addRange(HttpCode::at(FailedToSave));
        return false;
    }

    response.addRange(HttpCode::okCode());
    return true;
}

bool SimulatorService::updateLabel(const StringMap &request, StringMap &response) {
    Locker locker(&_labels);
    String name = request["name"];
    int position;
    bool found = false;
    for (size_t i = 0; i < _labels.count(); ++i) {
        const Label &label = _labels[i];
        if (label.name == name) {
            found = true;
            position = (int) i;
            break;
        }
    }
    if (!found) {
        // Can not find label by name.
        response.addRange(HttpCode::at(CannotFindLabel));
        return false;
    }
    return addOrUpdateLabel(request, response, position);
}

bool SimulatorService::addOrUpdateLabel(const StringMap &request, StringMap &response, int position) {
    Label label;
    label.name = request["name"];
    Label::parseRange(request["range"], label.minValue, label.maxValue);
    Double::parse(request["step"], label.step);
    TimeSpan::parse(request["interval"], label.interval);
    Label::parseTags(request["tags"], label.tags);

    // update profile yml file.
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto *cs = factory->getService<IConfigService>();
    assert(cs);

    YmlNode::Properties properties;
    updateYmlProperties(label, position >= 0 ? position : (int) _labels.count(), properties);
    if (!cs->updateConfigFile(properties)) {
        // Failed to save config file.
        response.addRange(HttpCode::at(FailedToSave));
        return false;
    }

    // update labels in memory.
    if (position >= 0) {
        _labels.set(position, label);
    } else {
        _labels.add(label);
    }

    response.addRange(HttpCode::okCode());
    return true;
}

bool SimulatorService::addOrUpdateTable(const StringMap &request, StringMap &response, int position) {
    Table table;
    table.name = request["name"];
    Table::parseRange(request["range"], table.minValue, table.maxValue);
    Double::parse(request["step"], table.step);
    Int32::parse(request["rowCount"], table.rowCount);
    Table::parseColumns(request["columns"], table.columns);

    // update profile yml file.
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto *cs = factory->getService<IConfigService>();
    assert(cs);

    YmlNode::Properties properties;
    updateYmlProperties(table, position >= 0 ? position : (int) _tables.count(), properties);
    if (!cs->updateConfigFile(properties)) {
        // Failed to save config file.
        response.addRange(HttpCode::at(FailedToSave));
        return false;
    }

    // update tables in memory.
    if (position >= 0) {
        _tables.set(position, table);
    } else {
        _tables.add(table);
    }

    response.addRange(HttpCode::okCode());
    return true;
}

bool SimulatorService::getTables(const SqlSelectFilter &filter, DataTable &table) {
    Locker locker(&_tables);
    Tables match;
    for (size_t i = 0; i < _tables.count(); ++i) {
        const Table &t = _tables[i];
        String name = filter.getValue("name");
        String columns = filter.getValue("columns");
        if (t.findName(name) && t.findColumnName(columns)) {
            match.add(t);
        }
    }

    table.setName("table");
    table.setTotalCount((int) match.count());
    table.addColumns({
                             DataColumn("name", ValueTypes::Text, true),
                             DataColumn("range", ValueTypes::Text, false),
                             DataColumn("step", ValueTypes::Float64, false),
                             DataColumn("rowCount", ValueTypes::Integer32, false),
                             DataColumn("columns", ValueTypes::Text, false)
                     });
    size_t offset = Math::max(0, filter.offset());
    size_t count = Math::min((int) match.count(), filter.offset() + filter.limit());
    for (size_t i = offset; i < count; ++i) {
        const Table &t = match[i];
        table.addRow(t.toDataRow(table));
    }

    return true;
}

bool SimulatorService::getTable(const StringMap &request, StringMap &response) {
    {
        Locker locker(&_tables);
        String name = request["name"];
        for (size_t i = 0; i < _tables.count(); ++i) {
            const Table &table = _tables[i];
            if (table.name == name) {
                response["code"] = "0";
                response["data"] = table.toJsonNode().toString();
                return true;
            }
        }
    }

    // Can not find table by name.
    response.addRange(HttpCode::at(CannotFindTable));
    return false;
}

bool SimulatorService::addTable(const StringMap &request, StringMap &response) {
    Locker locker(&_tables);
    String name = request["name"];
    for (size_t i = 0; i < _tables.count(); ++i) {
        const Table &table = _tables[i];
        if (table.name == name) {
            // Duplicate name.
            response.addRange(HttpCode::at(DuplicateName));
            return false;
        }
    }
    return addOrUpdateTable(request, response);
}

bool SimulatorService::removeTable(const StringMap &request, StringMap &response) {
    StringArray names;
    StringArray::parse(request["name"], names, ';');

    // find table name.
    Locker locker(&_tables);
    bool found = false;
    Vector<int> positions;
    for (int i = (int) _tables.count() - 1; i >= 0; --i) {
        const Table &table = _tables[i];
        for (size_t j = 0; j < _tables.count(); ++j) {
            if (table.name == names[j]) {
                positions.add((int) i);
                _tables.removeAt(i);
                found = true;
                break;
            }
        }
    }
    if (!found) {
        // Can not find table by name.
        response.addRange(HttpCode::at(CannotFindTable));
        return false;
    }

    // update profile yml file.
    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto *cs = factory->getService<IConfigService>();
    assert(cs);

    YmlNode::Properties properties;
    for (size_t i = 0; i < positions.count(); ++i) {
        updateTableYmlProperties(false, positions[i], properties);
    }
    if (!cs->updateConfigFile(properties)) {
        // Failed to save config file.
        response.addRange(HttpCode::at(FailedToSave));
        return false;
    }

    response.addRange(HttpCode::okCode());
    return true;
}

bool SimulatorService::updateTable(const StringMap &request, StringMap &response) {
    // find table name.
    Locker locker(&_tables);
    String name = request["name"];
    int position;
    bool found = false;
    for (size_t i = 0; i < _tables.count(); ++i) {
        const Table &table = _tables[i];
        if (table.name == name) {
            found = true;
            position = (int) i;
            break;
        }
    }
    if (!found) {
        // Can not find table by name.
        response.addRange(HttpCode::at(CannotFindTable));
        return false;
    }
    return addOrUpdateTable(request, response, position);
}

void SimulatorService::updateYmlProperties(const Label &label, int position, YmlNode::Properties &properties) {
    properties.add(String::format(labelPrefix "name", position), label.name);
    properties.add(String::format(labelPrefix "range", position), label.toRangeStr());
    properties.add(String::format(labelPrefix "step", position), label.step);
    properties.add(String::format(labelPrefix "interval", position), label.interval);
    for (size_t i = 0; i < label.tags.count(); ++i) {
        const Tag &tag = label.tags[i];
        properties.add(String::format(tagPrefix "name", position, i), tag.name);
        properties.add(String::format(tagPrefix "register", position, i), tag.registerStr);
    }
}

void SimulatorService::updateLabelYmlProperties(bool enable, int position, YmlNode::Properties &properties) {
    properties.add(String::format(labelPrefix "enable", position), enable);
}

void SimulatorService::updateYmlProperties(const Table &table, int position, YmlNode::Properties &properties) {
    properties.add(String::format(tablePrefix "name", position), table.name);
    properties.add(String::format(tablePrefix "range", position), table.toRangeStr());
    properties.add(String::format(tablePrefix "step", position), table.step);
    properties.add(String::format(tablePrefix "rowCount", position), table.rowCount);
    for (size_t i = 0; i < table.columns.count(); ++i) {
        const Column &column = table.columns[i];
        properties.add(String::format(columnPrefix "name", position, i), column.name);
        properties.add(String::format(columnPrefix "register", position, i), column.registerStr);
        properties.add(String::format(columnPrefix "style", position, i), column.style);
    }
}

void SimulatorService::updateTableYmlProperties(bool enable, int position, YmlNode::Properties &properties) {
    properties.add(String::format(tablePrefix "enable", position), enable);
}
