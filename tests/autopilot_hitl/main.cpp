/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#include <iostream>
#include "hitl_application.hpp"
#include "ap_json.hpp"
#include <chrono>
#include <thread>

int main (int argc, char *argv[]) {
    ArdupilotJson ap_json;
    ap_json.init();

    HitlApplication application;
    int init_res = application.init(-35.3632621, +149.1652374, 584.19);
    if (init_res < 0) {
        std::cout << "Error: " << init_res << std::endl;
        return -1;
    }
    std::cout << "Hello, HITL." << std::endl;

    uavcan_si_sample_magnetic_field_strength_Vector3_1_0 mag_tesla;
    mag_tesla.tesla[0] = 1e-07 * 232;
    mag_tesla.tesla[1] = 1e-07 * 52;
    mag_tesla.tesla[2] = 1e-07 * (-528);

    std::array<uint16_t, 16> servo_pwm;

    while(true) {
        application.get_servo_pwm(servo_pwm);
        ap_json.send_servo(servo_pwm);

        ap_json.receive_sensors();
        application.process(ap_json.position,
                            ap_json.velocity,
                            ap_json.accel,
                            ap_json.quaternion,
                            ap_json.gyro,
                            mag_tesla);
    }
}
