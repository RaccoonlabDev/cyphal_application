/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2023 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#ifndef UDRAL_RANGEFINDER_HPP_
#define UDRAL_RANGEFINDER_HPP_

#include "cyphal.hpp"
#include "uavcan/si/sample/length/Scalar_1_0.h"


struct RangefinderRangePublisher: public CyphalPublisher {
    RangefinderRangePublisher(Cyphal* driver_, CanardPortID port_id) : CyphalPublisher(driver_, port_id) {};
    void publish(float range);
};


#endif  // UDRAL_RANGEFINDER_HPP_
