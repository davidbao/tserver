//
//  Style.cpp
//  tserver
//
//  Created by baowei on 2023/2/22.
//  Copyright (c) 2023 com. All rights reserved.
//

#include "Style.h"

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

const String &Style::at(const String &pos) const {
    return _values.at(pos);
}

bool Style::isEmpty() const {
    return _values.count() == 0;
}

String Style::toString() const {
    String result;
    if (_values.count() == 1) {
        if (_values.at(String::Empty, result)) {
            return result;
        }
    }

    for (auto it = _values.begin(); it != _values.end(); ++it) {
        const String &k = it.key();
        const String &v = it.value();
        if (!result.isNullOrEmpty()) {
            result.append(';');
        }
        result.append(String::format("%s:%s", k.c_str(), v.c_str()));
    }
    return result;
}

bool Style::parse(const String &str, Style &style) {
    if (str.isNullOrEmpty()) {
        return false;
    }

    StringArray texts;
    StringArray::parse(str, texts, ';');
    for (size_t i = 0; i < texts.count(); ++i) {
//        char keyStr[255] = {0}, valueStr[255] = {0};
        const String &text = texts[i];
        ssize_t pos = text.find(':');
        if (pos > 0) {
            String key = text.substr(0, pos).trim('\'', '"', ' ');
            pos++;  // skip :
            String value = text.substr(pos, text.length() - pos).trim(' ', '\"');
            if (!key.isNullOrEmpty()) {
                style._values.add(key, value);
            }
        } else {
            return false;
        }
//        int rValue = sscanf(text, "%[a-z|A-Z|0-9]:%s", keyStr, valueStr);
//        if (rValue >= 2) {
//            String key = String::trim(keyStr, '\'', '"', ' ');
//            String value = String::trim(valueStr, '\'', '"', ' ');
//            if (!key.isNullOrEmpty()) {
//                style._values.add(key, value);
//            }
//        } else if(rValue == 1) {
//            style._values.add(String::Empty, text);
//        } else {
//            return false;
//        }
    }
    return style._values.count() > 0;
}
