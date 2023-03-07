/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#ifndef HITL_APPLICATION_HPP_
#define HITL_APPLICATION_HPP_

#include "cyphal.hpp"
#include "actuator.hpp"
#include "gnss.hpp"
#include "barometer.hpp"
#include "magnetometer.hpp"
#include "imu.hpp"

class HitlApplication {
public:
    HitlApplication() : setpoint(&cyphal),
                        readiness(&cyphal),
                        gps_point(&cyphal,          2010),
                        gps_sats(&cyphal,           2011),
                        gps_status(&cyphal,         2012),
                        gps_pdop(&cyphal,           2013),
                        baro_pressure(&cyphal,      2020),
                        baro_temperature(&cyphal,   2021),
                        magnetometer(&cyphal,       2030),
                        accel(&cyphal,              2040),
                        gyro(&cyphal,               2041) {}
    int init(double home_lat, double home_lon, double home_alt);
    void process(const std::array<double, 3>& local_position,
                 const std::array<double, 3>& linear_vel,
                 const std::array<double, 3>& linear_accel,
                 const std::array<double, 4>& orientation_wxyz,
                 const std::array<double, 3>& ang_vel,
                 const uavcan_si_sample_magnetic_field_strength_Vector3_1_0& magnetic_field);

    uint32_t get_servo_pwm(std::array<uint16_t, 16>& servo_pwm);
    void clear_servo_pwm_counter();
    void set_time_factor(double time_factor);
private:
    Cyphal cyphal;

    SetpointSubscriber setpoint;
    ReadinessSubscriber readiness;

    GpsPointPublisher gps_point;
    Int16Publisher gps_sats;
    Int16Publisher gps_status;
    Int16Publisher gps_pdop;

    BaroPressurePublisher baro_pressure;
    BaroTemperaturePublisher baro_temperature;

    MagneticFieldPublisher magnetometer;

    ImuAccelPublisher accel;
    ImuGyroPublisher gyro;

    std::array<double, 3> home;
    double _time_factor{1.0};
};

#endif  // HITL_APPLICATION_HPP_
