/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

/**
 * @file cyphal_registers.cpp
 * @author d.ponomarev
 * @date Aug 12, 2022
 */

#include "cyphal_registers.hpp"
#include "cyphal.hpp"
#include "algorithms.hpp"
#include "params.hpp"

void RegisterListRequest::callback(const CanardRxTransfer& transfer) {
    auto index = parseRequest(transfer);
    makeResponse(transfer, index);
}

ParamIndex_t RegisterListRequest::parseRequest(const CanardRxTransfer& transfer) const {
    auto payload_len = transfer.payload_size;
    auto payload = static_cast<const uint8_t*>(transfer.payload);
    uavcan_register_List_Request_1_0 request_msg = {};
    uavcan_register_List_Request_1_0_deserialize_(&request_msg, payload, &payload_len);
    return static_cast<ParamIndex_t>(request_msg.index);
}

void RegisterListRequest::makeResponse(const CanardRxTransfer& transfer, ParamIndex_t index) {
    CanardTransferMetadata _transfer_metadata;
    uavcan_register_List_Response_1_0 _response_msg = {};

    if (paramsGetType(index) != PARAM_TYPE_UNDEFINED) {
        auto param_name = (const uint8_t*)(paramsGetParamName(index));
        _response_msg.name.name.count = strlenSafely(param_name, MAX_PARAM_NAME_LENGTH);
        memcpy(_response_msg.name.name.elements, param_name, _response_msg.name.name.count);
    } else {
        _response_msg.name.name.count = 0;
    }

    _transfer_metadata.priority = CanardPriorityNominal;
    _transfer_metadata.transfer_kind = CanardTransferKindResponse;
    _transfer_metadata.port_id = port_id;
    _transfer_metadata.remote_node_id = transfer.metadata.remote_node_id;
    _transfer_metadata.transfer_id = transfer.metadata.transfer_id;

    static uint8_t buf[uavcan_register_List_Response_1_0_SERIALIZATION_BUFFER_SIZE_BYTES_];
    size_t buf_size = uavcan_register_List_Response_1_0_SERIALIZATION_BUFFER_SIZE_BYTES_;

    int32_t result = uavcan_register_List_Response_1_0_serialize_(&_response_msg, buf, &buf_size);
    if (NUNAVUT_SUCCESS == result) {
        driver->push(&_transfer_metadata, buf_size, buf);
    }
}


constexpr uint8_t EMPTY_TAG = 0;
constexpr uint8_t STRING_TAG = 1;
constexpr uint8_t NATURAL32_TAG = 9;


RegisterAccessRequest::RegisterAccessRequest(Cyphal* driver_) :
        CyphalSubscriber(driver_, uavcan_register_Access_1_0_FIXED_PORT_ID_) {
    _transfer_metadata.priority = CanardPriorityNominal;
    _transfer_metadata.transfer_kind = CanardTransferKindResponse;
}


void RegisterAccessRequest::callback(const CanardRxTransfer& transfer) {
    auto reg_index = parseRequest(transfer);
    makeResponse(transfer, reg_index);
}

ParamIndex_t RegisterAccessRequest::parseRequest(const CanardRxTransfer& transfer) {
    auto payload_len = transfer.payload_size;
    auto payload = static_cast<const uint8_t*>(transfer.payload);
    uavcan_register_Access_Request_1_0_deserialize_(&_request_msg, payload, &payload_len);

    return paramsGetIndexByName(_request_msg.name.name.elements,
                                static_cast<uint16_t>(_request_msg.name.name.count));
}

void RegisterAccessRequest::writeParam(ParamIndex_t reg_index) {
    if (_request_msg.value._tag_ == EMPTY_TAG) {
        return;
    }
    auto param_type = paramsGetType(reg_index);
    if (param_type == PARAM_TYPE_INTEGER &&
            _request_msg.value._tag_ == NATURAL32_TAG &&
            _request_msg.value.natural32.value.count > 0) {
        paramsSetIntegerValue(reg_index,
                              _request_msg.value.natural32.value.elements[0]);
    } else if (param_type == PARAM_TYPE_STRING &&
            _request_msg.value._tag_ == STRING_TAG &&
            _request_msg.value._string.value.count > 0) {
        paramsSetStringValue(reg_index,
                            _request_msg.value._string.value.count,
                            _request_msg.value._string.value.elements);
    }
}

void RegisterAccessRequest::readParam(uavcan_register_Access_Response_1_0& response_msg, ParamIndex_t reg_index) {
    auto param_type = paramsGetType(reg_index);

    if (param_type == PARAM_TYPE_INTEGER) {
        response_msg.value.natural32.value.count = 1;
        response_msg.value._tag_ = NATURAL32_TAG;
        response_msg.value.natural32.value.elements[0] = paramsGetIntegerValue(reg_index);
    } else if (param_type == PARAM_TYPE_STRING) {
        response_msg.value._tag_ = STRING_TAG;
        auto str_param = paramsGetStringValue(reg_index);
        auto str_len = strlenSafely((const uint8_t*)str_param, MAX_STRING_LENGTH);
        memcpy(response_msg.value._string.value.elements, str_param, str_len);
        response_msg.value._string.value.count = str_len;
    }
}

void RegisterAccessRequest::makeResponse(const CanardRxTransfer& transfer, ParamIndex_t reg_index) {

    _transfer_metadata.port_id = port_id;
    _transfer_metadata.remote_node_id = transfer.metadata.remote_node_id;
    _transfer_metadata.transfer_id = transfer.metadata.transfer_id;

    // The write operation is performed first
    writeParam(reg_index);

    // On the next step the register will be read regardless of the outcome of the write operation
    uavcan_register_Access_Response_1_0 response_msg = {};
    readParam(response_msg, reg_index);

    /// @note: It is not enough memory on stack, so use buffer as static
    static uint8_t buf[uavcan_register_Access_Response_1_0_SERIALIZATION_BUFFER_SIZE_BYTES_];
    size_t buf_size = uavcan_register_Access_Response_1_0_SERIALIZATION_BUFFER_SIZE_BYTES_;
    int32_t result = uavcan_register_Access_Response_1_0_serialize_(&response_msg, buf, &buf_size);
    if (NUNAVUT_SUCCESS == result) {
        driver->push(&_transfer_metadata, buf_size, buf);
    }
}
