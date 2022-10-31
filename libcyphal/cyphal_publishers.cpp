/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

/**
 * @file cyphal_publishers.cpp
 * @author d.ponomarev
 * @date Aug 12, 2022
 */

#include "cyphal_publishers.hpp"
#include "cyphal.hpp"
#include "main.h"
extern "C" {
#include "storage.h"
}

void CyphalPublisher::setPortId(CanardPortID port_id) {
    transfer_metadata.port_id = port_id;
}

bool CyphalPublisher::isEnabled() {
    constexpr uint16_t MAX_PORT_ID = 8191;
    uint16_t port_id = transfer_metadata.port_id;
    return (port_id == 0 || port_id > MAX_PORT_ID) ? false : true;
}

void HeartbeatPublisher::publish(const uavcan_node_Heartbeat_1_0& msg) {
    static uint8_t buffer[uavcan_node_Heartbeat_1_0_EXTENT_BYTES_];
    size_t buffer_size = uavcan_node_Heartbeat_1_0_EXTENT_BYTES_;
    int32_t result = uavcan_node_Heartbeat_1_0_serialize_(&msg, buffer, &buffer_size);
    if (NUNAVUT_SUCCESS == result) {
        driver->push(&transfer_metadata, buffer_size, buffer);
    }
}

