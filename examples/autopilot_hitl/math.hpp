/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2023 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#ifndef HITL_MATH_HPP_
#define HITL_MATH_HPP_

#include <array>
#include "reg/udral/physics/kinematics/geodetic/Point_0_1.h"

typedef std::array<double, 3> Vector3;
typedef std::array<double, 4> Quaternion;
void rotate_vector_by_quaternion(const Vector3& v, const Quaternion& q, Vector3& res);

void local_pose_to_geodetic_point(const Vector3& home,
                                  const Vector3& local_position,
                                  reg_udral_physics_kinematics_geodetic_Point_0_1& point);

float local_height_to_baro_pressure_pascal(float local_height_meters);

#endif  // HITL_MATH_HPP_
