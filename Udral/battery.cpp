/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2023 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#include "battery.hpp"
#include "main.h"
#include "params.hpp"

#define WH_TO_JOULE   3600

void UdralBatteryPublisher::publish(float voltage, float current, float soc, float full_capacity, float remaining_capacity) {
    (void)soc;
    // here we expect 4s lipo
    float full_energy_joule = 3600 * full_capacity * 14.8;
    float energy_joule = 3600 * remaining_capacity * voltage;
    _source_pub.publish(voltage, current, full_energy_joule, energy_joule);
}

void ElectricitySourcePublisher::publish(float voltage, float current, float full_energy_joule, float energy_joule) {
    if (!isEnabled()) {
        return;
    }

    reg_udral_physics_electricity_SourceTs_0_1 msg;
    msg.value.power.voltage.volt = voltage;
    msg.value.power.current.ampere = current;
    msg.value.energy.joule = energy_joule;
    msg.value.full_energy.joule = full_energy_joule;

    static uint8_t buffer[reg_udral_physics_electricity_SourceTs_0_1_SERIALIZATION_BUFFER_SIZE_BYTES_];
    size_t buffer_size = reg_udral_physics_electricity_SourceTs_0_1_SERIALIZATION_BUFFER_SIZE_BYTES_;
    int32_t result = reg_udral_physics_electricity_SourceTs_0_1_serialize_(&msg, buffer, &buffer_size);
    if (NUNAVUT_SUCCESS == result) {
        push(buffer_size, buffer);
    }
}
