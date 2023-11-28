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
#include "storage.h"
extern "C" {
#include "git_hash.h"
#include "git_software_version.h"
}

static const auto DEFAULT_NODE_NAME = (const uint8_t*)"co.raccoonlab.unknown_node";

uavcan_node_Version_1_0 NodeGetInfoSubscriber::hw_version;

bool CyphalSubscriber::isEnabled() const {
    constexpr uint16_t MAX_PORT_ID = 8191;
    return (port_id == 0 || port_id > MAX_PORT_ID) ? false : true;
}

bool CyphalSubscriber::isService() const {
    return false;
}


NodeGetInfoSubscriber::NodeGetInfoSubscriber(Cyphal* driver_) :
        CyphalSubscriber(driver_, uavcan_node_GetInfo_1_0_FIXED_PORT_ID_) {
    get_info_response.protocol_version.major = CANARD_CYPHAL_SPECIFICATION_VERSION_MAJOR;
    get_info_response.protocol_version.minor = CANARD_CYPHAL_SPECIFICATION_VERSION_MINOR;
    get_info_response.software_version.major = APP_VERSION_MAJOR;
    get_info_response.software_version.minor = APP_VERSION_MINOR;

#if !defined(GIT_HASH) || GIT_HASH == 0xBADC0FFEEFFF
    #pragma message "GIT_HASH is not provided"
    get_info_response.software_vcs_revision_id = GIT_HASH;
#endif
}

void NodeGetInfoSubscriber::init() {
    get_info_response.hardware_version.major = hw_version.major;
    get_info_response.hardware_version.minor = hw_version.minor;
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

    updateNodeName();
}

void NodeGetInfoSubscriber::updateNodeName() {
    auto param_idx = static_cast<ParamIndex_t>(IntParamsIndexes::INTEGER_PARAMS_AMOUNT);
    auto custom_name = (const uint8_t*)paramsGetStringValue(param_idx);
    auto& name = get_info_response.name;

    if (custom_name != nullptr) {
        name.count = strcpySafely(name.elements, custom_name, MAX_STRING_LENGTH);
        if (name.count != 0) {
            return;
        }
    }

    name.count = strcpySafely(name.elements, DEFAULT_NODE_NAME, MAX_STRING_LENGTH);
}

void NodeGetInfoSubscriber::callback(const CanardRxTransfer& transfer) {
    CanardTransferMetadata transfer_metadata = {
        CanardPriorityNominal,
        CanardTransferKindResponse,
        uavcan_node_GetInfo_1_0_FIXED_PORT_ID_,
        transfer.metadata.remote_node_id,
        transfer.metadata.transfer_id,
    };

    static uint8_t buffer[uavcan_node_GetInfo_Response_1_0_SERIALIZATION_BUFFER_SIZE_BYTES_] = {};
    size_t buffer_size = uavcan_node_GetInfo_Response_1_0_SERIALIZATION_BUFFER_SIZE_BYTES_;
    int32_t result;
    result = uavcan_node_GetInfo_Response_1_0_serialize_(&get_info_response, buffer, &buffer_size);
    if (NUNAVUT_SUCCESS == result) {
        driver->push(&transfer_metadata, buffer_size, buffer);
    }
}

void NodeGetInfoSubscriber::setHardwareVersion(uint8_t major, uint8_t minor) {
    hw_version.major = major;
    hw_version.minor = minor;
}

bool NodeGetInfoSubscriber::isService() const {
    return true;
}

void ExecuteCommandSubscriber::callback(const CanardRxTransfer& transfer) {
    auto payload = static_cast<const uint8_t*>(transfer.payload);
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
            if (LIBPARAMS_OK == paramsLoadToFlash()) {
                cmd_response.status = uavcan_node_ExecuteCommand_Response_1_0_STATUS_SUCCESS;
            } else {
                cmd_response.status = uavcan_node_ExecuteCommand_Response_1_0_STATUS_FAILURE;
            }
            break;

        case uavcan_node_ExecuteCommand_Request_1_0_COMMAND_FACTORY_RESET:
            paramsResetToDefault();
            if (LIBPARAMS_OK == paramsLoadToFlash()) {
                cmd_response.status = uavcan_node_ExecuteCommand_Response_1_0_STATUS_SUCCESS;
            } else {
                cmd_response.status = uavcan_node_ExecuteCommand_Response_1_0_STATUS_FAILURE;
            }
            break;

        default:
            cmd_response.status = uavcan_node_ExecuteCommand_Response_1_0_STATUS_BAD_COMMAND;
            break;
    }

    CanardTransferMetadata transfer_metadata = {
        CanardPriorityNominal,
        CanardTransferKindResponse,
        uavcan_node_ExecuteCommand_1_0_FIXED_PORT_ID_,
        transfer.metadata.remote_node_id,
        transfer.metadata.transfer_id,
    };

    uint8_t buffer[uavcan_node_ExecuteCommand_Response_1_0_SERIALIZATION_BUFFER_SIZE_BYTES_] = {};
    size_t buffer_size = uavcan_node_ExecuteCommand_Response_1_0_SERIALIZATION_BUFFER_SIZE_BYTES_;
    int32_t result;
    result = uavcan_node_ExecuteCommand_Response_1_0_serialize_(&cmd_response, buffer, &buffer_size);
    if (NUNAVUT_SUCCESS == result) {
        driver->push(&transfer_metadata, buffer_size, buffer);
    }
}

bool ExecuteCommandSubscriber::isService() const {
    return true;
}
