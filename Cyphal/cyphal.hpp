/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

/**
 * @file cyphal.h
 * @author d.ponomarev
 * @date Dec 28, 2021
 */

#ifndef CYPHAL_CYPHAL_HPP_
#define CYPHAL_CYPHAL_HPP_

#include "cyphal_transport_can.hpp"
#include "cyphal_subscribers.hpp"
#include "cyphal_publishers.hpp"
#include "canard.h"
#include "o1heap.h"
#include "uavcan/node/GetInfo_1_0.h"

#define HEAP_SIZE           (1024*3)

#define CYPHAL_TRANSPORT_INIT_ERROR         2
#define CYPHAL_HEAP_INIT_ERROR              3
#define CYPHAL_SUB_APPLICATION_INIT_ERROR   4

class Cyphal {
public:
    Cyphal(): heartbeat_pub(this), port_list_pub(this) {};
    int init();
    void process();
    int32_t push(CanardTransferMetadata *metadata, size_t payload_size, const void *payload);
    int8_t subscribe(CyphalSubscriber* sub_info, size_t size, CanardTransferKind kind);

    static constexpr size_t MAX_SUB_NUM = 10;
    CyphalSubscriber* _sub_info[MAX_SUB_NUM];
    size_t _sub_num{0};
private:
    void spinReceivedFrame(const CanardMicrosecond rx_timestamp_usec,
                           const CanardFrame* const received_frame);
    void spinTransmit();
    void processReceivedTransfer(const uint8_t redundant_interface_index,
                                 const CanardRxTransfer& transfer);

    ///< application
    int8_t subscribeApplication();

    CyphalTransportCan transport;
    CanardInstance canard_instance;
    CanardTxQueue queue;
    uint8_t base[HEAP_SIZE] __attribute__ ((aligned (O1HEAP_ALIGNMENT)));
    uint8_t my_message_transfer_id;
    uavcan_node_GetInfo_Response_1_0 node_status;
    uint32_t error_counter = 0;
    uint8_t node_id;

    HeartbeatPublisher heartbeat_pub;
    PortListPublisher port_list_pub;
};

#endif  // CYPHAL_CYPHAL_HPP_
