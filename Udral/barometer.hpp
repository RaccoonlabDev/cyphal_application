/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2023 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#ifndef UDRAL_BAROMETER_HPP_
#define UDRAL_BAROMETER_HPP_

#include "cyphal_publishers.hpp"
#include "uavcan/si/sample/pressure/Scalar_1_0.h"
#include "uavcan/si/sample/temperature/Scalar_1_0.h"

namespace cyphal {

struct BaroPressurePublisher: public CyphalPublisher {
    BaroPressurePublisher();
    void publish(const uavcan_si_sample_pressure_Scalar_1_0& msg);
};

struct BaroTemperaturePublisher: public CyphalPublisher {
    BaroTemperaturePublisher();
    void publish(const uavcan_si_sample_temperature_Scalar_1_0& msg);
};

}  // namespace cyphal

#endif  // UDRAL_BAROMETER_HPP_
