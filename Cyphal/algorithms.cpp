/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022-2023 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#include "algorithms.hpp"
#include <string.h>

static bool isAllowedSymbol(char symbol) {
    bool is_allowed;
    if ((symbol >= 'A' && symbol <= 'Z') ||
            (symbol >= 'a' && symbol <= 'z') ||
            (symbol >= '0' && symbol <= '9') ||
            (symbol == '-') ||
            (symbol == '_') ||
            (symbol == ' ') ||
            (symbol == '.') ||
            (symbol == '(') ||
            (symbol == ')')) {
        is_allowed = true;
    } else {
        is_allowed = false;
    }
    return is_allowed;
}

uint8_t strlenSafely(const uint8_t* str, uint8_t max_possible_length) {
    uint8_t length;
    for (length = 0; length < max_possible_length; length++) {
        if (!isAllowedSymbol(str[length])) {
            break;
        }
    }
    return length;
}

uint8_t strcpySafely(uint8_t* dst, const uint8_t* src, uint8_t max_possible_length) {
    uint8_t len = strlenSafely(src, max_possible_length);
    memcpy(dst, src, len);
    return len;
}
