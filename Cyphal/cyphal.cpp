/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022-2023 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#include "cyphal.hpp"
#include "main.h"
#include "cyphal_subscribers.hpp"
#include "cyphal_registers.hpp"
#include "uavcan/node/Heartbeat_1_0.h"
#include "uavcan/node/Health_1_0.h"
#include "storage.h"
#include "params.hpp"

namespace cyphal {

static const constexpr size_t TX_QUEUE_FRAME_SIZE = 320;  ///< we need 314 bytes for port.List

///< wrappers
static void* memAllocate(CanardInstance* const canard, const size_t amount);
static void memFree(CanardInstance* const canard, void* const pointer);
static uint32_t getCurrentMicroseconds();


O1HeapInstance* Cyphal::my_allocator;

int Cyphal::init() {
    heartbeat_pub.msg.health.value = uavcan_node_Health_1_0_NOMINAL;
    heartbeat_pub.msg.mode.value = uavcan_node_Mode_1_0_OPERATIONAL;
    heartbeat_pub.msg.vendor_specific_status_code = 0;

    node_id = static_cast<uint8_t>(paramsGetIntegerValue(IntParamsIndexes::PARAM_NODE_ID));
    if (node_id == 0 || node_id > 126) {
        node_id = 42;
    }

    if (!transport.init(1000000, 0)) {
        return -CYPHAL_TRANSPORT_INIT_ERROR;
    }

    my_allocator = o1heapInit(base, HEAP_SIZE);
    if (nullptr == my_allocator) {
        return -CYPHAL_HEAP_INIT_ERROR;
    }

    canard_instance = canardInit(&memAllocate, &memFree);
    canard_instance.node_id = node_id;
    queue = canardTxInit(TX_QUEUE_FRAME_SIZE, CANARD_MTU_CAN_CLASSIC);

    node_get_info_response.init();

    if (subscribeApplication() < 0) {
        return -CYPHAL_SUB_APPLICATION_INIT_ERROR;
    }

    return 0;
}

void Cyphal::process() {
    // 1. spin recv
    uint32_t crnt_time_ms = HAL_GetTick();
    for (uint_fast8_t frame_idx = 0; frame_idx < transport.get_rx_queue_size(); frame_idx++) {
        CanardFrame rx_frame;
        if (transport.receive(&rx_frame)) {
            spinReceivedFrame(crnt_time_ms * 1000, &rx_frame);
        } else {
            break;
        }
    }

    // 2. spin application
    if (next_pub_time_ms < crnt_time_ms) {
        next_pub_time_ms += 1000;
        heartbeat_pub.msg.uptime = crnt_time_ms / 1000;
        heartbeat_pub.publish();
    } else {
        port_list_pub.publish();
    }

    // 3. spin tx
    spinTransmit();
}

int32_t Cyphal::push(CanardTransferMetadata* metadata, size_t payload_size, const uint8_t* payload) {
    if (metadata->port_id == 0) {
        return 0;
    }
    auto res = canardTxPush(&queue, &canard_instance, 0, metadata, payload_size, payload);
    metadata->transfer_id++;
    return res;
}


int8_t Cyphal::subscribe(CyphalSubscriber* sub_info, size_t size, CanardTransferKind kind) {
    if (_sub_num >= MAX_SUB_NUM || sub_info->port_id == 0) {
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

void Cyphal::setNodeHealth(uavcan_node_Health_1_0 health) {
    heartbeat_pub.msg.health = health;
}

void Cyphal::setNodeMode(uavcan_node_Mode_1_0 mode) {
    heartbeat_pub.msg.mode = mode;
}

void Cyphal::setVSSC(uint8_t vssc) {
    heartbeat_pub.msg.vendor_specific_status_code = vssc;
}

void Cyphal::processReceivedTransfer([[maybe_unused]] const uint8_t redundant_interface_index,
                                     const CanardRxTransfer& transfer) const {
    for (size_t sub_idx = 0; sub_idx < _sub_num; sub_idx++) {
        if (transfer.metadata.port_id == _sub_info[sub_idx]->port_id) {
            _sub_info[sub_idx]->callback(transfer);
        }
    }
}

bool Cyphal::isTxQueueItemFresh(const CanardTxQueueItem* ti) const {
    return (0U == ti->tx_deadline_usec) || (ti->tx_deadline_usec > getCurrentMicroseconds());
}

void Cyphal::spinTransmit() {
    for (const CanardTxQueueItem* ti = nullptr; (ti = canardTxPeek(&queue)) != nullptr;) {
        if (isTxQueueItemFresh(ti) && !transport.transmit(ti)) {
            break;
        }
        canard_instance.memory_free(&canard_instance, canardTxPop(&queue, ti));
    }
}

int8_t Cyphal::subscribeApplication() {
    if (subscribe(&node_get_info_response,
                  uavcan_node_GetInfo_Request_1_0_EXTENT_BYTES_,
                  CanardTransferKindRequest) < 0) {
        return -1;
    }

    if (subscribe(&register_list_response,
                  uavcan_register_List_Request_1_0_EXTENT_BYTES_,
                  CanardTransferKindRequest) < 0) {
        return -1;
    }

    if (subscribe(&register_access_response,
                  uavcan_register_Access_Request_1_0_EXTENT_BYTES_,
                  CanardTransferKindRequest) < 0) {
        return -1;
    }

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
                                         nullptr);
    if (result < 0) {
        error_counter++;
    } else if (result == 1) {
        processReceivedTransfer(0, transfer);
        canard_instance.memory_free(&canard_instance, transfer.payload);
    }
}


static void* memAllocate(CanardInstance* const canard, const size_t amount) {
    (void) canard;
    return o1heapAllocate(Cyphal::my_allocator, amount);
}
static void memFree(CanardInstance* const canard, void* const pointer) {
    (void) canard;
    o1heapFree(Cyphal::my_allocator, pointer);
}
static uint32_t getCurrentMicroseconds() {
    return HAL_GetTick();
}

}  // namespace cyphal
