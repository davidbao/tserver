//
//  Style.h
//  tserver
//
//  Created by baowei on 2023/2/22.
//  Copyright (c) 2023 com. All rights reserved.
//

#ifndef TSERVER_STYLE_H
#define TSERVER_STYLE_H

#include "data/String.h"
#include "data/StringMap.h"
#include "data/DataInterface.h"

using namespace Data;

class Style : public IEvaluation<Style>,
              public IEquatable<Style>,
              public IPositionable<String, const String &> {
public:
    Style();

    explicit Style(const String &style);

    Style(const Style &other);

    bool equals(const Style &other) const override;

    void evaluates(const Style &other) override;

    String at(const String &pos) const override;

    String &at(const String &pos) override;

    bool set(const String &pos, const String &value) override;

    bool isEmpty() const;

    String toString() const;

    void addRange(const Style &style);

    bool containsVar(const String &varName) const;

public:
    static bool parse(const String &str, Style &style);

private:
    StringMap _values;
};

#endif //TSERVER_STYLE_H
