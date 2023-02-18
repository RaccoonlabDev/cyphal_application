/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

/**
 * @file application.cpp
 * @author d.ponomarev
 * @date Jul 11, 2022
 */

#include "application.hpp"
#include "cyphal.hpp"
#include "cyphal_registers.hpp"
#include "main.h"
#include "string_params.hpp"
#include "storage.h"

void application_entry_point() {
    paramsInit(static_cast<uint8_t>(IntParamsIndexes::INTEGER_PARAMS_AMOUNT), NUM_OF_STR_PARAMS);
    paramsLoadFromFlash();

    Cyphal cyphal;
    cyphal.init();

    while (true) {
        cyphal.process();
    }
}
