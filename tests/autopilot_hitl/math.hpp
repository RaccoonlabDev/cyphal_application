/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2023 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#ifndef HITL_MATH_HPP_
#define HITL_MATH_HPP_

#include <array>

typedef std::array<double, 3> Vector3;
typedef std::array<double, 4> Quaternion;
void rotate_vector_by_quaternion(const Vector3& v, const Quaternion& q, Vector3& res);

#endif  // HITL_MATH_HPP_
