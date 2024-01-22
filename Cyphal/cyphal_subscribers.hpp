/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022-2023 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#ifndef CYPHAL_CYPHAL_SUBSCRIBERS_HPP_
#define CYPHAL_CYPHAL_SUBSCRIBERS_HPP_

#include "canard.h"
#include "uavcan/node/GetInfo_1_0.h"
#include "uavcan/node/ExecuteCommand_1_0.h"

namespace cyphal {

class Cyphal;

class CyphalSubscriber {
public:
    CyphalSubscriber(Cyphal* driver_, CanardPortID port_id_) : driver(driver_), port_id(port_id_) {}
    virtual void callback(const CanardRxTransfer& transfer) = 0;
    bool isEnabled() const;
    virtual bool isService() const;
    CanardRxSubscription subscription;
    Cyphal* driver;
    CanardPortID port_id;
protected:
    virtual ~CyphalSubscriber() = default;
};

class NodeGetInfoSubscriber: public CyphalSubscriber {
public:
    /**
     * @brief Initialize the Node Info with data provided on the compile time:
     * - software_version
     * - software_vcs_revision_id
     */
    explicit NodeGetInfoSubscriber(Cyphal* driver_);

    /**
     * @brief Initialize the Node Info with data provided on the initialization time:
     * - hardware_version
     * - unique_id
     */
    void init();

    void callback(const CanardRxTransfer& transfer) override;
    static void setHardwareVersion(uint8_t major, uint8_t minor);
    static void setHardwareVersion(uavcan_node_Version_1_0 version);
    bool isService() const override;
private:
    void updateNodeName();
    uavcan_node_GetInfo_Response_1_0 get_info_response = {};
    uint8_t _transfer_id = 0;
    static uavcan_node_Version_1_0 hw_version;
};

struct ExecuteCommandSubscriber: public CyphalSubscriber {
    explicit ExecuteCommandSubscriber(Cyphal* driver_) :
        CyphalSubscriber(driver_, uavcan_node_ExecuteCommand_1_0_FIXED_PORT_ID_) {};
    void callback(const CanardRxTransfer& transfer) override;
    bool isService() const override;
private:
    uint8_t _transfer_id = 0;
};

}  // namespace cyphal

#endif  // CYPHAL_CYPHAL_SUBSCRIBERS_HPP_
