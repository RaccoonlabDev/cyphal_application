/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#include "hitl_application.hpp"
#include <algorithm>
#include <iostream>
#include <chrono>
#include "main.h"
#include "params.hpp"
#include "string_params.hpp"
#include "math.hpp"

uint32_t HAL_GetTick() {
    static auto time_start = std::chrono::steady_clock::now();
    auto time_now = std::chrono::steady_clock::now();
    auto elapsed_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(time_now - time_start).count();
    return elapsed_time_ms;
}

int HitlApplication::init(double home_lat, double home_lon, double home_alt) {
    romInit(0, 1);
    paramsInit(IntParamsIndexes::INTEGER_PARAMS_AMOUNT, NUM_OF_STR_PARAMS);
    paramsLoadFromFlash();
    int init_res = cyphal.init();
    if (init_res < 0) {
        std::cout << "Cyphal Initialization Error: " << init_res << std::endl;
        return -1;
    }

    setpoint.init();
    readiness.init();

    home[0] = home_lat;
    home[1] = home_lon;
    home[2] = home_alt;

    return 0;
}

void HitlApplication::process(const std::array<double, 3>& local_position,
                              const std::array<double, 3>& linear_vel,
                              const std::array<double, 3>& linear_accel,
                              const std::array<double, 4>& orientation_wxyz,
                              const std::array<double, 3>& ang_vel) {
    uint32_t crnt_time_ms = HAL_GetTick();
    cyphal.process();

    static uint32_t pub_10_hz_prev_time_ms = 1000;
    if (crnt_time_ms >= pub_10_hz_prev_time_ms + 100) {
        pub_10_hz_prev_time_ms = crnt_time_ms;

        reg_udral_physics_kinematics_geodetic_PointStateVarTs_0_1 gps_point_msg;
        gps_point_msg.timestamp.microsecond = HAL_GetTick() * 1000;
        gps_point_msg.value.position.value.latitude =  (home[0]  + 0.000008982 * local_position[0]) * 0.017453293;
        gps_point_msg.value.position.value.longitude = (home[1] + 0.000011015 * local_position[1]) * 0.017453293;
        gps_point_msg.value.position.value.altitude.meter = home[2] - local_position[2];

        gps_point_msg.value.velocity.value.meter_per_second[0] = linear_vel[0] * _time_factor;
        gps_point_msg.value.velocity.value.meter_per_second[1] = linear_vel[1] * _time_factor;
        gps_point_msg.value.velocity.value.meter_per_second[2] = linear_vel[2] * _time_factor;
        gps_point.publish(gps_point_msg);
        gps_sats.publish(10);
        gps_status.publish(3);
        gps_pdop.publish(1);

        static float baro_noise = 0;
        baro_noise = (int)(baro_noise + 1) % 10;
        uavcan_si_sample_pressure_Scalar_1_0 pressure;
        pressure.timestamp.microsecond = HAL_GetTick() * 1000;
        pressure.pascal = 94500 + (float)(11.3*local_position[2]) + 0.02f * baro_noise;
        baro_pressure.publish(pressure);

        uavcan_si_sample_temperature_Scalar_1_0 temperature{.kelvin = 312.0};
        baro_temperature.publish(temperature);
    }

    static uint32_t pub_50_hz_prev_time_ms = 1000;
    if (crnt_time_ms >= pub_50_hz_prev_time_ms + 20) {
        const Vector3 initial_mag{232, 52, -528};
        Vector3 mag_rotated;
        rotate_vector_by_quaternion(initial_mag, orientation_wxyz, mag_rotated);
        uavcan_si_sample_magnetic_field_strength_Vector3_1_0 magnetic_field;
        magnetic_field.tesla[0] = 1e-07 * mag_rotated[0];
        magnetic_field.tesla[1] = 1e-07 * mag_rotated[1];
        magnetic_field.tesla[2] = 1e-07 * mag_rotated[2];

        pub_50_hz_prev_time_ms = crnt_time_ms;
        magnetometer.publish(magnetic_field);
    }

    static uint32_t pub_200_hz_prev_time_us = 1000000;
    if (crnt_time_ms * 1000 >= pub_200_hz_prev_time_us + 5000) {
        pub_200_hz_prev_time_us += 5000;

        uavcan_si_sample_acceleration_Vector3_1_0 accel_msg;
        accel_msg.meter_per_second_per_second[0] = linear_accel[0];
        accel_msg.meter_per_second_per_second[1] = linear_accel[1];
        accel_msg.meter_per_second_per_second[2] = linear_accel[2];
        accel.publish(accel_msg);

        uavcan_si_sample_angular_velocity_Vector3_1_0 gyro_msg;
        gyro_msg.radian_per_second[0] = ang_vel[0] * _time_factor;
        gyro_msg.radian_per_second[1] = ang_vel[1] * _time_factor;
        gyro_msg.radian_per_second[2] = ang_vel[2] * _time_factor;
        gyro.publish(gyro_msg);
    }

    cyphal.process();
}

uint32_t HitlApplication::get_servo_pwm(std::array<uint16_t, 16>& servo_pwm) {
    auto sp = setpoint.get_setpoint();
    for (size_t sp_idx = 0; sp_idx < 4; sp_idx++) {
        servo_pwm[sp_idx] = 1000 + 1000.0 * sp.value[sp_idx];
    }

    for (size_t sp_idx = 4; sp_idx < 16; sp_idx++) {
        servo_pwm[sp_idx] = 0;
    }

    return setpoint.get_recv_counter();
}

void HitlApplication::clear_servo_pwm_counter() {
    setpoint.clear_recv_counter();
}

void HitlApplication::set_time_factor(double time_factor) {
    _time_factor = std::clamp(time_factor, 0.7, 1.0);
}
