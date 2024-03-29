/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#include "params.hpp"
#include "storage.h"

IntegerDesc_t integer_desc_pool[] = {
    {"id", 0, 127, 50},
    {"uavcan.pub.feedback.id", 2100, 2100, 2100},
};
IntegerParamValue_t integer_values_pool[sizeof(integer_desc_pool) / sizeof(IntegerDesc_t)];

StringDesc_t __attribute__((weak)) string_desc_pool[NUM_OF_STR_PARAMS] = {
    {"name", "", false},
    {"uavcan.pub.feedback.type", "reg.udral.service.actuator.common.Feedback.0.1", true},
};
StringParamValue_t string_values_pool[sizeof(string_desc_pool) / sizeof(StringDesc_t)];
