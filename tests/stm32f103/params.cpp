/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#include "params.hpp"
#include "string_params.hpp"
#include "storage.h"

IntegerDesc_t integer_desc_pool[] = {
    {(uint8_t*)"id", 0, 127, 50},
    {(uint8_t*)"uavcan.pub.baro.press.id", 0, 65535, 65535},
    {(uint8_t*)"uavcan.pub.baro.temp.id", 0, 65535, 65535},
    {(uint8_t*)"uavcan.pub.crct.5v.id", 0, 65535, 65535},
    {(uint8_t*)"uavcan.pub.crct.vin.id", 0, 65535, 65535},
    {(uint8_t*)"uavcan.pub.crct.temp.id", 0, 65535, 65535},
    {(uint8_t*)"uavcan.pub.gps.point.id", 0, 65535, 65535},
    {(uint8_t*)"uavcan.pub.gps.yaw.id", 0, 65535, 65535},
    {(uint8_t*)"uavcan.pub.gps.sats.id", 0, 65535, 65535},
    {(uint8_t*)"uavcan.pub.gps.status.id", 0, 65535, 65535},
    {(uint8_t*)"uavcan.pub.gps.pdop.id", 0, 65535, 65535},
    {(uint8_t*)"gps_self_test", 0, 1, 0},
    {(uint8_t*)"uavcan.pub.mag.id", 0, 65535, 65535},

};
IntegerParamValue_t integer_values_pool[sizeof(integer_desc_pool) / sizeof(IntegerDesc_t)];

StringDesc_t __attribute__((weak)) string_desc_pool[NUM_OF_STR_PARAMS] = {
    {(uint8_t*)"name", "", true},
    {(uint8_t*)"uavcan.pub.baro.press.type", "uavcan.si.sample.pressure.Scalar", true},
    {(uint8_t*)"uavcan.pub.baro.temp.type", "uavcan.si.sample.temperature.Scalar", true},
    {(uint8_t*)"uavcan.pub.crct.5v.type", "uavcan.si.sample.voltage.Scalar.1.0", true},
    {(uint8_t*)"uavcan.pub.crct.vin.type", "uavcan.si.sample.voltage.Scalar.1.0", true},
    {(uint8_t*)"uavcan.pub.crct.temp.type", "uavcan.si.sample.temperature.Scalar.1.0", true},
    {(uint8_t*)"uavcan.pub.gps.point.type", "reg.udral.physics.kinematics.geodetic.PointStateVarTs", true},
    {(uint8_t*)"uavcan.pub.gps.yaw.type", "uavcan.si.sample.angle.Scalar", true},
    {(uint8_t*)"uavcan.pub.gps.sats.type", "uavcan.primitive.scalar.Integer16", true},
    {(uint8_t*)"uavcan.pub.gps.status.type", "uavcan.primitive.scalar.Integer16", true},
    {(uint8_t*)"uavcan.pub.gps.pdop.type", "uavcan.primitive.scalar.Integer16", true},
    {(uint8_t*)"uavcan.pub.mag.type", "uavcan.si.sample.magnetic_field_strength.Vector3", true},

};
StringParamValue_t string_values_pool[sizeof(string_desc_pool) / sizeof(StringDesc_t)];
