/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022-2023 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#include "cyphal_transport_can.hpp"
#include <string.h>
#include "main.h"
#include "ring_buffer.hpp"
#include "params.hpp"

typedef struct{
    FDCAN_HandleTypeDef* handler;
    FDCAN_TxHeaderTypeDef tx_header;
    uint8_t rx_buf[8];
    size_t err_counter;
    size_t tx_counter;
    size_t rx_counter;
} CanDriver;

struct CanFrame {
    uint32_t identifier;
    uint8_t data[8];
    uint8_t size;
};

static RingBuffer<CanFrame, 40> ring_buffer;

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

static void configure_filters();

bool CyphalTransportCan::init(uint32_t, uint8_t can_driver_idx) {
    if (can_driver_idx >= CYPHAL_NUM_OF_CAN_BUSES) {
        return false;
    }

    auto& drv = driver[can_driver_idx];

    _can_driver_idx = can_driver_idx;
    drv.tx_header.IdType = FDCAN_EXTENDED_ID;
    drv.tx_header.TxFrameType = FDCAN_DATA_FRAME;
    drv.tx_header.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    drv.tx_header.BitRateSwitch = FDCAN_BRS_OFF;
    drv.tx_header.FDFormat = FDCAN_CLASSIC_CAN;
    drv.tx_header.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
    drv.tx_header.MessageMarker = 0;

    configure_filters();

    if (HAL_FDCAN_Start(drv.handler) != HAL_OK) {
        return false;
    }

    if (HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK) {
        return false;
    }

    return true;
}

// Not yet
void configure_filters() {
    // uint32_t can_id_1 = 0x13000000 + node_id + (dest_node_id << 7) + (384 << 14);
    // uint32_t can_id_2 = 0x13000000 + node_id + (dest_node_id << 7) + (385 << 14);

    // HAL_FDCAN_ConfigGlobalFilter(&hfdcan1, FDCAN_REJECT, FDCAN_REJECT, FDCAN_REJECT_REMOTE, FDCAN_REJECT_REMOTE);
    // FDCAN_FilterTypeDef sFilterConfig;
    // sFilterConfig.IdType = FDCAN_EXTENDED_ID;
    // sFilterConfig.FilterIndex = 0;
    // sFilterConfig.FilterType = FDCAN_FILTER_DUAL;
    // sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
    // sFilterConfig.FilterID1 = can_id_1;
    // sFilterConfig.FilterID2 = can_id_2;

    // HAL_FDCAN_ConfigFilter(&hfdcan1, &sFilterConfig);
}

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs) {
    FDCAN_RxHeaderTypeDef rx_header;
    const uint8_t fifo_size = HAL_FDCAN_GetRxFifoFillLevel(&hfdcan1, FDCAN_RX_FIFO0);
    CanFrame frame;

    for (uint_fast8_t idx = 0; idx < fifo_size; idx++) {
        HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &rx_header, frame.data);
        frame.identifier = rx_header.Identifier;
        frame.size = rx_header.DataLength >> 4*4;
        ring_buffer.push(frame);
    }
}

bool CyphalTransportCan::receive(CanardFrame* can_frame) {
    if (ring_buffer.get_size() == 0) {
        return false;
    }

    static uint32_t rb_max_size = 0;
    uint32_t rb_size = ring_buffer.get_size();
    if (rb_size > rb_max_size) {
        rb_max_size = rb_size;
    }
    paramsSetIntegerValue(PARAM_BOOTLOADER_INTERNAL, rb_max_size);

    driver[_can_driver_idx].rx_counter++;
    __disable_irq();
    CanFrame frame = ring_buffer.pop();
    __enable_irq();
    can_frame->extended_can_id = frame.identifier;
    can_frame->payload_size = frame.size;
    memcpy(_out_payload, frame.data, can_frame->payload_size);
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
        if (HAL_FDCAN_GetTxFifoFreeLevel(drv.handler) == 0) {
            drv.err_counter++;
            return false;
        }

        __disable_irq();
        auto res = HAL_FDCAN_AddMessageToTxFifoQ(drv.handler, &drv.tx_header, pTxData);
        __enable_irq();

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
    return ring_buffer.get_size();
}
