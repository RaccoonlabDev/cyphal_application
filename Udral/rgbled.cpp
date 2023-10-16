/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2023 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#include "rgbled.hpp"
#include "main.h"
#include "params.hpp"

void HighColorPublisher::publish(uint8_t red, uint8_t green, uint8_t blue) {
    reg_udral_physics_optics_HighColor_0_1 msg = {red, green, blue};

    uint8_t buffer[reg_udral_physics_optics_HighColor_0_1_SERIALIZATION_BUFFER_SIZE_BYTES_];
    size_t buffer_size = reg_udral_physics_optics_HighColor_0_1_SERIALIZATION_BUFFER_SIZE_BYTES_;
    int32_t result = reg_udral_physics_optics_HighColor_0_1_serialize_(&msg, buffer, &buffer_size);
    if (NUNAVUT_SUCCESS == result) {
        push(buffer_size, buffer);
    }
}


int8_t HighColorSubscriber::init() {
    port_id = paramsGetIntegerValue(IntParamsIndexes::RGBLED_ID);
    if (driver->subscribe(this,
                reg_udral_physics_optics_HighColor_0_1_EXTENT_BYTES_,
                CanardTransferKindMessage) < 0) {
        return -1;
    }

    return 0;
}

void HighColorSubscriber::callback(const CanardRxTransfer& transfer) {
    const uint8_t* payload = static_cast<const uint8_t*>(transfer.payload);
    size_t payload_len = transfer.payload_size;
    reg_udral_physics_optics_HighColor_0_1_deserialize_(&_msg, payload, &payload_len);
}
