//
//  DataContext.cpp
//  tserver
//
//  Created by baowei on 2022/12/17.
//  Copyright © 2022 com. All rights reserved.
//

#include "DataContext.h"

HttpCode::HttpCode() {
    registerCode({
                         {Success, String::Empty},
                         {JsonParseError, "Json string parse error."},
                         {Unknown, "Unknown"}
                 });
}

void HttpCode::registerCode(int code, const String &msg) {
    _codes[code] = msg;
}

void HttpCode::registerCode(std::initializer_list<Item> list) {
    for (const Item *code = list.begin(); code < list.end(); ++code) {
        _codes[code->code] = code->msg;
    }
}

String HttpCode::getMessage(int code) const {
    return _codes[code];
}

StringMap HttpCode::at(int code) const {
    StringMap result;
    if (_codes.contains(code)) {
        result["code"] = Int32(code).toString();
        result["msg"] = _codes[code];
    }
    return result;
}

HttpCode *HttpCode::instance() {
    return Singleton<HttpCode>::instance();
}