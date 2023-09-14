/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022-2023 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#include "cyphal_transport_can.hpp"
#include <string.h>
#include "main.h"

typedef struct{
    FDCAN_HandleTypeDef* handler;
    FDCAN_TxHeaderTypeDef tx_header;
    uint8_t rx_buf[8];
    size_t err_counter;
    size_t tx_counter;
    size_t rx_counter;
} CanDriver;

#ifndef CYPHAL_NUM_OF_CAN_BUSES
    #error "CYPHAL_NUM_OF_CAN_BUSES must be specified!"
#elif CYPHAL_NUM_OF_CAN_BUSES == 1
    extern FDCAN_HandleTypeDef hfdcan1;
    static CanDriver driver[CYPHAL_NUM_OF_CAN_BUSES] = {
        {.handler = &hfdcan1},
    };
#elif CYPHAL_NUM_OF_CAN_BUSES == 2
    extern FDCAN_HandleTypeDef hfdcan1;
    extern FDCAN_HandleTypeDef hfdcan2;
    static CanDriver driver[CYPHAL_NUM_OF_CAN_BUSES] = {
        {.handler = &hfdcan1},
        {.handler = &hfdcan2}
    };
#endif


bool CyphalTransportCan::init(uint32_t, uint8_t can_driver_idx) {
    if (can_driver_idx >= CYPHAL_NUM_OF_CAN_BUSES) {
        return false;
    }

    _can_driver_idx = can_driver_idx;
    driver[can_driver_idx].tx_header.IdType = FDCAN_EXTENDED_ID;
    driver[can_driver_idx].tx_header.TxFrameType = FDCAN_DATA_FRAME;
    driver[can_driver_idx].tx_header.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    driver[can_driver_idx].tx_header.BitRateSwitch = FDCAN_BRS_OFF;
    driver[can_driver_idx].tx_header.FDFormat = FDCAN_CLASSIC_CAN;
    driver[can_driver_idx].tx_header.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
    driver[can_driver_idx].tx_header.MessageMarker = 0;

    FDCAN_FilterTypeDef sFilterConfig;
    sFilterConfig.IdType = FDCAN_EXTENDED_ID;
    sFilterConfig.FilterIndex = 0;
    sFilterConfig.FilterType = FDCAN_FILTER_MASK;
    sFilterConfig.FilterConfig = FDCAN_FILTER_DISABLE;

    if (HAL_FDCAN_ConfigFilter(driver[can_driver_idx].handler, &sFilterConfig) != HAL_OK) {
        return false;
    } else if (HAL_FDCAN_Start(driver[can_driver_idx].handler) != HAL_OK) {
        return false;
    }

    return true;
}

bool CyphalTransportCan::receive(CanardFrame* can_frame) {
    memset(_out_payload, 0x00, 8);
    FDCAN_RxHeaderTypeDef rx_header;
    auto& drv = driver[_can_driver_idx];

    auto res = HAL_FDCAN_GetRxMessage(drv.handler, FDCAN_RX_FIFO0, &rx_header, drv.rx_buf);

    if (res != HAL_OK) {
        return false;
    }

    drv.rx_counter++;
    can_frame->extended_can_id = rx_header.Identifier;
    can_frame->payload_size = rx_header.DataLength >> 4*4;
    memcpy(_out_payload, drv.rx_buf, can_frame->payload_size);
    can_frame->payload = _out_payload;
    return true;
}

bool CyphalTransportCan::transmit(const CanardTxQueueItem* transfer) {
    if (transfer->frame.payload_size == 0) {
        return false;
    }

    size_t num_of_frames = transfer->frame.payload_size / 8;
    size_t length_of_last_frame = transfer->frame.payload_size % 8;
    if (length_of_last_frame == 0) {
        length_of_last_frame = 8;
    } else {
        num_of_frames++;
    }

    auto& drv = driver[_can_driver_idx];

    for (size_t frame_idx = 0; frame_idx < num_of_frames; frame_idx++) {
        uint8_t payload_size = (frame_idx + 1 < num_of_frames) ? 8 : length_of_last_frame;
        drv.tx_header.Identifier = transfer->frame.extended_can_id;
        drv.tx_header.DataLength = payload_size << 4*4;
        uint8_t* pTxData = (uint8_t*)(((uint8_t*)transfer->frame.payload) + frame_idx * 8);
        auto res = HAL_FDCAN_AddMessageToTxFifoQ(drv.handler, &drv.tx_header, pTxData);
        if (res == HAL_OK) {
            drv.tx_counter++;
            return true;
        } else {
            drv.err_counter++;
            return false;
        }

        ///< we need to have a delay between each push
        for (uint32_t idx = 0; idx < 1000; idx++) {
            asm("NOP");
        }
    }
    return false;
}

uint8_t CyphalTransportCan::get_rx_queue_size() {
    return HAL_FDCAN_GetRxFifoFillLevel(&hfdcan1, FDCAN_RX_FIFO0);
}
