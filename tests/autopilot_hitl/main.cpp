/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#include "main.h"

#include <algorithm>
#include <iostream>
#include <chrono>
#include <thread>

#include "hitl_application.hpp"
#include "ap_json.hpp"

int main() {
    ArdupilotJson ap_json;
    ap_json.init();

    HitlApplication cyphal_hitl;
    int init_res = cyphal_hitl.init(-35.3632621, +149.1652374, 584.19);
    if (init_res < 0) {
        std::cout << "HITL Initialization Error: " << init_res << std::endl;
        return -1;
    }
    std::cout << "Hello, HITL." << std::endl;

    std::array<uint16_t, 16> servo_pwm;
    uint32_t json_sensors_recv_counter = 0;
    uint32_t last_hint_time_ms = 0;
    uint32_t process_counter = 0;
    double prev_ts = 0.0;

    ap_json.send_servo(servo_pwm);

    while(true) {
        uint32_t crnt_time_ms = HAL_GetTick();
        cyphal_hitl.process(ap_json.position,
                            ap_json.velocity,
                            ap_json.accel,
                            ap_json.quaternion,
                            ap_json.gyro);
        process_counter++;

        uint32_t cyphal_servo_recv_counter = cyphal_hitl.get_servo_pwm(servo_pwm);
        ap_json.send_servo(servo_pwm);

        if (ap_json.receive_sensors()) {
            json_sensors_recv_counter++;
        };

        if (crnt_time_ms > last_hint_time_ms + 1000) {
            last_hint_time_ms = crnt_time_ms;
            double time_factor = std::clamp(ap_json.timestamp - prev_ts, 0.7, 1.0);
            std::cout << "Status: "
                      << "time_factor = " << (int)(100 * time_factor) << "%, "
                      << cyphal_servo_recv_counter << " and "
                      << json_sensors_recv_counter << " / "
                      << process_counter << ". " << std::endl;
            cyphal_hitl.clear_servo_pwm_counter();
            json_sensors_recv_counter = 0;
            process_counter = 0;
            prev_ts = ap_json.timestamp;
        }
    }
}
