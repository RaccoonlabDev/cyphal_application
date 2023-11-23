/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2023 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#ifndef RL_CIRCUIT_STATUS_HPP_
#define RL_CIRCUIT_STATUS_HPP_

#include "cyphal.hpp"
#include "uavcan/si/sample/voltage/Scalar_1_0.h"
#include "uavcan/si/sample/temperature/Scalar_1_0.h"

namespace RaccoonLab {

struct CircuitStatusVoltagePublisher: public cyphal::CyphalPublisher {
    CircuitStatusVoltagePublisher(cyphal::Cyphal* driver_, uint16_t port_id) : CyphalPublisher(driver_, port_id) {};
    void publish(const uavcan_si_sample_voltage_Scalar_1_0& msg);
};

struct CircuitStatusTemperaturePublisher: public cyphal::CyphalPublisher {
    CircuitStatusTemperaturePublisher(cyphal::Cyphal* driver_, uint16_t port_id) : CyphalPublisher(driver_, port_id) {};
    void publish(const uavcan_si_sample_temperature_Scalar_1_0& msg);
};

}  // namespace RaccoonLab

#endif  // RL_CIRCUIT_STATUS_HPP_
