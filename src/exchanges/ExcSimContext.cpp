//
//  ExcSimContext.cpp
//  tserver
//
//  Created by baowei on 2023/3/31.
//  Copyright (c) 2023 com. All rights reserved.
//

#include "thread/TickTimeout.h"
#include "system/Random.h"
#include "system/Math.h"
#include "database/SnowFlake.h"
#include "ExcSimContext.h"

using namespace IO;
using namespace System;

Item::Item(const String &name, const String &registerStr, const Style &style) :
        name(name), registerStr(registerStr), style(style) {
}

void Item::evaluates(const Item &other) {
    this->name = other.name;
    this->registerStr = other.registerStr;
    this->style = other.style;
}

bool Item::equals(const Item &other) const {
    return this->name == other.name &&
           this->registerStr == other.registerStr &&
           this->style == other.style;
}

bool Item::parseRange(const Style &style, double &minValue, double &maxValue, double &step) {
    if (style.isEmpty()) {
        return false;
    }

    bool result = true;
    String rangeStr = String::trim(style["range"], '\'', '"', ' ');
    StringArray ranges;
    StringArray::parse(rangeStr, ranges, '-');
    if (ranges.count() == 2) {
        if (!Double::parse(ranges[0], minValue)) {
            result = false;
        }
        if (!Double::parse(ranges[1], maxValue)) {
            result = false;
        }
        if (minValue > maxValue) {
            result = false;
        }
    } else {
        result = false;
    }

    String stepStr = String::trim(style["step"], '\'', '"', ' ');
    if (!Double::parse(stepStr, step)) {
        result = false;
    }
    return result;
}

Element::Element() : enable(true), minValue(Double::NaN), maxValue(Double::NaN), step(Double::NaN) {
}

Element::Element(const String &name, double minValue, double maxValue, double step) :
        name(name), enable(true), minValue(minValue), maxValue(maxValue), step(step) {
}

void Element::evaluates(const Element &other) {
    this->name = other.name;
    this->enable = other.enable;
    this->minValue = other.minValue;
    this->maxValue = other.maxValue;
    this->step = other.step;
}

bool Element::equals(const Element &other) const {
    return this->name == other.name &&
           this->enable == other.enable &&
           this->minValue == other.minValue &&
           this->maxValue == other.maxValue &&
           this->step == other.step;
}

bool Element::findName(const String &n) const {
    return n.isNullOrEmpty() || this->name.find(n) >= 0;
}

bool Element::findItemName(const String &itemNames) const {
    if (itemNames.isNullOrEmpty()) {
        return true;
    }
    StringArray texts;
    StringArray::parse(itemNames, texts, ';');
    return findItemName(texts);
}

bool Element::parseRange(const String &str, double &minValue, double &maxValue) {
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

String Element::toRangeStr() const {
    return String::format("%s-%s", Double(minValue).toString().c_str(), Double(maxValue).toString().c_str());
}

Tag::Tag(const String &name, const String &registerStr, const Style &style) :
        Item(name, registerStr, style) {
}

Tag::Tag(const Tag &other) : Item(other) {
    Item::evaluates(other);
}

bool Tag::equals(const Tag &other) const {
    return Item::equals(other);
}

void Tag::evaluates(const Tag &other) {
    Item::evaluates(other);
}

Variant Tag::getValue(const Label *label, const SqlSelectFilter &filter) const {
    double minValue, maxValue, step;
    if (!parseRange(style, minValue, maxValue, step)) {
        minValue = label->minValue;
        maxValue = label->maxValue;
        step = label->step;
    }

    ServiceFactory *factory = ServiceFactory::instance();
    assert(factory);
    auto esc = factory->getService<IExcSimCache>();
    if (esc == nullptr) {
        return Variant::NullValue;
    }

    Variant result;
    const String key = String::format("%s.%s", label->name.c_str(), name.c_str());
    String oldValueStr = esc->getValue(key);
    if (String::equals(registerStr, "increase", true)) {
        double oldValue, value;
        if (!Double::parse(oldValueStr, oldValue)) {
            value = minValue;
        } else {
            value = oldValue;
            value += step;
            if (value > maxValue) {
                value = minValue;
            }
        }
        result = Variant(Variant::Float64, value);
        esc->setValue(key, Double(value).toString());
    } else if (String::equals(registerStr, "decrease", true)) {
        double oldValue, value;
        if (!Double::parse(oldValueStr, oldValue)) {
            value = maxValue;
        } else {
            value = oldValue;
            value -= step;
            if (value < minValue) {
                value = maxValue;
            }
        }
        result = Variant(Variant::Float64, value);
        esc->setValue(key, Double(value).toString());
    } else if (String::equals(registerStr, "random", true)) {
        double value = Random::getRandValue(minValue, maxValue);
        result = Variant(Variant::Float64, value);
    } else if (String::equals(registerStr, "sin", true)) {
        const String xKey = String::format("%s.%s.x", label->name.c_str(), name.c_str());
        String oldXValueStr = esc->getValue(xKey);
        double oldXValue;
        Double::parse(oldXValueStr, oldXValue);
        if (Double::isNaN(oldXValue))
            oldXValue = 0.0;
        else
            oldXValue = oldXValue + step;
        if (oldXValue > Math::PI * 2.0)
            oldXValue = 0.0;
        esc->setValue(xKey, Double(oldXValue).toString());
        double scope = maxValue - minValue + step;
        double value = minValue + scope * (Math::sin(oldXValue) + 1.0) / 2.0;
        if (value < minValue)
            value = minValue;
        if (value > maxValue)
            value = maxValue;
        result = Variant(Variant::Float64, value);
    } else if (String::equals(registerStr, "cos", true)) {
        const String xKey = String::format("%s.%s.x", label->name.c_str(), name.c_str());
        String oldXValueStr = esc->getValue(xKey);
        double oldXValue;
        Double::parse(oldXValueStr, oldXValue);
        if (Double::isNaN(oldXValue))
            oldXValue = 0.0;
        else
            oldXValue = oldXValue + step;
        if (oldXValue > Math::PI * 2.0)
            oldXValue = 0.0;
        esc->setValue(xKey, Double(oldXValue).toString());
        double scope = maxValue - minValue + step;
        double value = minValue + scope * (Math::cos(oldXValue) + 1.0) / 2.0;
        if (value < minValue)
            value = minValue;
        if (value > maxValue)
            value = maxValue;
        result = Variant(Variant::Float64, value);
    } else if (String::equals(registerStr, "onoff", true)) {
        int oldValue, value;
        if (!Int32::parse(oldValueStr, oldValue)) {
            value = 0;
        } else {
            value = oldValue == 0 ? 1 : 0;
        }
        result = Variant(Variant::Integer32, value);
        esc->setValue(key, Int32(value).toString());
    } else if (String::equals(registerStr, "uuid", true)) {
        Uuid value = Uuid::generate();
        result = Variant(Variant::Text, value.toString());
    } else if (String::equals(registerStr, "array", true)) {
        StringArray texts;
        StringArray::parse(style.toString(), texts, ',');
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
                            result = Variant(Variant::Text, v);
                            break;
                        }
                    }
                }
            }
        }
    } else if (String::equals(registerStr, "incoming", true)) {
        result = Variant(Variant::Text, filter.getValue(name));
    } else {
        double value;
        if (Double::parse(registerStr, value)) {
            result = Variant(Variant::Float64, value);
        } else {
            result = Variant(Variant::Text, registerStr);
        }
    }
    return result;
}

DbType Tag::type() const {
    if (String::equals(registerStr, "time", true) ||
        String::equals(registerStr, "array", true) ||
        String::equals(registerStr, "uuid", true) ||
        String::equals(registerStr, "incoming", true)) {
        return Variant::Text;
    } else if (String::equals(registerStr, "increase", true) ||
               String::equals(registerStr, "decrease", true) ||
               String::equals(registerStr, "random", true) ||
               String::equals(registerStr, "sin", true) ||
               String::equals(registerStr, "cos", true)) {
        return Variant::Float64;
    } else if (String::equals(registerStr, "onoff", true)) {
        return Variant::Integer32;
    } else {
        double v;
        if (Double::parse(registerStr, v)) {
            return Variant::Float64;
        } else {
            return Variant::Text;
        }
    }
}

Tags::Tags() = default;

Tags::~Tags() = default;

bool Tags::contains(const StringArray &names) const {
    for (int i = 0; i < names.count(); ++i) {
        const String &name = names[i];
        bool found = false;
        for (int j = 0; j < count(); ++j) {
            const Tag &tag = at(j);
            if (String::equals(tag.name, name, true)) {
                found = true;
                break;
            }
        }
        if (!found) {
            return false;
        }
    }
    return true;
}

bool Tags::atByName(const String &name, Tag &tag) const {
    for (int i = 0; i < count(); ++i) {
        const Tag &t = at(i);
        if (String::equals(t.name, name, true)) {
            tag = t;
            return true;
        }
    }
    return false;
}

Label::Label(const String &name, double minValue, double maxValue, double step) :
        Element(name, minValue, maxValue, step) {
}

Label::Label(const Label &other) : Element(other) {
    Label::evaluates(other);
}

bool Label::equals(const Label &other) const {
    return Element::equals(other) &&
           this->tags == other.tags;
}

void Label::evaluates(const Label &other) {
    Element::evaluates(other);
    this->tags = other.tags;
}

void Label::updateYmlProperties(YmlNode::Properties &properties, int pos) const {
    properties.add(String::format(LabelPrefix "name", pos), name);
    properties.add(String::format(LabelPrefix "range", pos), toRangeStr());
    properties.add(String::format(LabelPrefix "step", pos), step);
    for (size_t i = 0; i < tags.count(); ++i) {
        const Tag &tag = tags[i];
        properties.add(String::format(TagPrefix "name", pos, i), tag.name);
        properties.add(String::format(TagPrefix "register", pos, i), tag.registerStr);
        properties.add(String::format(TagPrefix "style", pos, i), tag.style);
    }
}

void Label::removeYmlProperties(YmlNode::Properties &properties, int pos) const {
    properties.remove(String::format(LabelPrefix "name", pos));
    properties.remove(String::format(LabelPrefix "range", pos));
    properties.remove(String::format(LabelPrefix "step", pos));
    properties.remove(String::format(LabelPrefix "tags", pos));
    for (size_t i = 0; i < tags.count(); ++i) {
        const Tag &tag = tags[i];
        properties.remove(String::format(TagPrefix "name", pos, i));
        properties.remove(String::format(TagPrefix "register", pos, i));
        properties.remove(String::format(TagPrefix "style", pos, i));
    }
}

String Label::getTableName(const String &prefix, const String &tableName) {
    return prefix.isNullOrEmpty() ? tableName : String::format("%s.%s", prefix.c_str(), tableName.c_str());
}

DataRow Label::toDataRow(const DataTable &table) const {
    return DataRow({
                           DataCell(table.columns()["name"], name),
                           DataCell(table.columns()["range"], toRangeStr()),
                           DataCell(table.columns()["step"], step),
                           DataCell(table.columns()["tags"], toValuesStr()),
                   });
}

String Label::toValuesStr() const {
    JsonNode result("tags", JsonNode::TypeArray);
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

bool Label::findItemName(const StringArray &itemNames) const {
    for (size_t i = 0; i < itemNames.count(); ++i) {
        const String &itemName = itemNames[i];
        for (size_t j = 0; j < tags.count(); ++j) {
            if (tags[j].name.find(itemName) >= 0) {
                return true;
            }
        }
    }
    return false;
}

JsonNode Label::toJsonNode() const {
    JsonNode node;
    node.add(JsonNode("name", name));
    node.add(JsonNode("range", toRangeStr()));
    node.add(JsonNode("step", step));
    node.add(JsonNode("tags", toValuesStr()));
    return node;
}

bool Label::parseTags(const String &str, Tags &tags) {
    // str such like [{"name":"tag1","register":"increase"},{"name":"tag2","register":"decrease"},{"name":"tag3","register":"random"}]
    JsonNode tagsNode;
    if (JsonNode::parse(str, tagsNode)) {
        for (size_t i = 0; i < tagsNode.count(); ++i) {
            JsonNode node = tagsNode.at(i);
            Tag tag;
            tag.name = node.getAttribute("name").trim();
            tag.registerStr = node.getAttribute("register").trim();
            Style::parse(node.getAttribute("style"), tag.style);
            tags.add(tag);
        }
        return tags.count() > 0;
    }
    return false;
}

bool Label::parse(const StringMap &request, Label &label) {
    if (!request.at("name", label.name)) {
        return false;
    }
    if (!Label::parseRange(request["range"], label.minValue, label.maxValue)) {
        return false;
    }
    if (!Double::parse(request["step"], label.step)) {
        return false;
    }
    if (!Label::parseTags(request["tags"], label.tags)) {
        return false;
    }
    return true;
}

bool Label::parse(const YmlNode::Properties &properties, int pos, Label &label) {
    String name;
    if (properties.at(String::format(LabelPrefix "name", pos), name)) {
        label.name = name;
        properties.at(String::format(LabelPrefix "enable", pos), label.enable);
        String range;
        properties.at(String::format(LabelPrefix "range", pos), range);
        Label::parseRange(range, label.minValue, label.maxValue);
        properties.at(String::format(LabelPrefix "step", pos), label.step);
        for (int i = 0; i < MaxTagCount; i++) {
            Tag tag;
            if (!properties.at(String::format(TagPrefix "name", pos, i), tag.name)) {
                break;
            }
            properties.at(String::format(TagPrefix "register", pos, i), tag.registerStr);
            properties.at(String::format(TagPrefix "style", pos, i), tag.style);
            label.tags.add(tag);
        }
        return true;
    }
    return false;
}

bool Label::parse(const DataRow &labelRow, const DataRows &tagRows, Label &label) {
    if (!labelRow["name"].value().getValue(label.name)) {
        return false;
    }
    if (!labelRow["min_value"].value().getValue(label.minValue)) {
        return false;
    }
    if (!labelRow["max_value"].value().getValue(label.maxValue)) {
        return false;
    }
    if (!labelRow["step"].value().getValue(label.step)) {
        return false;
    }
    for (size_t i = 0; i < tagRows.count(); ++i) {
        const DataRow &row = tagRows[i];
        Tag tag;
        if (!row["name"].value().getValue(tag.name)) {
            return false;
        }
        row["register"].value().getValue(tag.registerStr);
        Style::parse(row["style"].valueStr(), tag.style);
        label.tags.add(tag);
    }
    return true;
}

bool Label::parse(const DataRow &row, Label &label) {
    if (!row["name"].value().getValue(label.name)) {
        return false;
    }
    if (!row["min_value"].value().getValue(label.minValue)) {
        return false;
    }
    if (!row["max_value"].value().getValue(label.maxValue)) {
        return false;
    }
    if (!row["step"].value().getValue(label.step)) {
        return false;
    }
    if (!Label::parseTags(row["tags"].valueStr(), label.tags)) {
        return false;
    }
    return true;
}

String Label::toSelectSqlStr(const String &prefix, const String &name) {
    String labelTableName = getTableName(prefix, LabelTableName);
    String tagTableName = getTableName(prefix, TagTableName);
    String tagsSql = String::format("(SELECT JSON_ARRAYAGG(JSON_OBJECT("
                                    "'name',NAME,'register',register,'style',style))"
                                    " FROM %s WHERE %s.label_id=%s.id) tags",
                                    tagTableName.c_str(),
                                    tagTableName.c_str(),
                                    labelTableName.c_str());
    String sql = String::format("SELECT id,name,min_value,max_value,step,%s FROM %s WHERE name='%s' LIMIT 1",
                                tagsSql.c_str(),
                                labelTableName.c_str(),
                                name.c_str());
    return sql;
}

String Label::toSelectSqlStr(const String &prefix, const SqlSelectFilter &filter) {
//    SELECT name,concat(concat(CAST(min_value as CHAR(64)),'-'),CAST(max_value as CHAR(64)))
//    'range',step,(SELECT JSON_ARRAYAGG(JSON_OBJECT('name',NAME,'register',register,'style',style)) FROM tag
//    WHERE tag.label_id=label.id AND name LIKE '%tag%') tags FROM label WHERE 1=1 ORDER BY name asc LIMIT 0,100
    String labelTableName = getTableName(prefix, LabelTableName);
    String tagTableName = getTableName(prefix, TagTableName);
    static const char *rangeStr = "concat(concat(CAST(min_value as CHAR(64)),'-'),"
                                  "CAST(max_value as CHAR(64))) 'range'";
    String tagsSql = String::format("(SELECT JSON_ARRAYAGG(JSON_OBJECT("
                                    "'name',NAME,'register',register,'style',style))"
                                    " FROM %s WHERE %s.label_id=%s.id AND %s) tags",
                                    tagTableName.c_str(),
                                    tagTableName.c_str(),
                                    labelTableName.c_str(),
                                    filter.toArrayLikeStr("tags", "name").c_str());
    String sql = String::format("SELECT name,%s,step,%s FROM %s WHERE %s ORDER BY %s LIMIT %d,%d",
                                rangeStr,
                                tagsSql.c_str(),
                                labelTableName.c_str(),
                                filter.toLikeStr("name").c_str(),
                                !filter.orderBy().isNullOrEmpty() ? filter.orderBy().c_str() : "name asc",
                                filter.offset(), filter.limit());
    return sql;
}

String Label::toCountSqlStr(const String &prefix, const SqlSelectFilter &filter) {
    String labelTableName = getTableName(prefix, LabelTableName);
    String sql = String::format("SELECT COUNT(1) FROM %s WHERE %s",
                                labelTableName.c_str(),
                                filter.toLikeStr("name").c_str());
    return sql;
}

String Label::toInsertSqlStr(const String &prefix) const {
    String sql;
    DateTime now = DateTime::now();
    uint64_t lid = SnowFlake::generateId();
    String labelTableName = getTableName(prefix, LabelTableName);
    String tagTableName = getTableName(prefix, TagTableName);
    sql = String::format("INSERT INTO %s VALUES(%lld,'%s',%llg,%llg,%llg,'%s','','%s','');",
                         labelTableName.c_str(),
                         lid, name.c_str(),
                         minValue, maxValue, step,
                         now.toString().c_str(), now.toString().c_str());
    for (size_t i = 0; i < tags.count(); ++i) {
        const Tag &tag = tags[i];
        sql.appendLine(String::format("INSERT INTO %s VALUES(%lld,%lld,'%s','%s','%s','%s','','%s','');",
                                      tagTableName.c_str(),
                                      SnowFlake::generateId(), lid,
                                      tag.name.c_str(), tag.registerStr.c_str(), tag.style.toString().c_str(),
                                      now.toString().c_str(), now.toString().c_str()));
    }
    return sql;
}

String Label::toReplaceSqlStr(const String &prefix) const {
    String sql;
    DateTime now = DateTime::now();
    String labelTableName = getTableName(prefix, LabelTableName);
    String tagTableName = getTableName(prefix, TagTableName);
    sql = String::format(
            "UPDATE %s SET min_value=%llg,max_value=%llg,step=%llg,update_time='%s',update_user='' WHERE name='%s';",
            labelTableName.c_str(),
            minValue, maxValue, step,
            now.toString().c_str(), name.c_str());
    String idSelectStr = String::format("(SELECT id FROM %s WHERE name='%s')", labelTableName.c_str(), name.c_str());
    sql.appendLine(String::format("DELETE FROM %s WHERE label_id=%s", idSelectStr.c_str()));
    for (size_t i = 0; i < tags.count(); ++i) {
        const Tag &tag = tags[i];
        sql.appendLine(String::format("INSERT INTO %s VALUES(%lld,%s,'%s','%s','%s','%s','','%s','');",
                                      tagTableName.c_str(),
                                      SnowFlake::generateId(),
                                      idSelectStr.c_str(),
                                      tag.name.c_str(), tag.registerStr.c_str(), tag.style.toString().c_str(),
                                      now.toString().c_str(), now.toString().c_str()));
    }
    return sql;
}

String Label::toDeleteSqlStr(const String &prefix, const String &labelName) {
    String sql;
    String labelTableName = getTableName(prefix, LabelTableName);
    String tagTableName = getTableName(prefix, TagTableName);
    String idSelectStr = String::format("(SELECT id FROM %s WHERE name='%s')", labelTableName.c_str(),
                                        labelName.c_str());
    sql.appendLine(String::format("DELETE FROM %s WHERE label_id=%s;",
                                  tagTableName.c_str(),
                                  idSelectStr.c_str()));
    sql.appendLine(String::format("DELETE FROM %s WHERE name='%s';",
                                  labelTableName.c_str(),
                                  labelName.c_str()));
    return sql;
}

bool Label::getTags(const StringArray &tagNames, Tags &values) const {
    if (tagNames.isEmpty()) {
        values.addRange(tags);
    } else {
        for (size_t i = 0; i < tagNames.count(); ++i) {
            const String &tagName = tagNames[i];
            Tag tag;
            if (tags.atByName(tagName, tag)) {
                values.add(tag);
            }
        }
    }
    return values.count() > 0;
}

Column::Column(const String &name, const String &registerStr, const Style &style) :
        Item(name, registerStr, style) {
}

Column::Column(const Column &Column) : Item(Column) {
    Column::evaluates(Column);
}

bool Column::equals(const Column &other) const {
    return Item::equals(other);
}

void Column::evaluates(const Column &other) {
    Item::evaluates(other);
}

Variant Column::getCellValue(const Table *table, const SqlSelectFilter &filter, int row, int col) const {
    const Rows &rows = table->rows;
    if (rows.count() > 0) {
        if (row < rows.count()) {
            auto cells = rows[row].cells();
            const String &cellValue = col < cells.count() ? cells[col] : String::Empty;
            Variant value(type(), cellValue);
            return value;
        } else {
            return Variant::NullValue;
        }
    } else {
        double minValue, maxValue, step;
        if (!parseRange(style, minValue, maxValue, step)) {
            minValue = table->minValue;
            maxValue = table->maxValue;
            step = table->step;
        }

        Variant result;
        if (String::equals(registerStr, "increase", true)) {
            int maxValueRowCount = (int) Math::round((maxValue - minValue) / step + 0.5);
            double v = minValue;
            v += step * (row % maxValueRowCount);
            result = Variant(Variant::Float64, v);
        } else if (String::equals(registerStr, "decrease", true)) {
            int maxValueRowCount = (int) Math::round((maxValue - minValue) / step + 0.5);
            double v = maxValue;
            v -= step * (row % maxValueRowCount);
            result = Variant(Variant::Float64, v);
        } else if (String::equals(registerStr, "random", true)) {
            double v = Random::getRandValue(minValue, maxValue);
            result = Variant(Variant::Float64, v);
        } else if (String::equals(registerStr, "sin", true)) {
            double v = minValue;
            v += step * row;
            double scope = maxValue - minValue + step;
            v = minValue + scope * (Math::sin(v) + 1.0) / 2.0;
            if (v < minValue)
                v = minValue;
            if (v > maxValue)
                v = maxValue;
            result = Variant(Variant::Float64, v);
        } else if (String::equals(registerStr, "cos", true)) {
            double v = minValue;
            v += step * row;
            double scope = maxValue - minValue + step;
            v = minValue + scope * (Math::cos(v) + 1.0) / 2.0;
            if (v < minValue)
                v = minValue;
            if (v > maxValue)
                v = maxValue;
            result = Variant(Variant::Float64, v);
        } else if (String::equals(registerStr, "onoff", true)) {
            int v = row % 2;
            result = Variant(Variant::Integer32, v);
        } else if (String::equals(registerStr, "time", true)) {
            DateTime tMinValue, tMaxValue;
            String rangeStr = String::trim(style["range"], '\'', '"', ' ');
            if (String::equals(rangeStr, "today", true)) {
                DateTime now = DateTime::now();
                tMinValue = now.date();
                tMaxValue = now.date().addDays(1);
//            printf("today minValue: %s, maxValue: %s\n", tMinValue.toString().c_str(), tMaxValue.toString().c_str());
            } else if (String::equals(rangeStr, "yesterday", true)) {
                DateTime now = DateTime::now();
                tMinValue = now.date().addDays(-1);
                tMaxValue = now.date();
//            printf("yesterday minValue: %s, maxValue: %s\n", tMinValue.toString().c_str(), tMaxValue.toString().c_str());
            } else if (String::equals(rangeStr, "thismonth", true)) {
                DateTime now = DateTime::now();
                tMinValue = DateTime(now.year(), now.month(), 1);
                tMaxValue = tMinValue.addMonths(1);
//            printf("yesterday minValue: %s, maxValue: %s\n", tMinValue.toString().c_str(), tMaxValue.toString().c_str());
            } else if (String::equals(rangeStr, "datetime", true) ||
                       String::equals(rangeStr, "month", true)) {
                DateTime::parse(filter.getFromValue(name), tMinValue);
                DateTime::parse(filter.getToValue(name), tMaxValue);
            } else {
                StringArray ranges;
                StringArray::parse(rangeStr, ranges, '~');
                if (ranges.count() == 2) {
                    DateTime::parse(ranges[0], tMinValue);
                    DateTime::parse(ranges[1], tMaxValue);
                    DateTime now = DateTime::now();
                    static const DateTime AD1 = DateTime(1, 1, 1);
                    if (tMinValue.date() == AD1) {
                        tMinValue = now.date() + tMinValue.timeOfDay();
                    }
                    if (tMaxValue.date() == AD1) {
                        tMaxValue = now.date() + tMaxValue.timeOfDay();
                    }
//                printf("min time: %s, max time: %s\n", tMinValue.toString().c_str(), tMaxValue.toString().c_str());
                }
            }
            TimeSpan tStep;
            TimeSpan::parse(String::trim(style["step"], '\'', '"', ' '), tStep);
            String formatStr = String::trim(style["format"], '\'', '"', ' ');
            if (!(tMinValue == DateTime::MinValue && tMaxValue == DateTime::MaxValue &&
                  tStep == TimeSpan::Zero && !formatStr.isNullOrEmpty())) {
                bool upToNow = false;
                Boolean::parse(style["upToNow"], upToNow);
                if (upToNow) {
                    tMaxValue = DateTime::now();
                }
                DateTime v = tMinValue.addTicks(tStep.ticks() * row);
//            printf("name: %s, v: %s, step: %s\n", name.c_str(), v.toString().c_str(), tStep.toString().c_str());
                if (v <= tMaxValue) {
                    result = Variant(Variant::Text, v.toString(formatStr));
                }
            }
        } else if (String::equals(registerStr, "array", true)) {
            StringArray texts;
            StringArray::parse(style.toString(), texts, ',');
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
                result = Variant(Variant::Text, String::trim(texts[row], '\'', '"', ' '));
            } else {
                result = Variant(Variant::Text, String::Empty);
            }
        } else if (String::equals(registerStr, "map", true)) {
            String key = String::trim(style["key"], '\'', '"', ' ');
            if (key.isNullOrEmpty()) {
                key = String::trim(style["_key"], '\'', '"', ' ');
            }
            if (key.isNullOrEmpty()) {
                key = String::trim(style["keyName"], '\'', '"', ' ');
            }
            if (key.isNullOrEmpty()) {
                key = String::trim(style["_keyName"], '\'', '"', ' ');
            }

            if (!key.isNullOrEmpty()) {
                String value = filter.getValue(key);
                String arrayStr = String::trim(style[value], '\'', '"', ' ');
                StringArray texts;
                StringArray::parse(arrayStr, texts, ',');
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
                    result = Variant(Variant::Text, String::trim(texts[row], '\'', '"', ' '));
                } else {
                    result = Variant(Variant::Text, String::Empty);
                }
            }
        } else if (String::equals(registerStr, "uuid", true)) {
            result = Variant(Variant::Text, Uuid::generate().toString());
        } else if (String::equals(registerStr, "incoming", true)) {
            result = Variant(Variant::Text, filter.getValue(name));
        } else {
            double v;
            if (Double::parse(registerStr, v)) {
                result = Variant(Variant::Float64, v);
            } else {
                result = Variant(Variant::Text, registerStr);
            }
        }
        return result;
    }
}

DbType Column::type() const {
    if (String::equals(registerStr, "time", true) ||
        String::equals(registerStr, "array", true) ||
        String::equals(registerStr, "uuid", true) ||
        String::equals(registerStr, "incoming", true)) {
        return Variant::Text;
    } else if (String::equals(registerStr, "increase", true) ||
               String::equals(registerStr, "decrease", true) ||
               String::equals(registerStr, "random", true) ||
               String::equals(registerStr, "sin", true) ||
               String::equals(registerStr, "cos", true)) {
        return Variant::Float64;
    } else if (String::equals(registerStr, "onoff", true)) {
        return Variant::Integer32;
    } else {
        double v;
        if (Double::parse(registerStr, v)) {
            return Variant::Float64;
        } else {
            return Variant::Text;
        }
    }
}

Columns::Columns() = default;

Columns::~Columns() = default;

bool Columns::contains(const StringArray &names) const {
    for (int i = 0; i < names.count(); ++i) {
        const String &name = names[i];
        bool found = false;
        for (int j = 0; j < count(); ++j) {
            const Column &column = at(j);
            if (String::equals(column.name, name, true)) {
                found = true;
                break;
            }
        }
        if (!found) {
            return false;
        }
    }
    return true;
}

bool Columns::atByName(const String &name, Column &column) const {
    for (int i = 0; i < count(); ++i) {
        const Column &col = at(i);
        if (String::equals(col.name, name, true)) {
            column = col;
            return true;
        }
    }
    return false;
}

Row::Row(const String &rowStr) {
    StringArray cells;
    StringArray::parse(rowStr, cells, ',');
    for (size_t i = 0; i < cells.count(); ++i) {
        _cells.add(cells[i].trim('\'', '"', ' '));
    }
}

Row::Row(const StringArray &cells) : _cells(cells) {
}

Row::Row(const Row &row) : _cells(row._cells) {
}

bool Row::equals(const Row &other) const {
    return _cells.equals(other._cells);
}

void Row::evaluates(const Row &other) {
    _cells.evaluates(other._cells);
}

const StringArray &Row::cells() const {
    return _cells;
}

String Row::toString() const {
    return _cells.toString(',');
}

Table::Table() : Element(), _rowCount(0) {
}

Table::Table(const Table &other) : Element(other), _rowCount(0) {
    Table::evaluates(other);
}

bool Table::equals(const Table &other) const {
    return Element::equals(other) &&
           this->_rowCount == other._rowCount &&
           this->columns == other.columns &&
           this->rows == other.rows;
}

void Table::evaluates(const Table &other) {
    Element::evaluates(other);
    this->_rowCount = other._rowCount;
    this->columns = other.columns;
    this->rows = other.rows;
}

DataRow Table::toDataRow(const DataTable &table) const {
    return DataRow({
                           DataCell(table.columns()["name"], name),
                           DataCell(table.columns()["range"], toRangeStr()),
                           DataCell(table.columns()["step"], step),
                           DataCell(table.columns()["rowCount"], _rowCount),
                           DataCell(table.columns()["columns"], toColumnsStr()),
                           DataCell(table.columns()["rows"], toRowsStr()),
                   });
}

String Table::toValuesStr() const {
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

bool Table::findItemName(const StringArray &itemNames) const {
    if (itemNames.isEmpty()) {
        return true;
    }

    for (size_t i = 0; i < itemNames.count(); ++i) {
        const String &itemName = itemNames[i];
        for (size_t j = 0; j < columns.count(); ++j) {
            if (columns[j].name.find(itemName) >= 0) {
                return true;
            }
        }
    }

    return false;
}

JsonNode Table::toJsonNode() const {
    JsonNode node;
    node.add(JsonNode("name", name));
    node.add(JsonNode("range", toRangeStr()));
    node.add(JsonNode("step", step));
    node.add(JsonNode("rowCount", _rowCount));
    node.add(JsonNode("columns", toColumnsStr()));
    node.add(JsonNode("Rows", toRowsStr()));
    return node;
}

int Table::rowCount() const {
    return rows.count() > 0 ? (int) rows.count() : _rowCount;
}

String Table::toColumnsStr() const {
    return toValuesStr();
}

String Table::toRowsStr() const {
    JsonNode result("rows", JsonNode::TypeArray);
    for (size_t i = 0; i < rows.count(); ++i) {
        const Row &row = rows[i];
        JsonNode node;
        node.add(JsonNode("cells", row.toString()));
        result.add(node);
    }
    return result.toString();
}

bool Table::getColumns(const StringArray &colNames, Columns &cols) const {
    if (colNames.isEmpty()) {
        cols.addRange(columns);
    } else {
        for (size_t i = 0; i < colNames.count(); ++i) {
            const String &colName = colNames[i];
            Column col;
            if (columns.atByName(colName, col)) {
                cols.add(col);
            }
        }
    }
    return cols.count() > 0;
}

void Table::updateYmlProperties(YmlNode::Properties &properties, int pos) const {
    properties.add(String::format(TablePrefix "name", pos), name);
    properties.add(String::format(TablePrefix "range", pos), toRangeStr());
    properties.add(String::format(TablePrefix "step", pos), step);
    properties.add(String::format(TablePrefix "rowCount", pos), _rowCount);
    for (size_t i = 0; i < columns.count(); ++i) {
        const Column &column = columns[i];
        properties.add(String::format(ColumnPrefix "name", pos, i), column.name);
        properties.add(String::format(ColumnPrefix "register", pos, i), column.registerStr);
        properties.add(String::format(ColumnPrefix "style", pos, i), column.style);
    }
    for (size_t i = 0; i < rows.count(); ++i) {
        const Row &row = rows[i];
        properties.add(String::format(RowPrefix "row", pos, i), row.toString());
    }
}

void Table::removeYmlProperties(YmlNode::Properties &properties, int pos) const {
    properties.remove(String::format(TablePrefix "name", pos));
    properties.remove(String::format(TablePrefix "range", pos));
    properties.remove(String::format(TablePrefix "step", pos));
    properties.remove(String::format(TablePrefix "rowCount", pos));
    properties.remove(String::format(TablePrefix "columns", pos));
    for (size_t i = 0; i < columns.count(); ++i) {
        const Column &column = columns[i];
        properties.remove(String::format(ColumnPrefix "name", pos, i));
        properties.remove(String::format(ColumnPrefix "register", pos, i));
        properties.remove(String::format(ColumnPrefix "style", pos, i));
    }
    properties.remove(String::format(TablePrefix "rows", pos));
    for (size_t i = 0; i < rows.count(); ++i) {
        const Row &row = rows[i];
        properties.remove(String::format(RowPrefix "row", pos, i));
    }
}

bool Table::parseColumns(const String &str, Columns &columns) {
    // str such like [{"name":"time","register":"time","style":{"range":"today","step":"01:00:00","format":"G"}},
    // {"name":"tag1","register":"increase"},{"name":"tag2","register":"decrease"}]
    JsonNode columnsNode;
    if (JsonNode::parse(str, columnsNode)) {
        for (size_t i = 0; i < columnsNode.count(); ++i) {
            JsonNode node = columnsNode.at(i);
            Column column;
            column.name = node.getAttribute("name").trim();
            column.registerStr = node.getAttribute("register").trim();
            Style::parse(node.getAttribute("style"), column.style);
            columns.add(column);
        }
        return columns.count() > 0;
    }
    return false;
}

bool Table::parseRows(const String &str, Rows &rows) {
    JsonNode rowsNode;
    if (JsonNode::parse(str, rowsNode)) {
        for (size_t i = 0; i < rowsNode.count(); ++i) {
            JsonNode node = rowsNode.at(i);
            Row row(node.getAttribute("cells").trim());
            rows.add(row);
        }
        return rows.count() > 0;
    }
    return false;
}

bool Table::parse(const StringMap &request, Table &table) {
    if (!request.at("name", table.name)) {
        return false;
    }
    if (!Label::parseRange(request["range"], table.minValue, table.maxValue)) {
        return false;
    }
    if (!Double::parse(request["step"], table.step)) {
        return false;
    }
    if (!Int32::parse(request["rowCount"], table._rowCount)) {
        return false;
    }
    if (!Table::parseColumns(request["columns"], table.columns)) {
        return false;
    }
    if (!Table::parseRows(request["rows"], table.rows)) {
        return false;
    }
    return true;
}

bool Table::parse(const YmlNode::Properties &properties, int pos, Table &table) {
    String name;
    if (properties.at(String::format(TablePrefix "name", pos), name)) {
        table.name = name;
        properties.at(String::format(TablePrefix "enable", pos), table.enable);
        String range;
        properties.at(String::format(TablePrefix "range", pos), range);
        Table::parseRange(range, table.minValue, table.maxValue);
        properties.at(String::format(TablePrefix "step", pos), table.step);
        properties.at(String::format(TablePrefix "rowCount", pos), table._rowCount);
        for (int j = 0; j < MaxColumnCount; j++) {
            Column column;
            if (!properties.at(String::format(ColumnPrefix "name", pos, j), column.name)) {
                break;
            }
            properties.at(String::format(ColumnPrefix "register", pos, j), column.registerStr);
            properties.at(String::format(ColumnPrefix "style", pos, j), column.style);
            table.columns.add(column);
        }
        for (int j = 0; j < MaxRowCount; j++) {
            String rowStr;
            if (!properties.at(String::format(RowPrefix "row", pos, j), rowStr)) {
                break;
            }
            Row row(rowStr);
            table.rows.add(row);
        }
        return true;
    }
    return false;
}

bool Table::parse(const DataRow &row, Table &table) {
    if (!row["name"].value().getValue(table.name)) {
        return false;
    }
    if (!row["row_count"].value().getValue(table._rowCount)) {
        return false;
    }
    if (!row["min_value"].value().getValue(table.minValue)) {
        return false;
    }
    if (!row["max_value"].value().getValue(table.maxValue)) {
        return false;
    }
    if (!row["step"].value().getValue(table.step)) {
        return false;
    }
    if (!Table::parseColumns(row["columns"].valueStr(), table.columns)) {
        return false;
    }
    if (!Table::parseRows(row["rows"].valueStr(), table.rows)) {
        return false;
    }
    return true;
}

String Table::toInsertSqlStr(const String &prefix) const {
    String sql;
    DateTime now = DateTime::now();
    uint64_t lid = SnowFlake::generateId();
    String tableTableName = getTableName(prefix, TableTableName);
    String columnTableName = getTableName(prefix, ColumnTableName);
    sql = String::format("INSERT INTO %s VALUES(%lld,'%s',%d,%llg,%llg,%llg,'%s','','%s','');",
                         tableTableName.c_str(),
                         lid, name.c_str(),
                         _rowCount, minValue, maxValue, step,
                         now.toString().c_str(), now.toString().c_str());
    for (size_t i = 0; i < columns.count(); ++i) {
        const Column &column = columns[i];
        sql.appendLine(String::format("INSERT INTO %s VALUES(%lld,%lld,'%s','%s','%s','%s','','%s','');",
                                      columnTableName.c_str(),
                                      SnowFlake::generateId(), lid,
                                      column.name.c_str(), column.registerStr.c_str(), column.style.toString().c_str(),
                                      now.toString().c_str(), now.toString().c_str()));
    }
    return sql;
}

String Table::toReplaceSqlStr(const String &prefix) const {
    String sql;
    DateTime now = DateTime::now();
    String tableTableName = getTableName(prefix, TableTableName);
    String columnTableName = getTableName(prefix, ColumnTableName);
    sql = String::format(
            "UPDATE %s SET row_count=%d,min_value=%llg,max_value=%llg,step=%llg,update_time='%s',update_user='' WHERE name='%s';",
            tableTableName.c_str(),
            _rowCount, minValue, maxValue, step,
            now.toString().c_str(), name.c_str());
    String idSelectStr = String::format("(SELECT id FROM %s WHERE name='%s')", tableTableName.c_str(), name.c_str());
    sql.appendLine(String::format("DELETE FROM %s WHERE table_id=%s", columnTableName.c_str(), idSelectStr.c_str()));
    for (size_t i = 0; i < columns.count(); ++i) {
        const Column &column = columns[i];
        sql.appendLine(String::format("INSERT INTO %s VALUES(%lld,%s,'%s','%s','%s','%s','','%s','');",
                                      columnTableName.c_str(),
                                      SnowFlake::generateId(),
                                      idSelectStr.c_str(),
                                      column.name.c_str(), column.registerStr.c_str(), column.style.toString().c_str(),
                                      now.toString().c_str(), now.toString().c_str()));
    }
    return sql;
}

String Table::toSelectSqlStr(const String &prefix, const String &name) {
    String tableTableName = getTableName(prefix, TableTableName);
    String columnTableName = getTableName(prefix, ColumnTableName);
    String columnsSql = String::format("(SELECT JSON_ARRAYAGG(JSON_OBJECT("
                                       "'name',NAME,'register',register,'style',style))"
                                       " FROM %s WHERE %s.table_id=%s.id) columns",
                                       columnTableName.c_str(), columnTableName.c_str(),
                                       tableTableName.c_str());
    String sql = String::format("SELECT id,row_count,name,min_value,max_value,step,%s FROM %s WHERE name='%s' LIMIT 1",
                                columnsSql.c_str(),
                                tableTableName.c_str(),
                                name.c_str());
    return sql;
}

String Table::toSelectSqlStr(const String &prefix, const SqlSelectFilter &filter) {
//        SELECT name,concat(concat(CAST(min_value as CHAR(64)),'-'),CAST(max_value as CHAR(64)))
//        'range',step,(SELECT JSON_ARRAYAGG(JSON_OBJECT('name',NAME,'register',register,'style',style)) FROM column
//        WHERE column.label_id=table.id AND name LIKE '%column%') columns FROM table WHERE 1=1 ORDER BY name asc LIMIT 0,100
    String tableTableName = getTableName(prefix, TableTableName);
    String columnTableName = getTableName(prefix, ColumnTableName);
    static const char *rangeStr = "concat(concat(CAST(min_value as CHAR(64)),'-'),"
                                  "CAST(max_value as CHAR(64))) 'range'";
    String columnsSql = String::format("(SELECT JSON_ARRAYAGG(JSON_OBJECT("
                                       "'name',NAME,'register',register,'style',style))"
                                       " FROM %s WHERE %s.table_id=%s.id AND %s) columns",
                                       columnTableName.c_str(), columnTableName.c_str(),
                                       tableTableName.c_str(),
                                       filter.toArrayLikeStr("columns", "name").c_str());
    String sql = String::format("SELECT name,row_count,%s,step,%s FROM %s WHERE %s ORDER BY %s LIMIT %d,%d",
                                rangeStr,
                                columnsSql.c_str(),
                                tableTableName.c_str(),
                                filter.toLikeStr("name").c_str(),
                                !filter.orderBy().isNullOrEmpty() ? filter.orderBy().c_str() : "name asc",
                                filter.offset(), filter.limit());
    return sql;
}

String Table::toCountSqlStr(const String &prefix, const SqlSelectFilter &filter) {
    String tableTableName = getTableName(prefix, TableTableName);
    String sql = String::format("SELECT COUNT(1) FROM %s WHERE %s",
                                tableTableName.c_str(),
                                filter.toLikeStr("name").c_str());
    return sql;
}

String Table::toDeleteSqlStr(const String &prefix, const String &tableName) {
    String sql;
    String tableTableName = getTableName(prefix, TableTableName);
    String columnTableName = getTableName(prefix, ColumnTableName);
    String idSelectStr = String::format("(SELECT id FROM %s WHERE name='%s')", tableTableName.c_str(),
                                        tableName.c_str());
    sql.appendLine(String::format("DELETE FROM %s WHERE table_id=%s;",
                                  columnTableName.c_str(),
                                  idSelectStr.c_str()));
    sql.appendLine(String::format("DELETE FROM %s WHERE name='%s';",
                                  tableTableName.c_str(),
                                  tableName.c_str()));
    return sql;
}

String Table::getTableName(const String &prefix, const String &tableName) {
    return prefix.isNullOrEmpty() ? tableName : String::format("%s.%s", prefix.c_str(), tableName.c_str());
}