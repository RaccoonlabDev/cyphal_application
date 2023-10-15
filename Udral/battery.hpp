/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2023 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#ifndef UDRAL_BATTERY_HPP_
#define UDRAL_BATTERY_HPP_

#include "cyphal.hpp"
#include "reg/udral/physics/electricity/SourceTs_0_1.h"

struct ElectricitySourcePublisher: public CyphalPublisher {
    ElectricitySourcePublisher(Cyphal* driver_, CanardPortID port_id) : CyphalPublisher(driver_, port_id) {};
    void publish(float voltage, float current, float full_energy_joule, float energy_joule);
};


struct UdralBatteryPublisher {
    UdralBatteryPublisher(Cyphal* driver_, CanardPortID port_id) : _source_pub(driver_, port_id) {};
    void publish(float voltage, float current, float soc, float full_capacity, float remaining_capacity);
private:
    ElectricitySourcePublisher _source_pub;
};


#endif  // UDRAL_BATTERY_HPP_
