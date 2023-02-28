/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2023 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#include "gnss.hpp"
#include "reg/udral/physics/kinematics/geodetic/PointStateVarTs_0_1.h"
#include "uavcan/primitive/scalar/Integer16_1_0.h"
#include "main.h"
#include "params.hpp"

void GpsPointPublisher::publish() {
    reg_udral_physics_kinematics_geodetic_PointStateVarTs_0_1 msg;
    msg.timestamp.microsecond = HAL_GetTick() * 1000;

    // ardupilot: 37.40033710, -122.08003510, 0,353
    msg.value.position.value.latitude = 37.40033710 * 1.7453293e-2;
    msg.value.position.value.longitude = -122.08003510 * 1.7453293e-2;
    msg.value.position.value.altitude.meter = 353 / 1000;
    msg.value.velocity.value.meter_per_second[0] = 0.01;
    msg.value.velocity.value.meter_per_second[1] = 0.02;
    msg.value.velocity.value.meter_per_second[2] = 0.03;

    uint8_t buffer[reg_udral_physics_kinematics_geodetic_PointStateVarTs_0_1_EXTENT_BYTES_];
    size_t buffer_size = reg_udral_physics_kinematics_geodetic_PointStateVarTs_0_1_EXTENT_BYTES_;
    int32_t result = reg_udral_physics_kinematics_geodetic_PointStateVarTs_0_1_serialize_(&msg, buffer, &buffer_size);
    if (NUNAVUT_SUCCESS == result) {
        push(buffer_size, buffer);
    }
}

void Int16Publisher::publish(uint16_t value) {
    uavcan_primitive_scalar_Integer16_1_0 msg;
    msg.value = value;

    uint8_t buffer[uavcan_primitive_scalar_Integer16_1_0_EXTENT_BYTES_];
    size_t buffer_size = uavcan_primitive_scalar_Integer16_1_0_EXTENT_BYTES_;
    int32_t result = uavcan_primitive_scalar_Integer16_1_0_serialize_(&msg, buffer, &buffer_size);
    if (NUNAVUT_SUCCESS == result) {
        push(buffer_size, buffer);
    }
}
