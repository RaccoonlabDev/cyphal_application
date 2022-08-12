/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

/**
 * @file cyphal_transport_can.cpp
 * @author d.ponomarev
 * @date Jul 07, 2022
 */

#include "cyphal_transport_can.hpp"
#include <string.h>
#include "bxcan.h"
#include "stm32f1xx_hal.h"


bool CyphalTransportCan::init() {
    BxCANTimings timings;
    uint32_t target_bitrate = 1000000;
    uint32_t peripheral_clock_rate = HAL_RCC_GetPCLK1Freq();

    if (!bxCANComputeTimings(peripheral_clock_rate, target_bitrate, &timings) ||
            !bxCANConfigure(0, timings, false)) {
        return false;
    }

    return true;
}

bool CyphalTransportCan::receive(CanardFrame* can_frame) {
    const uint8_t iface_index = 0;
    uint32_t out_extended_can_id;
    size_t out_payload_size;
    static uint8_t out_payload[256];
    memset(out_payload, 0x00, 256);
    if (bxCANPop(iface_index, &out_extended_can_id, &out_payload_size, out_payload)) {

        can_frame->extended_can_id = out_extended_can_id;
        can_frame->payload_size = out_payload_size;
        can_frame->payload = out_payload;
        return true;
    }
    return false;
}

bool CyphalTransportCan::transmit(const CanardTxQueueItem* transfer) {
    if (transfer->frame.payload_size == 0) {
        return false;
    }

    const uint64_t current_time = HAL_GetTick() * 1000;
    const uint64_t deadline = current_time + 100 * 1000;
    const uint8_t iface_index = 0;

    size_t num_of_frames = transfer->frame.payload_size / 8;
    size_t length_of_last_frame = transfer->frame.payload_size % 8;
    if (length_of_last_frame == 0) {
        length_of_last_frame = 8;
    } else {
        num_of_frames++;
    }

    bool result = false;
    for (size_t frame_idx = 0; frame_idx < num_of_frames; frame_idx++) {
        uint8_t payload_size = (frame_idx + 1 < num_of_frames) ? 8 : length_of_last_frame;
        result = bxCANPush(iface_index,
                           current_time,
                           deadline,
                           transfer->frame.extended_can_id,
                           payload_size,
                           (void*)(((uint8_t*)transfer->frame.payload) + frame_idx * 8));
        if (!result) {
            break;
        }

        ///< we need to have a delay between each push
        for (uint32_t idx = 0; idx < 1000; idx++) {
            asm("NOP");
        }
    }
    return result;
}
