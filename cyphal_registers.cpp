/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

/**
 * @file cyphal_registers.cpp
 * @author d.ponomarev
 * @date Aug 12, 2022
 */

#include "cyphal_registers.hpp"
extern "C" {
#include "storage.h"
}


IntegerDesc_t __attribute__((weak)) integer_desc_pool[] = {
    // name                                 min     max     default
    {(uint8_t*)"id",                        0,      127,    50},
    {(uint8_t*)"uavcan.pub.setpoint.id",    0,      65535,  65535},
};
IntegerParamValue_t integer_values_pool[sizeof(integer_desc_pool) / sizeof(IntegerDesc_t)];


StringDesc_t __attribute__((weak)) string_desc_pool[1] = {
};
StringParamValue_t string_values_pool[sizeof(string_desc_pool) / sizeof(StringDesc_t)];


void RegisterListRequest::callback(const CanardRxTransfer& transfer) {
    auto index = parseRequest(transfer);
    makeResponse(transfer, index);
}

uint16_t RegisterListRequest::parseRequest(const CanardRxTransfer& transfer) {
    auto payload_len = transfer.payload_size;
    const uint8_t* payload = static_cast<const uint8_t*>(transfer.payload);
    uavcan_register_List_Request_1_0 request_msg = {};
    uavcan_register_List_Request_1_0_deserialize_(&request_msg, payload, &payload_len);
    return request_msg.index;
}

void RegisterListRequest::makeResponse(const CanardRxTransfer& transfer, uint16_t index) {
    CanardTransferMetadata _transfer_metadata;
    uavcan_register_List_Response_1_0 _response_msg = {};

    _transfer_metadata.priority = CanardPriorityNominal;
    _transfer_metadata.transfer_kind = CanardTransferKindResponse;
    _transfer_metadata.port_id = port_id;
    _transfer_metadata.remote_node_id = transfer.metadata.remote_node_id;
    _transfer_metadata.transfer_id = transfer.metadata.transfer_id;

    /// No need to manually clean elements because we explicitly get buffer size
    char* param_name = paramsGetParamName(index);
    _response_msg.name.name.count = strlen(param_name);
    memcpy(_response_msg.name.name.elements, param_name, _response_msg.name.name.count);

    /// It is not enough memory on stack, so use buffer as static
    /// No need to manually clean it because we explicitly get buffer size
    static uint8_t buf[uavcan_register_List_Response_1_0_SERIALIZATION_BUFFER_SIZE_BYTES_];
    size_t buf_size = uavcan_register_List_Response_1_0_SERIALIZATION_BUFFER_SIZE_BYTES_;

    int32_t result = uavcan_register_List_Response_1_0_serialize_(&_response_msg, buf, &buf_size);
    if (NUNAVUT_SUCCESS == result) {
        driver->push(&_transfer_metadata, buf_size, buf);
    }
}


void RegisterAccessRequest::callback(const CanardRxTransfer& transfer) {
    auto reg_index = parseRequest(transfer);
    makeResponse(transfer, reg_index);
}

uint16_t RegisterAccessRequest::parseRequest(const CanardRxTransfer& transfer) {
    auto payload_len = transfer.payload_size;
    const uint8_t* payload = static_cast<const uint8_t*>(transfer.payload);
    uavcan_register_Access_Request_1_0_deserialize_(&_request_msg, payload, &payload_len);

    return paramsGetIndexByName(_request_msg.name.name.elements, _request_msg.name.name.count);
}

void RegisterAccessRequest::makeResponse(const CanardRxTransfer& transfer, int8_t reg_index) {
    CanardTransferMetadata _transfer_metadata;
    uavcan_register_Access_Response_1_0 response_msg = {};

    constexpr uint8_t NATURAL16_TAG = 10;

    _transfer_metadata.priority = CanardPriorityNominal;
    _transfer_metadata.transfer_kind = CanardTransferKindResponse;
    _transfer_metadata.port_id = port_id;
    _transfer_metadata.remote_node_id = transfer.metadata.remote_node_id;
    _transfer_metadata.transfer_id = transfer.metadata.transfer_id;

    // The write operation is performed first
    if (_request_msg.value._tag_ == NATURAL16_TAG && _request_msg.value.natural16.value.count > 0) {
        paramsSetIntegerValue(reg_index, _request_msg.value.natural16.value.elements[0]);
    }

    // On the next step the register will be read regardless of the outcome of the write operation
    response_msg.value.natural16.value.count = 1;
    response_msg.value._tag_ = NATURAL16_TAG;
    if (reg_index < 0) {
        response_msg.value.natural16.value.elements[0] = 65535;
    } else {
        response_msg.value.natural16.value.elements[0] = paramsGetValue(reg_index);
    }

    /// @note: It is not enough memory on stack, so use buffer as static
    static uint8_t buf[uavcan_register_Access_Response_1_0_SERIALIZATION_BUFFER_SIZE_BYTES_];
    size_t buf_size = uavcan_register_Access_Response_1_0_SERIALIZATION_BUFFER_SIZE_BYTES_;
    int32_t result = uavcan_register_Access_Response_1_0_serialize_(&response_msg, buf, &buf_size);
    if (NUNAVUT_SUCCESS == result) {
        driver->push(&_transfer_metadata, buf_size, buf);
    }
}
