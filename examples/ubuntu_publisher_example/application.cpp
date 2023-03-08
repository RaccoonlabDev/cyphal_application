/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#include "application.hpp"
#include <iostream>
#include <chrono>
#include <thread>
#include "reg/udral/service/actuator/common/Feedback_0_1.h"
#include "main.h"
#include "params.hpp"
#include "string_params.hpp"

void FeedbackPublisher::process() {
    static uint32_t prev_pub_time_ms = 1000;
    uint32_t crnt_time_ms = HAL_GetTick();
    if (crnt_time_ms < prev_pub_time_ms + 1000) {
        return;
    }
    prev_pub_time_ms = crnt_time_ms;

    reg_udral_service_actuator_common_Feedback_0_1 msg;
    msg.heartbeat.health.value = uavcan_node_Health_1_0_NOMINAL;
    msg.heartbeat.readiness.value = 0;
    msg.demand_factor_pct = 0;

    uint8_t buffer[reg_udral_service_actuator_common_Feedback_0_1_EXTENT_BYTES_];
    size_t buffer_size = reg_udral_service_actuator_common_Feedback_0_1_EXTENT_BYTES_;
    int32_t result = reg_udral_service_actuator_common_Feedback_0_1_serialize_(&msg, buffer, &buffer_size);
    if (NUNAVUT_SUCCESS == result) {
        push(buffer_size, buffer);
    }
}


int Application::init() {
    romInit(0, 1);
    paramsInit(IntParamsIndexes::INTEGER_PARAMS_AMOUNT, NUM_OF_STR_PARAMS);
    paramsLoadFromFlash();
    int init_res = cyphal.init();
    if (init_res < 0) {
        std::cout << "Error: " << init_res << std::endl;
        return -1;
    }

    return 0;
}

void Application::process() {
    feedback.process();
    cyphal.process();
}
