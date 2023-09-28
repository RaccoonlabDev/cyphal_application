/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#ifndef UBUNTU_PUBLISHER_EXAMPLE_APPLICATION_HPP_
#define UBUNTU_PUBLISHER_EXAMPLE_APPLICATION_HPP_

#include "cyphal.hpp"

struct FeedbackPublisher: public CyphalPublisher {
    FeedbackPublisher(Cyphal* driver_) : CyphalPublisher(driver_, 2100) {}
    void process();
};

class Application {
public:
    Application() : feedback(&cyphal) {}
    int init();
    void process();
private:
    Cyphal cyphal;
    FeedbackPublisher feedback;
};

#endif  // UBUNTU_PUBLISHER_EXAMPLE_APPLICATION_HPP_
