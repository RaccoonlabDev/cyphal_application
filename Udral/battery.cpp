/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2023 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#include "battery.hpp"
#include "main.h"
#include "params.hpp"

#define WH_TO_JOULE     3600
#define AH_TO_COULOMB   3600

void UdralBatteryPublisher::set_nominal_data(float design_capacity_ah, uint64_t unique_id, float nominal_voltage) {
    _parameters_pub.set_nominal_data(design_capacity_ah, unique_id, nominal_voltage);
}

void UdralBatteryPublisher::publish(float voltage, float current, float temperature_kelvin, float full_capacity_ah, float remaining_capacity_ah) {
    uint32_t crnt_time_ms = HAL_GetTick();

    if (crnt_time_ms > _next_source_pub_time_ms) {
        _next_source_pub_time_ms = crnt_time_ms + 10;

        float full_energy_joule = WH_TO_JOULE * full_capacity_ah * _parameters_pub.get_nominal_voltage();
        float energy_joule = WH_TO_JOULE * remaining_capacity_ah * voltage;
        _source_pub.publish(voltage, current, full_energy_joule, energy_joule);
    }

    if (crnt_time_ms > _next_status_pub_time_ms) {
        _next_status_pub_time_ms = crnt_time_ms + 1000;

        _status_pub.publish(voltage, temperature_kelvin);
    }

    if (crnt_time_ms > _next_parameters_pub_time_ms) {
        _next_parameters_pub_time_ms = crnt_time_ms + 5000;

        _parameters_pub.publish();
    }
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

void BatteryStatusPublisher::publish(float voltage, float temperature_kelvin) {
    if (!isEnabled()) {
        return;
    }

    reg_udral_service_battery_Status_0_2 msg;
    msg.temperature_min_max[0].kelvin = temperature_kelvin;
    msg.temperature_min_max[1].kelvin = temperature_kelvin;
    msg.cell_voltages.count = 1;
    msg.cell_voltages.elements[0] = voltage;

    static uint8_t buffer[reg_udral_service_battery_Status_0_2_SERIALIZATION_BUFFER_SIZE_BYTES_];
    size_t buffer_size = reg_udral_service_battery_Status_0_2_SERIALIZATION_BUFFER_SIZE_BYTES_;
    int32_t result = reg_udral_service_battery_Status_0_2_serialize_(&msg, buffer, &buffer_size);
    if (NUNAVUT_SUCCESS == result) {
        push(buffer_size, buffer);
    }
}

void BatteryParametersPublisher::set_nominal_data(float design_capacity_ah, uint64_t unique_id, float nominal_voltage) {
    _msg.design_capacity.coulomb = design_capacity_ah * AH_TO_COULOMB;
    _msg.unique_id = unique_id;
    _msg.nominal_voltage.volt = nominal_voltage;
    _msg.cycle_count = 0;
    _msg.state_of_health_pct = 100;  // ranges from 0 (unusable) to 100 (new)
}

void BatteryParametersPublisher::publish() {
    if (!isEnabled()) {
        return;
    }

    static uint8_t buffer[reg_udral_service_battery_Parameters_0_3_SERIALIZATION_BUFFER_SIZE_BYTES_];
    size_t buffer_size = reg_udral_service_battery_Parameters_0_3_SERIALIZATION_BUFFER_SIZE_BYTES_;
    int32_t result = reg_udral_service_battery_Parameters_0_3_serialize_(&_msg, buffer, &buffer_size);
    if (NUNAVUT_SUCCESS == result) {
        push(buffer_size, buffer);
    }
}

float BatteryParametersPublisher::get_nominal_voltage() {
    return _msg.nominal_voltage.volt;
}
