//
//  StyleTest.cpp
//  common
//
//  Created by baowei on 2023/6/2.
//  Copyright (c) 2023 com. All rights reserved.
//

#include "../src/Style.h"
#include "json/JsonNode.h"

using namespace Json;

bool testConstructor() {
    {
        Style test;
        if (!test.isEmpty()) {
            return false;
        }
    }
    {
        Style style("test1, test2, test3");
        if (style.isEmpty()) {
            return false;
        }
        if (style.containsVar(String::Empty)) {
            return false;
        }
        String value = style.at(String::Empty);
        if (value != "test1, test2, test3") {
            return false;
        }
    }
    {
        Style style("test1, test2, test3");
        Style style2(style);
        if (style2 != style) {
            return false;
        }
    }
    {
        Style style("test1, test2, test3");
        Style style2 = style;
        if (style2 != style) {
            return false;
        }
    }

    return true;
}

bool testEquals() {
    {
        Style style("test1, test2, test3");
        Style style2 = style;
        if (style2 != style) {
            return false;
        }
    }
    {
        Style style("test1, test2, test3");
        Style style2 = style;
        if (!style2.equals(style)) {
            return false;
        }
    }

    return true;
}

bool testEvaluates() {
    {
        Style style("test1, test2, test3");
        Style style2 = style;
        if (!style2.equals(style)) {
            return false;
        }
    }
    {
        Style style("test1, test2, test3");
        Style style2;
        style2.evaluates(style);
        if (!style2.equals(style)) {
            return false;
        }
    }

    return true;
}

bool testPositionable() {
    {
        Style style("test1, test2, test3");
        String value = style.at(String::Empty);
        if (value != "test1, test2, test3") {
            return false;
        }
    }
    {
        Style style("test1, test2, test3");
        String &value = style.at(String::Empty);
        if (value != "test1, test2, test3") {
            return false;
        }
        value = "test4, test5, test6";
        String value2 = style.at(String::Empty);
        if (value2 != value) {
            return false;
        }
    }
    {
        Style style("test1, test2, test3");
        String value = style.at(String::Empty);
        if (value != "test1, test2, test3") {
            return false;
        }
        value = "test4, test5, test6";
        if (!style.set(String::Empty, value)) {
            return false;
        }
        String value2 = style.at(String::Empty);
        if (value2 != value) {
            return false;
        }
    }

    return true;
}

bool testToString() {
    {
        Style test;
        if (!test.toString().isNullOrEmpty()) {
            return false;
        }
    }
    {
        Style style("test1, test2, test3");
        String value = style.toString();
        if (value != "test1, test2, test3") {
            return false;
        }
    }
    {
        Style style("range: '00:00:00~23:00:00'; step: '01:00:00'; format: 'HH:mm'; upToNow: true");
        String value = style.toString();
        JsonNode node;
        if (!JsonNode::parse(value, node)) {
            return false;
        }
        if (node.getAttribute("range") != "00:00:00~23:00:00") {
            return false;
        }
        if (node.getAttribute("step") != "01:00:00") {
            return false;
        }
        if (node.getAttribute("format") != "HH:mm") {
            return false;
        }
        if (node.getAttribute("upToNow") != "true") {
            return false;
        }
    }

    return true;
}

bool testAddRange() {
    {
        Style style("range: '00:00:00~23:00:00'; step: '01:00:00'; format: 'HH:mm'; upToNow: true");
        Style style2("test1, test2, test3");
        style.addRange(style2);
        String value = style.at(String::Empty);
        if (value != "test1, test2, test3") {
            return false;
        }
    }
    return true;
}

bool testParse() {
    {
        String str = "test1, test2, test3";
        Style style;
        if (!Style::parse(str, style)) {
            return false;
        }
        String value = style.at(String::Empty);
        if (value != "test1, test2, test3") {
            return false;
        }
    }
    {
        String str = "'01:00:00', '00:01:00', '02:00:00', '00:00:03'";
        Style style;
        if (!Style::parse(str, style)) {
            return false;
        }
        String value = style.at(String::Empty);
        if (value != "'01:00:00', '00:01:00', '02:00:00', '00:00:03'") {
            return false;
        }
    }
    {
        String str = "'2023-03-19 10:11:12', '2023-04-19 10:11:12', '2023-05-19 10:11:12'";
        Style style;
        if (!Style::parse(str, style)) {
            return false;
        }
        String value = style.at(String::Empty);
        if (value != "'2023-03-19 10:11:12', '2023-04-19 10:11:12', '2023-05-19 10:11:12'") {
            return false;
        }
    }
    {
        String str = "range: '00:00:00~23:00:00'; step: '01:00:00'; format: 'HH:mm'; upToNow: true";
        Style style;
        if (!Style::parse(str, style)) {
            return false;
        }
        JsonNode node;
        if (!JsonNode::parse(style.toString(), node)) {
            return false;
        }
        if (node.getAttribute("range") != "00:00:00~23:00:00") {
            return false;
        }
        if (node.getAttribute("step") != "01:00:00") {
            return false;
        }
        if (node.getAttribute("format") != "HH:mm") {
            return false;
        }
        if (node.getAttribute("upToNow") != "true") {
            return false;
        }
    }
    {
        String str = "113715892637925380: '线路1, 线路2, 线路3'; 113715892637925399: '线路4, 线路5, 线路6'; 113715892637925400: '线路7, 线路8'";
        Style style;
        if (!Style::parse(str, style)) {
            return false;
        }
        JsonNode node;
        if (!JsonNode::parse(style.toString(), node)) {
            return false;
        }
        if (node.getAttribute("113715892637925380") != "线路1, 线路2, 线路3") {
            return false;
        }
        if (node.getAttribute("113715892637925399") != "线路4, 线路5, 线路6") {
            return false;
        }
        if (node.getAttribute("113715892637925400") != "线路7, 线路8") {
            return false;
        }
    }

    return true;
}

int main() {
    if (!testConstructor()) {
        return 1;
    }
    if (!testEquals()) {
        return 2;
    }
    if (!testEvaluates()) {
        return 3;
    }
    if (!testPositionable()) {
        return 4;
    }
    if (!testToString()) {
        return 5;
    }
    if (!testAddRange()) {
        return 6;
    }
    if (!testParse()) {
        return 7;
    }

    return 0;
}
