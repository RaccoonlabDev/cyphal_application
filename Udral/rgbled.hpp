/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2023 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#ifndef UDRAL_RGBLED_HPP_
#define UDRAL_RGBLED_HPP_

#include "cyphal.hpp"
#include "reg/udral/physics/optics/HighColor_0_1.h"


struct HighColorPublisher: public CyphalPublisher {
    HighColorPublisher(Cyphal* driver_, CanardPortID port_id) : CyphalPublisher(driver_, port_id) {};
    void publish(uint8_t red, uint8_t green, uint8_t blue);
};

struct HighColorSubscriber: public CyphalSubscriber {
    HighColorSubscriber(Cyphal* driver, CanardPortID port_id = 0) : CyphalSubscriber(driver, port_id) {}
    int8_t init();
    void callback(const CanardRxTransfer& transfer) override;
    const reg_udral_physics_optics_HighColor_0_1& get() const {return _msg;};
private:
    reg_udral_physics_optics_HighColor_0_1 _msg = {};
};

#endif  // UDRAL_RGBLED_HPP_
