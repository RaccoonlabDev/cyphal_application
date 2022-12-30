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
#include "main.h"
#include "params.hpp"
#include "algorithms.hpp"
#include "CircuitStatus/hardware_version.hpp"
extern "C" {
#include "storage.h"
#include "git_hash.h"
#include "git_software_version.h"
}

HardwareVersion hw_type{"Unknown", 0, 0};

bool CyphalSubscriber::isEnabled() {
    constexpr uint16_t MAX_PORT_ID = 8191;
    return (port_id == 0 || port_id > MAX_PORT_ID) ? false : true;
}

NodeGetInfoSubscriber::NodeGetInfoSubscriber(Cyphal* driver_, CanardPortID port_id_) :
        CyphalSubscriber(driver_, port_id_) {
    get_info_response.protocol_version.major = CANARD_CYPHAL_SPECIFICATION_VERSION_MAJOR;
    get_info_response.protocol_version.minor = CANARD_CYPHAL_SPECIFICATION_VERSION_MINOR;

    get_info_response.hardware_version.major = hw_type.hw_major;
    get_info_response.hardware_version.minor = hw_type.hw_minor;

    get_info_response.software_version.major = APP_VERSION_MAJOR;
    get_info_response.software_version.minor = APP_VERSION_MINOR;

    get_info_response.certificate_of_authenticity.count = 0;

    auto uid_u32 = HAL_GetUIDw0();
    get_info_response.unique_id[0] = uid_u32 & 0xFF;
    get_info_response.unique_id[1] = (uid_u32 >> 8) & 0xFF;
    get_info_response.unique_id[2] = (uid_u32 >> 16) & 0xFF;
    get_info_response.unique_id[3] = (uid_u32 >> 24) & 0xFF;

    uid_u32 = HAL_GetUIDw1();
    get_info_response.unique_id[4] = uid_u32 & 0xFF;
    get_info_response.unique_id[5] = (uid_u32 >> 8) & 0xFF;
    get_info_response.unique_id[6] = (uid_u32 >> 16) & 0xFF;
    get_info_response.unique_id[7] = (uid_u32 >> 24) & 0xFF;

    uid_u32 = HAL_GetUIDw2();
    get_info_response.unique_id[8] = uid_u32 & 0xFF;
    get_info_response.unique_id[9] = (uid_u32 >> 8) & 0xFF;
    get_info_response.unique_id[10] = (uid_u32 >> 16) & 0xFF;
    get_info_response.unique_id[11] = (uid_u32 >> 24) & 0xFF;

    get_info_response.software_vcs_revision_id = GIT_HASH;
};


void NodeGetInfoSubscriber::callback(const CanardRxTransfer& transfer) {
    auto node_name_param_idx = static_cast<ParamIndex_t>(IntParamsIndexes::INTEGER_PARAMS_AMOUNT);
    auto node_name = (const uint8_t*)paramsGetStringValue(node_name_param_idx);
    get_info_response.name.count = strcpySafely(get_info_response.name.elements, node_name, 15);
    if (get_info_response.name.count == 0) {
        get_info_response.name.count = strcpySafely(get_info_response.name.elements, (const uint8_t*)hw_type.name, 15);
    }

    CanardTransferMetadata transfer_metadata = {
        .priority       = CanardPriorityNominal,
        .transfer_kind  = CanardTransferKindResponse,
        .port_id        = uavcan_node_GetInfo_1_0_FIXED_PORT_ID_,
        .remote_node_id = transfer.metadata.remote_node_id,
        .transfer_id    = _transfer_id,
    };
    _transfer_id++;

    static uint8_t buffer[uavcan_node_GetInfo_Response_1_0_SERIALIZATION_BUFFER_SIZE_BYTES_] = {};
    size_t buffer_size = uavcan_node_GetInfo_Response_1_0_SERIALIZATION_BUFFER_SIZE_BYTES_;
    int32_t result;
    result = uavcan_node_GetInfo_Response_1_0_serialize_(&get_info_response, buffer, &buffer_size);
    if (NUNAVUT_SUCCESS == result) {
        result = driver->push(&transfer_metadata, buffer_size, buffer);
    }
}

void ExecuteCommandSubscriber::callback(const CanardRxTransfer& transfer) {
    const uint8_t* payload = static_cast<const uint8_t*>(transfer.payload);
    size_t payload_len = transfer.payload_size;
    uavcan_node_ExecuteCommand_Request_1_0 msg;
    if (uavcan_node_ExecuteCommand_Request_1_0_deserialize_(&msg, payload, &payload_len) < 0) {
        return;
    }

    uavcan_node_ExecuteCommand_Response_1_0 cmd_response = {};

    switch (msg.command) {
        case uavcan_node_ExecuteCommand_Request_1_0_COMMAND_RESTART:
            HAL_NVIC_SystemReset();
            cmd_response.status = uavcan_node_ExecuteCommand_Response_1_0_STATUS_SUCCESS;
            break;

        case uavcan_node_ExecuteCommand_Request_1_0_COMMAND_STORE_PERSISTENT_STATES:
            paramsLoadToFlash();
            cmd_response.status = uavcan_node_ExecuteCommand_Response_1_0_STATUS_SUCCESS;
            break;

        case uavcan_node_ExecuteCommand_Request_1_0_COMMAND_FACTORY_RESET:
            paramsResetToDefault();
            cmd_response.status = uavcan_node_ExecuteCommand_Response_1_0_STATUS_SUCCESS;
            break;

        case uavcan_node_ExecuteCommand_Request_1_0_COMMAND_POWER_OFF:
        case uavcan_node_ExecuteCommand_Request_1_0_COMMAND_BEGIN_SOFTWARE_UPDATE:
        case uavcan_node_ExecuteCommand_Request_1_0_COMMAND_EMERGENCY_STOP:
        default:
            cmd_response.status = uavcan_node_ExecuteCommand_Response_1_0_STATUS_BAD_COMMAND;
            break;
    }

    CanardTransferMetadata transfer_metadata = {
        .priority       = CanardPriorityNominal,
        .transfer_kind  = CanardTransferKindResponse,
        .port_id        = uavcan_node_ExecuteCommand_1_0_FIXED_PORT_ID_,
        .remote_node_id = transfer.metadata.remote_node_id,
        .transfer_id    = _transfer_id,
    };
    _transfer_id++;


    uint8_t buffer[uavcan_node_ExecuteCommand_Response_1_0_SERIALIZATION_BUFFER_SIZE_BYTES_] = {};
    size_t buffer_size = uavcan_node_ExecuteCommand_Response_1_0_SERIALIZATION_BUFFER_SIZE_BYTES_;
    int32_t result;
    result = uavcan_node_ExecuteCommand_Response_1_0_serialize_(&cmd_response, buffer, &buffer_size);
    if (NUNAVUT_SUCCESS == result) {
        result = driver->push(&transfer_metadata, buffer_size, buffer);
    }
}
