/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2023 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#ifndef GNSS_HPP_
#define GNSS_HPP_

#include "cyphal.hpp"

struct GpsPointPublisher: public CyphalPublisher {
    GpsPointPublisher(Cyphal* driver_, uint16_t port_id) : CyphalPublisher(driver_, port_id) {}
    void publish();
};

struct Int16Publisher: public CyphalPublisher {
    Int16Publisher(Cyphal* driver_, uint16_t port_id) : CyphalPublisher(driver_, port_id) {}
    void publish(uint16_t value);
};

#endif  // GNSS_HPP_
