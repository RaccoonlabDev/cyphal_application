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

std::array<CyphalPublisher*, 10> CyphalPublisher::publishers;
uint8_t CyphalPublisher::publishers_amount{0};


void CyphalPublisher::setPortId(CanardPortID port_id) {
    transfer_metadata.port_id = port_id;
}

CanardPortID CyphalPublisher::getPortId() {
    return transfer_metadata.port_id;
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

void PortListPublisher::publish() {
#if PORT_LIST_PUBLISHER
    auto crnt_time_ms = HAL_GetTick();
    if (crnt_time_ms < next_pub_time_ms) {
        return;
    }
    next_pub_time_ms = crnt_time_ms + 5000;

    static uavcan_node_port_List_0_1 msg{};
    uavcan_node_port_List_0_1_initialize_(&msg);
    uavcan_node_port_SubjectIDList_0_1_select_sparse_list_(&msg.publishers);
    uavcan_node_port_SubjectIDList_0_1_select_sparse_list_(&msg.subscribers);

    uint_fast8_t enabled_pub_amount = 0;
    for (uint_fast8_t pub_idx = 0; pub_idx < CyphalPublisher::publishers_amount; pub_idx++) {
        if (CyphalPublisher::publishers[pub_idx]->isEnabled()) {
            auto port_id = static_cast<uint16_t>(CyphalPublisher::publishers[pub_idx]->getPortId());
            msg.publishers.sparse_list.elements[enabled_pub_amount].value = port_id;
            enabled_pub_amount++;
        }
    }
    msg.publishers.sparse_list.count = enabled_pub_amount;

    uint_fast8_t enabled_sub_amount = 0;
    for (uint_fast8_t sub_idx = 0; sub_idx < driver->_sub_num; sub_idx++) {
        if (driver->_sub_info[sub_idx]->isEnabled()) {
            auto port_id = driver->_sub_info[sub_idx]->port_id;
            msg.subscribers.sparse_list.elements[enabled_sub_amount].value = port_id;
            enabled_sub_amount++;
        }
    }
    msg.subscribers.sparse_list.count = enabled_sub_amount;

    static uint8_t buffer[uavcan_node_port_List_0_1_EXTENT_BYTES_];
    size_t buffer_size = uavcan_node_port_List_0_1_EXTENT_BYTES_;
    int32_t result = uavcan_node_port_List_0_1_serialize_(&msg, buffer, &buffer_size);
    if (NUNAVUT_SUCCESS == result) {
        driver->push(&transfer_metadata, buffer_size, buffer);
    }
#endif  // PORT_LIST_PUBLISHER
}
