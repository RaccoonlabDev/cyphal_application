/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

/**
 * @file algorithms.hpp
 * @author d.ponomarev
 * @date Oct 23, 2022
 */

#ifndef ALGORITHMS_HPP_
#define ALGORITHMS_HPP_

#include <stdint.h>

uint8_t strlenSafely(const uint8_t* str, uint8_t max_possible_length);
uint8_t strcpySafely(uint8_t* dst, const uint8_t* src, uint8_t max_possible_length);

#endif  // ALGORITHMS_HPP_
