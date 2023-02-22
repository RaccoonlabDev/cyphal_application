/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

/**
 * @file cyphal_publishers.hpp
 * @author d.ponomarev
 * @date Aug 22, 2022
 */

#ifndef CYPHAL_CYPHAL_PUBLISHERS_HPP_
#define CYPHAL_CYPHAL_PUBLISHERS_HPP_

#include <array>
#include "canard.h"
#include "uavcan/node/Heartbeat_1_0.h"
#include "uavcan/node/Health_1_0.h"
#include "uavcan/node/port/List_0_1.h"
#include "uavcan/node/port/List_1_0.h"

class Cyphal;

class CyphalPublisher {
public:
    CyphalPublisher(Cyphal* driver_, CanardPortID port_id_) : driver(driver_) {
        setPortId(port_id_);
        if (publishers_amount < MAX_PUB_NUM) {
            publishers[publishers_amount] = this;
            publishers_amount++;
        }
    }
    void setPortId(CanardPortID port_id);
    CanardPortID getPortId() const;
    bool isEnabled() const;
    int32_t push(size_t payload_size, const uint8_t* payload);

    static constexpr const size_t MAX_PUB_NUM = 15;
protected:
    Cyphal* driver;
    static std::array<CyphalPublisher*, MAX_PUB_NUM> publishers;
    static uint8_t publishers_amount;

private:
    CanardTransferMetadata transfer_metadata = {
        .priority       = CanardPriorityNominal,
        .transfer_kind  = CanardTransferKindMessage,
        .port_id        = 65535,
        .remote_node_id = CANARD_NODE_ID_UNSET,
        .transfer_id    = 0,
    };
};

struct HeartbeatPublisher: public CyphalPublisher {
    explicit HeartbeatPublisher(Cyphal* driver_) :
        CyphalPublisher(driver_, uavcan_node_Heartbeat_1_0_FIXED_PORT_ID_) {};
    void publish(const uavcan_node_Heartbeat_1_0& msg);
};

struct PortListPublisher: public CyphalPublisher {
    explicit PortListPublisher(Cyphal* driver_) :
        CyphalPublisher(driver_, uavcan_node_port_List_1_0_FIXED_PORT_ID_) {};
    void publish();
    size_t uavcan_node_port_List_1_0_create();
private:
    static constexpr const size_t PORT_LIST_BUFFER_SIZE = 148 + (MAX_PUB_NUM + 10)*2;
    uint8_t _port_list_buffer[PORT_LIST_BUFFER_SIZE];
    uint32_t next_pub_time_ms = 1000;
};

#endif  // CYPHAL_CYPHAL_PUBLISHERS_HPP_
