/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

/**
 * @file cyphal.cpp
 * @author d.ponomarev
 * @date Dec 26, 2021
 */

#include "cyphal.hpp"
#include "main.h"
#include "cyphal_subscribers.hpp"
#include "cyphal_registers.hpp"
#include "servo.hpp"
#include "uavcan/node/Heartbeat_1_0.h"
#include "uavcan/node/Health_1_0.h"
extern "C" {
#include "storage.h"
}

#define TX_QUEUE_FRAME_SIZE 200


///< wrappers
static void* memAllocate(CanardInstance* const canard, const size_t amount);
static void memFree(CanardInstance* const canard, void* const pointer);
static uint32_t getCurrentMicroseconds();


O1HeapInstance* my_allocator;

int Cyphal::init() {
    if (!transport.init()) {
        return -1;
    }

    my_allocator = o1heapInit(base, HEAP_SIZE);
    if (NULL == my_allocator) {
        return -1;
    }

    canard_instance = canardInit(&memAllocate, &memFree);
    canard_instance.node_id = node_id;
    queue = canardTxInit(TX_QUEUE_FRAME_SIZE, CANARD_MTU_CAN_CLASSIC);

    if (subscribeApplication() < 0) {
        return -1;
    }

    return 0;
}

void Cyphal::process() {
    // 1. spin recv
    CanardFrame rx_frame;
    if (transport.receive(&rx_frame)) {
        spinReceivedFrame(HAL_GetTick(), &rx_frame);
    }

    // 2. spin application
    static uint32_t next_pub_time_ms = 0;
    if (next_pub_time_ms < HAL_GetTick()) {
        HAL_GPIO_TogglePin(INTERNAL_LED_1_GPIO_Port, INTERNAL_LED_1_Pin);
        next_pub_time_ms += 500;

        publishHeartbeat();
    }

    // 3. spin tx
    spinTransmit();
}

int32_t Cyphal::push(const CanardTransferMetadata *metadata, size_t payload_size, const void *payload) {
    return canardTxPush(&queue, &canard_instance, 0, metadata, payload_size, payload);
}


int8_t Cyphal::subscribe(CyphalSubscriber* sub_info, size_t size, CanardTransferKind kind) {
    if (_sub_num >= MAX_SUB_NUM) {
        return -1;
    }

    _sub_info[_sub_num] = sub_info;

    int8_t res = canardRxSubscribe(&canard_instance,
                                   kind,
                                   _sub_info[_sub_num]->port_id,
                                   size,
                                   CANARD_DEFAULT_TRANSFER_ID_TIMEOUT_USEC,
                                   &_sub_info[_sub_num]->subscription);

    _sub_num++;

    return res;
}


void Cyphal::processReceivedTransfer(const uint8_t redundant_interface_index,
                                     const CanardRxTransfer& transfer) {
    const CanardPortID PORT_ID = transfer.metadata.port_id;
    if (PORT_ID == uavcan_node_Heartbeat_1_0_FIXED_PORT_ID_) {
        asm("NOP");
    }

    for (size_t sub_idx = 0; sub_idx < _sub_num; sub_idx++) {
        if (PORT_ID == _sub_info[sub_idx]->port_id) {
            _sub_info[sub_idx]->callback(transfer);
        }
    }
}

void Cyphal::publishHeartbeat() {
    static uint8_t heartbeat_transfer_id = 0;
    uavcan_node_Health_1_0 health = {
        .value = uavcan_node_Health_1_0_WARNING
    };
    uavcan_node_Mode_1_0 mode = {
        .value = uavcan_node_Mode_1_0_INITIALIZATION
    };
    const uavcan_node_Heartbeat_1_0 msg = {
        .uptime = HAL_GetTick() / 1000,
        .health = health,
        .mode = mode,
        .vendor_specific_status_code = static_cast<uint8_t>(0)
    };
    const CanardTransferMetadata transfer_metadata = {
        .priority       = CanardPriorityNominal,
        .transfer_kind  = CanardTransferKindMessage,
        .port_id        = uavcan_node_Heartbeat_1_0_FIXED_PORT_ID_,
        .remote_node_id = CANARD_NODE_ID_UNSET,
        .transfer_id    = heartbeat_transfer_id,
    };
    heartbeat_transfer_id++;

    static uint8_t heartbeat_buffer[64];
    size_t heartbeat_buffer_size = 64;
    int32_t result;
    result = uavcan_node_Heartbeat_1_0_serialize_(&msg, heartbeat_buffer, &heartbeat_buffer_size);
    if (NUNAVUT_SUCCESS == result) {
        result = canardTxPush(&queue,
                              &canard_instance,
                              0,
                              &transfer_metadata,
                              heartbeat_buffer_size,
                              heartbeat_buffer);
        if (result < 0) {
            error_counter++;
        }
    }
}

void Cyphal::spinTransmit() {
    for (const CanardTxQueueItem* ti = NULL; (ti = canardTxPeek(&queue)) != NULL;) {
        if ((0U == ti->tx_deadline_usec) || (ti->tx_deadline_usec > getCurrentMicroseconds())) {
            if (!transport.transmit(ti)) {
                break;
            }
        }
        canard_instance.memory_free(&canard_instance, canardTxPop(&queue, ti));
    }
}

int8_t Cyphal::subscribeApplication() {
    // uavcan.node.GetInfo.Response
    static NodeGetInfoSubscriber node_get_info_response(this, uavcan_node_GetInfo_1_0_FIXED_PORT_ID_);
    if (subscribe(&node_get_info_response,
                  uavcan_node_GetInfo_Request_1_0_EXTENT_BYTES_,
                  CanardTransferKindRequest) < 0) {
        return -1;
    }

    // uavcan.register.List
    static RegisterListRequest register_list_response(this, uavcan_register_List_1_0_FIXED_PORT_ID_);
    if (subscribe(&register_list_response,
                  uavcan_register_List_Request_1_0_EXTENT_BYTES_,
                  CanardTransferKindRequest) < 0) {
        return -1;
    }

    // uavcan.register.Access
    static RegisterAccessRequest register_access_response(this, uavcan_register_Access_1_0_FIXED_PORT_ID_);
    if (subscribe(&register_access_response,
                  uavcan_register_Access_Request_1_0_EXTENT_BYTES_,
                  CanardTransferKindRequest) < 0) {
        return -1;
    }

    static ExecuteCommandSubscriber execute_cmd_response(this, uavcan_node_ExecuteCommand_1_0_FIXED_PORT_ID_);
    if (subscribe(&execute_cmd_response,
                  uavcan_node_ExecuteCommand_Request_1_0_EXTENT_BYTES_,
                  CanardTransferKindRequest) < 0) {
        return -1;
    }

    return 0;
}


// rx_timestamp_usec - When the frame was received, in microseconds.
// received_frame - The CAN frame received from the bus.
void Cyphal::spinReceivedFrame(const CanardMicrosecond rx_timestamp_usec,
                               const CanardFrame* const received_frame) {
    CanardRxTransfer transfer;
    const int8_t result = canardRxAccept(&canard_instance,
                                         rx_timestamp_usec,
                                         received_frame,
                                         0,
                                         &transfer,
                                         NULL);
    if (result < 0) {
        error_counter++;
    } else if (result == 1) {
        processReceivedTransfer(0, transfer);
        canard_instance.memory_free(&canard_instance, transfer.payload);
    }
}


void* memAllocate(CanardInstance* const canard, const size_t amount) {
    (void) canard;
    return o1heapAllocate(my_allocator, amount);
}
void memFree(CanardInstance* const canard, void* const pointer) {
    (void) canard;
    o1heapFree(my_allocator, pointer);
}
uint32_t getCurrentMicroseconds() {
    return HAL_GetTick();
}
