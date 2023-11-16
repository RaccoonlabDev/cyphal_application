/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022-2023 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#ifndef CYPHAL_CYPHAL_HPP_
#define CYPHAL_CYPHAL_HPP_

#include "cyphal_transport_can.hpp"
#include "cyphal_subscribers.hpp"
#include "cyphal_registers.hpp"
#include "cyphal_publishers.hpp"
#include "canard.h"
#include "o1heap.h"
#include "uavcan/node/GetInfo_1_0.h"
#include "uavcan/node/Heartbeat_1_0.h"

#ifndef CYPHAL_HEAP_SIZE
    #define CYPHAL_HEAP_SIZE (1024*10)
#endif

inline constexpr size_t HEAP_SIZE                       = CYPHAL_HEAP_SIZE;

inline constexpr int CYPHAL_TRANSPORT_INIT_ERROR        = 2;
inline constexpr int CYPHAL_HEAP_INIT_ERROR             = 3;
inline constexpr int CYPHAL_SUB_APPLICATION_INIT_ERROR  = 4;

class Cyphal {
public:
    Cyphal(): heartbeat_pub(this),
              port_list_pub(this),
              node_get_info_response(this),
              register_list_response(this),
              register_access_response(this),
              execute_cmd_response(this) {};
    int init();
    void process();
    int32_t push(CanardTransferMetadata* metadata, size_t payload_size, const uint8_t* payload);
    int8_t subscribe(CyphalSubscriber* sub_info, size_t size, CanardTransferKind kind);

    void setNodeHealth(uavcan_node_Health_1_0 health);
    void setNodeMode(uavcan_node_Mode_1_0 mode);
    void spinTransmit();

    uint16_t getNodeId() const {return node_id;}

    uint32_t ports_updated{false};

    static constexpr size_t MAX_SUB_NUM = 10;
    static O1HeapInstance* my_allocator;
private:
    friend PortListPublisher;
    void spinReceivedFrame(const CanardMicrosecond rx_timestamp_usec,
                           const CanardFrame* const received_frame);
    void processReceivedTransfer(const uint8_t redundant_interface_index,
                                 const CanardRxTransfer& transfer) const;
    bool isTxQueueItemFresh(const CanardTxQueueItem* ti) const;

    ///< application
    int8_t subscribeApplication();

    CyphalTransportCan transport;
    CanardInstance canard_instance;
    CanardTxQueue queue;
    uint8_t base[HEAP_SIZE] __attribute__ ((aligned (O1HEAP_ALIGNMENT)));
    uint32_t error_counter = 0;
    uint32_t next_pub_time_ms = 10;
    uint8_t node_id;

    HeartbeatPublisher heartbeat_pub;
    PortListPublisher port_list_pub;
    NodeGetInfoSubscriber node_get_info_response;
    RegisterListRequest register_list_response;
    RegisterAccessRequest register_access_response;
    ExecuteCommandSubscriber execute_cmd_response;

    std::array<CyphalSubscriber*, MAX_SUB_NUM> _sub_info;
    size_t _sub_num{0};


};

#endif  // CYPHAL_CYPHAL_HPP_
