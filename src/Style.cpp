//
//  Style.cpp
//  tserver
//
//  Created by baowei on 2023/2/22.
//  Copyright (c) 2023 com. All rights reserved.
//

#include "Style.h"
#include "json/JsonNode.h"

using namespace Json;

Style::Style() = default;

Style::Style(const String &style) {
    parse(style, *this);
}

Style::Style(const Style &other) {
    Style::evaluates(other);
}

bool Style::equals(const Style &other) const {
    return this->_values == other._values;
}

void Style::evaluates(const Style &other) {
    this->_values = other._values;
}

String Style::at(const String &pos) const {
    return _values.at(pos);
}

String &Style::at(const String &pos) {
    return _values.at(pos);
}

bool Style::set(const String &pos, const String &value) {
    return _values.set(pos, value);
}

bool Style::isEmpty() const {
    return _values.count() == 0;
}

String Style::toString() const {
    if (_values.count() == 0) {
        return String::Empty;
    }
    else if (_values.count() == 1) {
        return _values[String::Empty];
    } else {
        JsonNode node("style", _values);
        return node.toString();
    }
//    for (auto it = _values.begin(); it != _values.end(); ++it) {
//        const String &k = it.key();
//        const String &v = it.value();
//        if (!result.isNullOrEmpty()) {
//            result.append(';');
//        }
//        result.append(String::format("%s:%s", k.c_str(), v.c_str()));
//    }
//    return result;
}

void Style::addRange(const Style &style) {
    _values.addRange(style._values);
}

bool Style::containsVar(const String &varName) const {
    for (auto it = _values.begin(); it != _values.end(); ++it) {
        const String &v = it.value();
        if (v.find(String::format("$%s", varName.c_str())) >= 0) {
            return true;
        }
    }
    return false;
}

bool Style::parse(const String &str, Style &style) {
    if (str.isNullOrEmpty()) {
        return false;
    }

    JsonNode node;
    if (JsonNode::parse(str, node)) {
        if (node.type() == JsonNode::TypeNode) {
            StringArray names;
            node.getAttributeNames(names);
            for (size_t i = 0; i < names.count(); ++i) {
                const String &key = names[i];
                String value = node.getAttribute(key);
                style._values.add(key, value);
            }
        } else {
            style._values.add(String::Empty, str);
        }
    } else {
        StringArray texts;
        StringArray::parse(str, texts, ';');
        for (size_t i = 0; i < texts.count(); ++i) {
            const String &text = texts[i];
            ssize_t pos = text.find(':');
            if (pos > 0) {
                String key = text.substr(0, pos).trim('\'', '"', ' ');
                pos++;  // skip :
                String value = text.substr(pos, text.length() - pos).trim('\'', '"', ' ');
                if (!key.isNullOrEmpty()) {
                    style._values.add(key, value);
                }
            } else {
                if (texts.count() == 1) {
                    style._values.add(String::Empty, str);
                    return true;
                } else {
                    return false;
                }
            }
        }
    }
    return style._values.count() > 0;
}
