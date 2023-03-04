/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#include <iostream>
#include "hitl_application.hpp"
#include <chrono>
#include <thread>

int main (int argc, char *argv[]) {
    HitlApplication application;
    int init_res = application.init(-35.3632621, +149.1652374, 584.19);
    if (init_res < 0) {
        std::cout << "Error: " << init_res << std::endl;
        return -1;
    }
    std::cout << "Hello, world." << std::endl;

    const std::array<double, 3> local_position = {0, 0, 0};
    const std::array<double, 3> linear_vel = {0, 0, 0};
    const std::array<double, 3> linear_accel = {0, 0, -9.8};
    const std::array<double, 4> orientation_wxyz = {1, 0, 0, 0};
    const std::array<double, 3> ang_vel = {0, 0, 0};

    uavcan_si_sample_magnetic_field_strength_Vector3_1_0 mag_tesla;
    mag_tesla.tesla[0] = 1e-07 * 232;
    mag_tesla.tesla[1] = 1e-07 * 52;
    mag_tesla.tesla[2] = 1e-07 * (-528);

    while(true) {
        application.process(local_position, linear_vel, linear_accel, orientation_wxyz, ang_vel, mag_tesla);
    }
}
