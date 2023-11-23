/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2023 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#include "gnss.hpp"

namespace cyphal {

void Ds015GnssGnssPublisher::publish() {
    if (!isEnabled()) {
        return;
    }

    static uint8_t buffer[ds015_service_gnss_Gnss_0_1_EXTENT_BYTES_];
    size_t buffer_size = ds015_service_gnss_Gnss_0_1_EXTENT_BYTES_;
    int32_t result = ds015_service_gnss_Gnss_0_1_serialize_(&msg, buffer, &buffer_size);
    if (NUNAVUT_SUCCESS == result) {
        push(buffer_size, buffer);
    }
}

void Ds015GnssCovariancePublisher::publish() {
    if (!isEnabled()) {
        return;
    }

    static uint8_t buffer[ds015_service_gnss_Covariance_0_1_EXTENT_BYTES_];
    size_t buffer_size = ds015_service_gnss_Covariance_0_1_EXTENT_BYTES_;
    int32_t result = ds015_service_gnss_Covariance_0_1_serialize_(&msg, buffer, &buffer_size);
    if (NUNAVUT_SUCCESS == result) {
        push(buffer_size, buffer);
    }
}

void Ds015GnssRelativePositionPublisher::publish() {
    if (!isEnabled()) {
        return;
    }

    static uint8_t buffer[ds015_service_gnss_RelativePosition_0_1_EXTENT_BYTES_];
    size_t buffer_size = ds015_service_gnss_RelativePosition_0_1_EXTENT_BYTES_;
    int32_t result = ds015_service_gnss_RelativePosition_0_1_serialize_(&msg, buffer, &buffer_size);
    if (NUNAVUT_SUCCESS == result) {
        push(buffer_size, buffer);
    }
}

void Ds015GnssTimePublisher::publish() {
    if (!isEnabled()) {
        return;
    }

    static uint8_t buffer[uavcan_time_SynchronizedTimestamp_1_0_EXTENT_BYTES_];
    size_t buffer_size = uavcan_time_SynchronizedTimestamp_1_0_EXTENT_BYTES_;
    int32_t result = uavcan_time_SynchronizedTimestamp_1_0_serialize_(&msg, buffer, &buffer_size);
    if (NUNAVUT_SUCCESS == result) {
        push(buffer_size, buffer);
    }
}

}  // namespace cyphal
