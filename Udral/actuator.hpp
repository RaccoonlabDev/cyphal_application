/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2023 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#ifndef UDRAL_ACTUATOR_HPP_
#define UDRAL_ACTUATOR_HPP_

#include "cyphal.hpp"
#include "reg/udral/service/actuator/common/sp/Scalar_0_1.h"
#include "reg/udral/service/actuator/common/sp/Vector4_0_1.h"
#include "reg/udral/service/common/Readiness_0_1.h"


struct SetpointSubscriber: public CyphalSubscriber {
    SetpointSubscriber(Cyphal* driver);
    int8_t init();
    void callback(const CanardRxTransfer& transfer) override;
    const reg_udral_service_actuator_common_sp_Vector4_0_1& get_setpoint() const;
    uint32_t get_recv_counter() const;
    void clear_recv_counter();
private:
    reg_udral_service_actuator_common_sp_Vector4_0_1 msg = {};
    uint32_t _recv_counter{0};
};


struct ReadinessSubscriber: public CyphalSubscriber {
    ReadinessSubscriber(Cyphal* driver_);
    int8_t init();
    uint8_t get_readiness();
private:
    void callback(const CanardRxTransfer& transfer) override;
    reg_udral_service_common_Readiness_0_1 msg = {};
    uint32_t _last_recv_time_ms{0};
};


// https://telega.zubax.com/interfaces/cyphal.html#compact
#pragma pack(push, 1)
struct ZubaxCompactFeedback {
    uint32_t dc_voltage             : 11;
    int32_t dc_current              : 12;
    int32_t phase_current_amplitude : 12;
    int32_t velocity                : 13;
    int8_t demand_factor_pct        : 8;
};
#pragma pack(pop)

struct ZubaxCompactFeedbackPublisher: public CyphalPublisher {
    ZubaxCompactFeedbackPublisher(Cyphal* driver_, CanardPortID port_id) : CyphalPublisher(driver_, port_id) {};
    void publish(const ZubaxCompactFeedback& msg);
    static constexpr float RPM_TO_RAD_PER_SEC = 0.1047198;
private:
    static constexpr size_t zubax_telega_CompactFeedback_0_1_SERIALIZATION_BUFFER_SIZE_BYTES = 7;
};

#endif  // UDRAL_ACTUATOR_HPP_
