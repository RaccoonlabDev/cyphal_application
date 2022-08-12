/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

/**
 * @file cyphal_subscribers.cpp
 * @author d.ponomarev
 * @date Aug 12, 2022
 */

#include "cyphal.hpp"
#include "uavcan/node/GetInfo_1_0.h"


void NodeGetInfoSubscriber::callback(const CanardRxTransfer& transfer) {
    static uint8_t transfer_id = 0;

    // init node status
    uavcan_node_GetInfo_Response_1_0 node_status;
    static auto node_name = "Raccoon";
    memcpy(node_status.name.elements, node_name, 7);
    node_status.name.count = 7;
    node_status.protocol_version.major = CANARD_CYPHAL_SPECIFICATION_VERSION_MAJOR;
    node_status.protocol_version.minor = CANARD_CYPHAL_SPECIFICATION_VERSION_MINOR;
    node_status.hardware_version.major = 0;
    node_status.hardware_version.minor = 1;
    node_status.software_version.major = 0;
    node_status.software_version.minor = 1;
    node_status.certificate_of_authenticity.count = 0;
    node_status.software_image_crc.count = 0;

    const CanardTransferMetadata transfer_metadata = {
        .priority       = CanardPriorityNominal,
        .transfer_kind  = CanardTransferKindResponse,
        .port_id        = uavcan_node_GetInfo_1_0_FIXED_PORT_ID_,
        .remote_node_id = transfer.metadata.remote_node_id,
        .transfer_id    = transfer_id,
    };
    transfer_id++;

    static uint8_t buffer[512];
    size_t buffer_size = 512;
    int32_t result;
    result = uavcan_node_GetInfo_Response_1_0_serialize_(&node_status, buffer, &buffer_size);
    if (NUNAVUT_SUCCESS == result) {
        result = driver->push(&transfer_metadata, buffer_size, buffer);
        if (result < 0) {
            asm("NOP");
        }
    }
}
