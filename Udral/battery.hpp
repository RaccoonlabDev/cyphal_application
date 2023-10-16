/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2023 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#ifndef UDRAL_BATTERY_HPP_
#define UDRAL_BATTERY_HPP_

#include "cyphal.hpp"
#include "reg/udral/physics/electricity/SourceTs_0_1.h"
#include "reg/udral/service/battery/Status_0_2.h"
#include "reg/udral/service/battery/Parameters_0_3.h"

struct ElectricitySourcePublisher: public CyphalPublisher {
    ElectricitySourcePublisher(Cyphal* driver_, CanardPortID port_id) : CyphalPublisher(driver_, port_id) {};
    void publish(float voltage, float current, float full_energy_joule, float energy_joule);
};

struct BatteryStatusPublisher: public CyphalPublisher {
    BatteryStatusPublisher(Cyphal* driver_, CanardPortID port_id) : CyphalPublisher(driver_, port_id) {};
    void publish(float voltage, float temperature_kelvin);
};

struct BatteryParametersPublisher: public CyphalPublisher {
    BatteryParametersPublisher(Cyphal* driver_, CanardPortID port_id) : CyphalPublisher(driver_, port_id) {
        reg_udral_service_battery_Parameters_0_3_initialize_(&_msg);
    }
    void publish();
    void set_nominal_data(float design_capacity_ah, uint64_t unique_id, float nominal_voltage);
    float get_nominal_voltage();
private:
    reg_udral_service_battery_Parameters_0_3 _msg;
};


struct UdralBatteryPublisher {
    UdralBatteryPublisher(Cyphal* driver_, CanardPortID source_port, CanardPortID status_port, CanardPortID params_port) :
        _source_pub(driver_, source_port),
        _status_pub(driver_, status_port),
        _parameters_pub(driver_, params_port) {
    };

    /**
     * @brief Initialize the battery parameters in the beginning of the application
     */
    void set_nominal_data(float design_capacity_ah, uint64_t unique_id, float nominal_voltage);

    /**
     * @brief Call this function at least once per second. It publishes the following topics:
     * SUBJECT                  TYP. RATE [Hz]
     * energy_source            1...100 (calling rate)
     * battery_status           1
     * battery_parameters       0.2
     */
    void publish(float voltage, float current, float temperature_kelvin, float full_capacity_ah, float remaining_capacity_ah);

private:
    ElectricitySourcePublisher _source_pub;
    BatteryStatusPublisher _status_pub;
    BatteryParametersPublisher _parameters_pub;

    uint32_t _next_source_pub_time_ms{1000};
    uint32_t _next_status_pub_time_ms{1000};
    uint32_t _next_parameters_pub_time_ms{1000};
};

#endif  // UDRAL_BATTERY_HPP_
