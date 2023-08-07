/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2023 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#ifndef DS015_GNSS_HPP_
#define DS015_GNSS_HPP_

#include "cyphal.hpp"
#include "ds015/service/gnss/Gnss_0_1.h"
#include "ds015/service/gnss/Covariance_0_1.h"
#include "ds015/service/gnss/RelativePosition_0_1.h"


struct Ds015GnssGnssPublisher: public CyphalPublisher {
    Ds015GnssGnssPublisher(Cyphal* driver_, CanardPortID port_id) : CyphalPublisher(driver_, port_id) {};
    void publish();
    ds015_service_gnss_Gnss_0_1 msg;
};

struct Ds015GnssCovariancePublisher: public CyphalPublisher {
    Ds015GnssCovariancePublisher(Cyphal* driver_, CanardPortID port_id) : CyphalPublisher(driver_, port_id) {};
    void publish();
    ds015_service_gnss_Covariance_0_1 msg;
};

struct Ds015GnssRelativePositionPublisher: public CyphalPublisher {
    Ds015GnssRelativePositionPublisher(Cyphal* driver_, CanardPortID port_id) : CyphalPublisher(driver_, port_id) {};
    void publish();
    ds015_service_gnss_RelativePosition_0_1 msg;
};

#endif  // DS015_GNSS_HPP_
