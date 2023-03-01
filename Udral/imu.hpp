/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2023 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#ifndef UDRAL_IMU_HPP_
#define UDRAL_IMU_HPP_

#include "cyphal.hpp"
#include "uavcan/si/sample/angular_velocity/Vector3_1_0.h"
#include "uavcan/si/sample/acceleration/Vector3_1_0.h"

struct ImuAccelPublisher: public CyphalPublisher {
    ImuAccelPublisher(Cyphal* driver_, CanardPortID port_id) : CyphalPublisher(driver_, port_id) {};
    void publish(const uavcan_si_sample_acceleration_Vector3_1_0& msg);
};

struct ImuGyroPublisher: public CyphalPublisher {
    ImuGyroPublisher(Cyphal* driver_, CanardPortID port_id) : CyphalPublisher(driver_, port_id) {};
    void publish(const uavcan_si_sample_angular_velocity_Vector3_1_0& msg);
};

#endif  // UDRAL_IMU_HPP_
