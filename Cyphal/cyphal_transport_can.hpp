/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022-2023 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#ifndef CYPHAL_CYPHAL_TRANSPORT_CAN_HPP_
#define CYPHAL_CYPHAL_TRANSPORT_CAN_HPP_

#include "canard.h"

namespace cyphal {

class CyphalTransportCan {
public:
    CyphalTransportCan() = default;
    bool init(uint32_t can_speed, uint8_t can_driver_idx);
    bool receive(CanardFrame* can_frame);
    bool transmit(const CanardTxQueueItem* transfer);
    uint8_t get_rx_queue_size();
private:
    uint8_t _out_payload[64];  ///< 8 for BXCAN, 64 for FDCAN
    uint8_t _can_driver_idx;
};

}  // namespace cyphal

#endif  // CYPHAL_CYPHAL_TRANSPORT_CAN_HPP_
