/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2023 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#ifndef BAROMETER_HPP_
#define BAROMETER_HPP_

#include "cyphal.hpp"
#include "uavcan/si/sample/pressure/Scalar_1_0.h"
#include "uavcan/si/sample/temperature/Scalar_1_0.h"

struct BaroPressurePublisher: public CyphalPublisher {
    BaroPressurePublisher(Cyphal* driver_, CanardPortID port_id) : CyphalPublisher(driver_, port_id) {};
    void publish(const uavcan_si_sample_pressure_Scalar_1_0& msg);
};

struct BaroTemperaturePublisher: public CyphalPublisher {
    BaroTemperaturePublisher(Cyphal* driver_, CanardPortID port_id) : CyphalPublisher(driver_, port_id) {};
    void publish(const uavcan_si_sample_temperature_Scalar_1_0& msg);
};

#endif  // BAROMETER_HPP_
