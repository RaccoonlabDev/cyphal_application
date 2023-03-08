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

static void print_log_info_periodically(uint32_t& json_sensors_recv_counter,
                                        uint32_t cyphal_servo_recv_counter,
                                        double crnt_ts,
                                        HitlApplication& cyphal_hitl);

int main() {
    ArdupilotJson ap;
    ap.init();

    HitlApplication cyphal_hitl;
    int init_res = cyphal_hitl.init(-35.3632621, +149.1652374, 584.19);
    if (init_res < 0) {
        std::cout << "HITL Initialization Error: " << init_res << std::endl;
        return -1;
    }
    std::cout << "Hello, HITL." << std::endl;

    std::array<uint16_t, 16> servo_pwm;
    uint32_t json_sensors_recv_counter = 0;

    ap.send_servo(servo_pwm);

    while(true) {
        cyphal_hitl.process(ap.position, ap.velocity, ap.accel, ap.quaternion, ap.gyro);

        uint32_t cyphal_servo_recv_counter = cyphal_hitl.get_servo_pwm(servo_pwm);
        ap.send_servo(servo_pwm);

        if (ap.receive_sensors()) {
            json_sensors_recv_counter++;
        };

        print_log_info_periodically(json_sensors_recv_counter,
                                    cyphal_servo_recv_counter,
                                    ap.timestamp,
                                    cyphal_hitl);
    }
}

void print_log_info_periodically(uint32_t& json_sensors_recv_counter,
                                 uint32_t cyphal_servo_recv_counter,
                                 double crnt_ts,
                                 HitlApplication& cyphal_hitl) {
    uint32_t crnt_time_ms = HAL_GetTick();
    static uint32_t last_hint_time_ms = 0;
    if (crnt_time_ms < last_hint_time_ms + 1000) {
        return;
    }
    last_hint_time_ms = crnt_time_ms;

    static double prev_ts = 0.0;
    double time_factor = std::clamp(crnt_ts - prev_ts, 0.5, 2.0);
    std::cout << "Status: "
              << "gz time factor = " << (int)(100 * time_factor) << "%, "
              << "cyphal input = " << (int)(0.5 * cyphal_servo_recv_counter) << "%, "
              << "json input = " << (int)(0.1 * json_sensors_recv_counter) << "%."
              << std::endl;

    if (time_factor < 0.8 || time_factor > 1.1) {
        std::cout << "\033[1;31m"
                  << "Gazebo time factor should be ~ 1.0. "
                  << "Now it is not enough for a stable flight. "
                  << "Try headless-rendering for gazebo."
                  << "\033[0m\n";
    }

    if (cyphal_servo_recv_counter < 180) {
        std::cout << "\033[1;31m"
                  << "Setpoint rate should be ~200 Hz. "
                  << "Don't you have a problem with transport layer?"
                  << "\033[0m\n";
    }

    cyphal_hitl.clear_servo_pwm_counter();
    cyphal_hitl.set_time_factor(time_factor);
    json_sensors_recv_counter = 0;
    prev_ts = crnt_ts;
}
