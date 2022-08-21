/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

/**
 * @file cyphal_publishers.hpp
 * @author d.ponomarev
 * @date Aug 22, 2022
 */

#ifndef LIBCYPHAL_CYPHAL_PUBLISHERS_HPP_
#define LIBCYPHAL_CYPHAL_PUBLISHERS_HPP_

#include "canard.h"
#include "uavcan/node/Heartbeat_1_0.h"
#include "uavcan/node/Health_1_0.h"

class Cyphal;

class CyphalPublisher {
public:
    CyphalPublisher(Cyphal* driver_, CanardPortID port_id_) : driver(driver_) {
        transfer_metadata.port_id = port_id_;
    }
protected:
    Cyphal* driver;
    CanardTransferMetadata transfer_metadata = {
        .priority       = CanardPriorityNominal,
        .transfer_kind  = CanardTransferKindMessage,
        .port_id        = 65535,
        .remote_node_id = CANARD_NODE_ID_UNSET,
        .transfer_id    = 0,
    };
};

struct HeartbeatPublisher: public CyphalPublisher {
    HeartbeatPublisher(Cyphal* driver_) : CyphalPublisher(driver_, uavcan_node_Heartbeat_1_0_FIXED_PORT_ID_) {};
    void publish(const uavcan_node_Heartbeat_1_0& msg);
};


#endif  // LIBCYPHAL_CYPHAL_PUBLISHERS_HPP_
