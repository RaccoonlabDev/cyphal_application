/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2023 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#ifndef UDRAL_RANGEFINDER_HPP_
#define UDRAL_RANGEFINDER_HPP_

#include "cyphal.hpp"
#include "uavcan/si/sample/length/Scalar_1_0.h"

namespace cyphal {

struct RangefinderRangePublisher: public CyphalPublisher {
    RangefinderRangePublisher(Cyphal* driver_, uint16_t port_id) : CyphalPublisher(driver_, port_id) {};
    void publish(float range);
};

}  // namespace cyphal

#endif  // UDRAL_RANGEFINDER_HPP_
