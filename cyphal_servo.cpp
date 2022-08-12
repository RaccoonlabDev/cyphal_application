/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

/**
 * @file cyphal_servo.cpp
 * @author d.ponomarev
 * @date Aug 12, 2022
 */

#include "cyphal_servo.hpp"
#include "cyphal.hpp"
#include "stm32f1xx_hal.h"
extern "C" {
#include "servos.h"
}


void SetpointSubscriber::callback(const CanardRxTransfer& transfer) {
    const uint8_t* payload = static_cast<const uint8_t*>(transfer.payload);
    size_t payload_len = transfer.payload_size;
    reg_udral_service_actuator_common_sp_Vector4_0_1 msg;
    if (reg_udral_service_actuator_common_sp_Vector4_0_1_deserialize_(&msg, payload, &payload_len) < 0) {
        return;
    }
    uint32_t crnt_time_ms = HAL_GetTick();
    uavcanServosSetSetpoint(0, 8191*msg.value[0], crnt_time_ms);
    uavcanServosSetSetpoint(1, 8191*msg.value[1], crnt_time_ms);
    uavcanServosSetSetpoint(2, 8191*msg.value[2], crnt_time_ms);
    uavcanServosSetSetpoint(3, 8191*msg.value[3], crnt_time_ms);
    uavcanServosUpdateAllChannelsPwm();
}
