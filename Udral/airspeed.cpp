/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2023 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#include "airspeed.hpp"
#include "main.h"
#include "params.hpp"

namespace cyphal {

void DiffPressurePublisher::publish(float pressure) {
    uavcan_si_sample_pressure_Scalar_1_0 msg;
    msg.pascal = pressure;
    msg.timestamp.microsecond = 0;

    static uint8_t buffer[uavcan_si_sample_pressure_Scalar_1_0_SERIALIZATION_BUFFER_SIZE_BYTES_];
    size_t buffer_size = uavcan_si_sample_pressure_Scalar_1_0_SERIALIZATION_BUFFER_SIZE_BYTES_;
    int32_t result = uavcan_si_sample_pressure_Scalar_1_0_serialize_(&msg, buffer, &buffer_size);
    if (NUNAVUT_SUCCESS == result) {
        push(buffer_size, buffer);
    }
}

}  // namespace cyphal
