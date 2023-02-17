/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

/**
 * @file params.hpp
 * @author d.ponomarev
 * @date Feb 17, 2023
 */

#ifndef PARAMS_HPP_
#define PARAMS_HPP_

enum class IntParamsIndexes {
    ID,
    BAROMETER_PRESSURE_ID,
    BAROMETER_TEMPERATURE_ID,
    CRCT_5V_ID,
    CRCT_VIN_ID,
    CRCT_DEVICE_TEMPERATURE_ID,
    GPS_POINT_ID,
    GPS_YAW_ID,
    GPS_SATS_ID,
    GPS_STATUS_ID,
    GPS_PDOP_ID,
    GPS_SELF_TEST,
    MAGNETOMETER_ID,

    INTEGER_PARAMS_AMOUNT
};

#endif  // PARAMS_HPP_
