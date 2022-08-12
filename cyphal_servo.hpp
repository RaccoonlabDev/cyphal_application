/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

/**
 * @file cyphal.h
 * @author d.ponomarev
 * @date Dec 28, 2021
 */

#ifndef LIBCYPHAL_CYPHAL_SERVO_HPP_
#define LIBCYPHAL_CYPHAL_SERVO_HPP_

#include "cyphal_subscribers.hpp"
#include "reg/udral/service/actuator/common/sp/Scalar_0_1.h"
#include "reg/udral/service/actuator/common/sp/Vector4_0_1.h"

struct SetpointSubscriber: public CyphalSubscriber {
    SetpointSubscriber(Cyphal* driver_, CanardPortID port_id_) : CyphalSubscriber(driver_, port_id_) {};
    void callback(const CanardRxTransfer& transfer) override;
};


#endif  // LIBCYPHAL_CYPHAL_SERVO_HPP_
