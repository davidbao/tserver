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
              public IPositionGetter<const String &, const String &> {
public:
    Style();

    explicit Style(const String &style);

    Style(const Style &other);

    bool equals(const Style &other) const override;

    void evaluates(const Style &other) override;

    const String &at(const String &pos) const override;

    bool isEmpty() const;

    String toString() const;

public:
    static bool parse(const String &str, Style &style);

private:
    StringMap _values;
};

#endif //TSERVER_STYLE_H
