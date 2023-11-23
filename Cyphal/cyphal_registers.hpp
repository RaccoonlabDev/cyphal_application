/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#ifndef CYPHAL_CYPHAL_REGISTERS_HPP_
#define CYPHAL_CYPHAL_REGISTERS_HPP_

#include "cyphal_subscribers.hpp"
#include "uavcan/_register/Access_1_0.h"
#include "uavcan/_register/List_1_0.h"
#include "storage.h"

namespace cyphal {

struct RegisterListRequest: public CyphalSubscriber {
    explicit RegisterListRequest(Cyphal* driver_) :
        CyphalSubscriber(driver_, uavcan_register_List_1_0_FIXED_PORT_ID_) {};
    void callback(const CanardRxTransfer& transfer) override;
    bool isService() const override;
private:
    ParamIndex_t parseRequest(const CanardRxTransfer& transfer) const;
    void makeResponse(const CanardRxTransfer& transfer, ParamIndex_t index);
};

struct RegisterAccessRequest: public CyphalSubscriber {
    explicit RegisterAccessRequest(Cyphal* driver_);
    void callback(const CanardRxTransfer& transfer) override;
    bool isService() const override;
private:
    ParamIndex_t parseRequest(const CanardRxTransfer& transfer);
    void makeResponse(const CanardRxTransfer& transfer, ParamIndex_t reg_index);
    void writeParam(ParamIndex_t reg_index);
    void readParam(uavcan_register_Access_Response_1_0& response_msg, ParamIndex_t reg_index);

    uavcan_register_Access_Request_1_0 _request_msg;
    CanardTransferMetadata _transfer_metadata;
};

}  // namespace cyphal

#endif  // CYPHAL_CYPHAL_REGISTERS_HPP_
