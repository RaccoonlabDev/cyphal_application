/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

/**
 * @file cyphal_subscribers.cpp
 * @author d.ponomarev
 * @date Aug 12, 2022
 */

#include "cyphal_subscribers.hpp"
#include "cyphal.hpp"
#include "stm32f1xx_hal.h"
extern "C" {
#include "storage.h"
}


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

    static uint8_t buffer[uavcan_node_GetInfo_Response_1_0_EXTENT_BYTES_];
    size_t buffer_size = uavcan_node_GetInfo_Response_1_0_EXTENT_BYTES_;
    int32_t result;
    result = uavcan_node_GetInfo_Response_1_0_serialize_(&node_status, buffer, &buffer_size);
    if (NUNAVUT_SUCCESS == result) {
        result = driver->push(&transfer_metadata, buffer_size, buffer);
        if (result < 0) {
            asm("NOP");
        }
    }
}

void ExecuteCommandSubscriber::callback(const CanardRxTransfer& transfer) {
    const uint8_t* payload = static_cast<const uint8_t*>(transfer.payload);
    size_t payload_len = transfer.payload_size;
    uavcan_node_ExecuteCommand_Request_1_0 msg;
    if (uavcan_node_ExecuteCommand_Request_1_0_deserialize_(&msg, payload, &payload_len) < 0) {
        return;
    }

    switch (msg.command) {
        case uavcan_node_ExecuteCommand_Request_1_0_COMMAND_RESTART:
            HAL_NVIC_SystemReset();
            break;

        case uavcan_node_ExecuteCommand_Request_1_0_COMMAND_STORE_PERSISTENT_STATES:
            paramsLoadToFlash();
            break;

        case uavcan_node_ExecuteCommand_Request_1_0_COMMAND_POWER_OFF:
        case uavcan_node_ExecuteCommand_Request_1_0_COMMAND_BEGIN_SOFTWARE_UPDATE:
        case uavcan_node_ExecuteCommand_Request_1_0_COMMAND_FACTORY_RESET:
        case uavcan_node_ExecuteCommand_Request_1_0_COMMAND_EMERGENCY_STOP:
        default:
            break;
    }
}
