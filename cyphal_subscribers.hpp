/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

/**
 * @file cyphal.h
 * @author d.ponomarev
 * @date Dec 28, 2021
 */

#ifndef LIBCYPHAL_CYPHAL_SUBSCRIBERS_HPP_
#define LIBCYPHAL_CYPHAL_SUBSCRIBERS_HPP_

#include "canard.h"
#include "uavcan/node/GetInfo_1_0.h"
#include "uavcan/node/ExecuteCommand_1_0.h"

class Cyphal;

class CyphalSubscriber {
public:
    CyphalSubscriber(Cyphal* driver_, CanardPortID port_id_) : driver(driver_), port_id(port_id_) {}
    virtual void callback(const CanardRxTransfer& transfer) = 0;
    CanardRxSubscription subscription;
    Cyphal* driver;
    CanardPortID port_id;
};

struct NodeGetInfoSubscriber: public CyphalSubscriber {
    NodeGetInfoSubscriber(Cyphal* driver_, CanardPortID port_id_) : CyphalSubscriber(driver_, port_id_) {};
    void callback(const CanardRxTransfer& transfer) override;
private:
    uint8_t _transfer_id = 0;
};

struct ExecuteCommandSubscriber: public CyphalSubscriber {
    ExecuteCommandSubscriber(Cyphal* driver_, CanardPortID port_id_) : CyphalSubscriber(driver_, port_id_) {};
    void callback(const CanardRxTransfer& transfer) override;
private:
    uint8_t _transfer_id = 0;
};

#endif  // LIBCYPHAL_CYPHAL_SUBSCRIBERS_HPP_
