/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2023 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#include "gnss.hpp"

void Ds015GnssGnssPublisher::publish() {
    static uint8_t buffer[ds015_service_gnss_Gnss_0_1_EXTENT_BYTES_];
    size_t buffer_size = ds015_service_gnss_Gnss_0_1_EXTENT_BYTES_;
    int32_t result = ds015_service_gnss_Gnss_0_1_serialize_(&msg, buffer, &buffer_size);
    if (NUNAVUT_SUCCESS == result) {
        push(buffer_size, buffer);
    }
}

void Ds015GnssCovariancePublisher::publish() {
    static uint8_t buffer[ds015_service_gnss_Covariance_0_1_EXTENT_BYTES_];
    size_t buffer_size = ds015_service_gnss_Covariance_0_1_EXTENT_BYTES_;
    int32_t result = ds015_service_gnss_Covariance_0_1_serialize_(&msg, buffer, &buffer_size);
    if (NUNAVUT_SUCCESS == result) {
        push(buffer_size, buffer);
    }
}

void Ds015GnssRelativePositionPublisher::publish() {
    static uint8_t buffer[ds015_service_gnss_RelativePosition_0_1_EXTENT_BYTES_];
    size_t buffer_size = ds015_service_gnss_RelativePosition_0_1_EXTENT_BYTES_;
    int32_t result = ds015_service_gnss_RelativePosition_0_1_serialize_(&msg, buffer, &buffer_size);
    if (NUNAVUT_SUCCESS == result) {
        push(buffer_size, buffer);
    }
}
