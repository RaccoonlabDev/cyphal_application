/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

/**
 * @file cyphal_transport_can.cpp
 * @author d.ponomarev
 * @date Dec 21, 2022
 */

#include "cyphal_transport_can.hpp"
#include <string.h>
#include "socketcan.h"
#include "main.h"

static SocketCANFD _instance = 0;

bool CyphalTransportCan::init(uint32_t can_speed, uint8_t can_driver_idx) {
    _instance = socketcanOpen("vcan0", false);

    if (_instance < 0) {
        return false;
    }

    return true;
}

bool CyphalTransportCan::receive(CanardFrame* can_frame) {
    CanardMicrosecond out_timestamp_usec = 0;
    const size_t payload_buffer_size = 64;
    static uint8_t payload_buffer[payload_buffer_size];
    const CanardMicrosecond timeout_usec = 1000;
    bool loopback = false;

    int16_t res = socketcanPop(_instance,
                               can_frame,
                               &out_timestamp_usec,
                               payload_buffer_size,
                               payload_buffer,
                               timeout_usec,
                               &loopback);

    if (res > 0) {
        can_frame->payload = payload_buffer;
        return true;
    }
    return false;
}

bool CyphalTransportCan::transmit(const CanardTxQueueItem* transfer) {
    if (transfer->frame.payload_size == 0) {
        return false;
    }

    const uint64_t current_time_us = HAL_GetTick() * 1000;
    int16_t errno = socketcanPush(_instance, &transfer->frame, current_time_us);
    return (errno > 0) ? true : false;
}
