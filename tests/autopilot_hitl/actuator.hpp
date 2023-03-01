/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2023 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#ifndef ACTUATOR_HPP_
#define ACTUATOR_HPP_

#include "cyphal.hpp"
#include "reg/udral/service/actuator/common/sp/Scalar_0_1.h"
#include "reg/udral/service/actuator/common/sp/Vector4_0_1.h"
#include "reg/udral/service/common/Readiness_0_1.h"

class SetpointSubscriber: public CyphalSubscriber {
public:
    SetpointSubscriber(Cyphal* driver);
    int8_t init();
    void callback(const CanardRxTransfer& transfer) override;
    reg_udral_service_actuator_common_sp_Vector4_0_1 msg = {};
};

class ReadinessSubscriber: public CyphalSubscriber {
public:
    ReadinessSubscriber(Cyphal* driver_);
    int8_t init();
    void callback(const CanardRxTransfer& transfer) override;
    reg_udral_service_common_Readiness_0_1 msg = {};
};

#endif  // ACTUATOR_HPP_
