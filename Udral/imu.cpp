/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2023 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#include "imu.hpp"
#include "main.h"
#include "params.hpp"

namespace cyphal {

void ImuAccelPublisher::publish(const uavcan_si_unit_acceleration_Vector3_1_0& msg) {
    setPortId(paramsGetIntegerValue(IntParamsIndexes::IMU_ACCEL_ID));

    static uint8_t buffer[uavcan_si_unit_acceleration_Vector3_1_0_EXTENT_BYTES_];
    size_t buffer_size = uavcan_si_unit_acceleration_Vector3_1_0_EXTENT_BYTES_;
    int32_t result = uavcan_si_unit_acceleration_Vector3_1_0_serialize_(&msg, buffer, &buffer_size);
    if (NUNAVUT_SUCCESS == result) {
        push(buffer_size, buffer);
    }
}

void ImuGyroPublisher::publish(const uavcan_si_unit_angular_velocity_Vector3_1_0& msg) {
    setPortId(paramsGetIntegerValue(IntParamsIndexes::IMU_GYRO_ID));

    static uint8_t buffer[uavcan_si_unit_angular_velocity_Vector3_1_0_EXTENT_BYTES_];
    size_t buffer_size = uavcan_si_unit_angular_velocity_Vector3_1_0_EXTENT_BYTES_;
    int32_t result = uavcan_si_unit_angular_velocity_Vector3_1_0_serialize_(&msg, buffer, &buffer_size);
    if (NUNAVUT_SUCCESS == result) {
        push(buffer_size, buffer);
    }
}

void RawImuPublisher::publish(const std::array<double, 3>& accel, const std::array<double, 3>& gyro) {
    if (!isEnabled()) {
        return;
    }

    uavcan_primitive_array_Real16_1_0 msg;
    msg.value.count = 6;
    msg.value.elements[0] = accel[0];
    msg.value.elements[1] = accel[1];
    msg.value.elements[2] = accel[2];
    msg.value.elements[3] = gyro[0];
    msg.value.elements[4] = gyro[1];
    msg.value.elements[5] = gyro[2];

    static uint8_t buffer[uavcan_primitive_array_Real16_1_0_EXTENT_BYTES_];
    size_t buffer_size = uavcan_primitive_array_Real16_1_0_EXTENT_BYTES_;
    int32_t result = uavcan_primitive_array_Real16_1_0_serialize_(&msg, buffer, &buffer_size);
    if (NUNAVUT_SUCCESS == result) {
        push(buffer_size, buffer);
    }
}

}  // namespace cyphal
