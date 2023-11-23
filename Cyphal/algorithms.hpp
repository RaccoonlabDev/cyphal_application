/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022-2023 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#ifndef ALGORITHMS_HPP_
#define ALGORITHMS_HPP_

#include <stdint.h>

uint8_t strlenSafely(const uint8_t* str, uint8_t max_possible_length);
uint8_t strcpySafely(uint8_t* dst, const uint8_t* src, uint8_t max_possible_length);

#endif  // ALGORITHMS_HPP_
