/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#include "storage.h"

enum class IntParamsIndexes {
    ID,
};

static IntegerDesc_t integer_desc_pool[] __attribute__((unused)) = {
    {(uint8_t*)"id",                        0,      127,    50},
};
