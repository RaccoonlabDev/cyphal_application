/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#include <iostream>
#include "hitl_application.hpp"
#include "ap_json.hpp"
#include "main.h"
#include <chrono>
#include <thread>

int main() {
    ArdupilotJson ap_json;
    ap_json.init();

    HitlApplication application;
    int init_res = application.init(-35.3632621, +149.1652374, 584.19);
    if (init_res < 0) {
        std::cout << "HITL Initialization Error: " << init_res << std::endl;
        return -1;
    }
    std::cout << "Hello, HITL." << std::endl;

    uavcan_si_sample_magnetic_field_strength_Vector3_1_0 mag_tesla;
    mag_tesla.tesla[0] = 1e-07 * 232;
    mag_tesla.tesla[1] = 1e-07 * 52;
    mag_tesla.tesla[2] = 1e-07 * (-528);

    std::array<uint16_t, 16> servo_pwm;

    uint32_t last_json_process_time_ms = 0;
    uint32_t json_sensors_recv_counter = 0;
    uint32_t last_hint_time_ms = 0;
    uint32_t process_counter = 0;

    ap_json.send_servo(servo_pwm);

    while(true) {
        uint32_t crnt_time_ms = HAL_GetTick();
        application.process(ap_json.position,
                            ap_json.velocity,
                            ap_json.accel,
                            ap_json.quaternion,
                            ap_json.gyro,
                            mag_tesla);
        process_counter++;

        uint32_t cyphal_servo_recv_counter = application.get_servo_pwm(servo_pwm);
        ap_json.send_servo(servo_pwm);

        if (ap_json.receive_sensors()) {
            mag_tesla.tesla[0] = 1e-07 * ap_json.mag[0];
            mag_tesla.tesla[1] = 1e-07 * ap_json.mag[1];
            mag_tesla.tesla[2] = 1e-07 * ap_json.mag[2];
            json_sensors_recv_counter++;
        };

        if (crnt_time_ms > last_hint_time_ms + 1000) {
            last_hint_time_ms = crnt_time_ms;
            std::cout << "Status: "
                      << cyphal_servo_recv_counter << " and "
                      << json_sensors_recv_counter << " / "
                      << process_counter << ". " << std::endl;
            application.clear_servo_pwm_counter();
            json_sensors_recv_counter = 0;
            process_counter = 0;
        }
    }
}
