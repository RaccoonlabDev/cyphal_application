/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2023 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#include "imu.hpp"
#include "main.h"
#include "params.hpp"

void ImuAccelPublisher::publish(const uavcan_si_sample_acceleration_Vector3_1_0& msg) {
    setPortId(paramsGetIntegerValue(IntParamsIndexes::IMU_ACCEL_ID));

    static uint8_t buffer[uavcan_si_sample_acceleration_Vector3_1_0_EXTENT_BYTES_];
    size_t buffer_size = uavcan_si_sample_acceleration_Vector3_1_0_EXTENT_BYTES_;
    int32_t result = uavcan_si_sample_acceleration_Vector3_1_0_serialize_(&msg, buffer, &buffer_size);
    if (NUNAVUT_SUCCESS == result) {
        push(buffer_size, buffer);
    }
}

void ImuGyroPublisher::publish(const uavcan_si_sample_angular_velocity_Vector3_1_0& msg) {
    setPortId(paramsGetIntegerValue(IntParamsIndexes::IMU_GYRO_ID));

    static uint8_t buffer[uavcan_si_sample_angular_velocity_Vector3_1_0_EXTENT_BYTES_];
    size_t buffer_size = uavcan_si_sample_angular_velocity_Vector3_1_0_EXTENT_BYTES_;
    int32_t result = uavcan_si_sample_angular_velocity_Vector3_1_0_serialize_(&msg, buffer, &buffer_size);
    if (NUNAVUT_SUCCESS == result) {
        push(buffer_size, buffer);
    }
}
