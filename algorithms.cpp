/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

/**
 * @file algorithms.cpp
 * @author d.ponomarev
 * @date Oct 23, 2022
 */

#include "algorithms.hpp"
#include <string.h>

uint8_t strlenSafely(const uint8_t* str, uint8_t max_possible_length) {
    uint8_t length;
    for (length = 0; length < max_possible_length; length++) {
        if (str[length] == '\0' || str[length] == 255) {
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
