/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2023 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#include "math.hpp"
#include <cstddef>

double dot(const Vector3& u, const Vector3& v) {
    return u[0] * v[0] + u[1] * v[1] + u[2] * v[2];
}

Vector3 cross(const Vector3& u, const Vector3& v) {
    return Vector3{
        u[1] * v[2] - u[2] * v[1],
        -(u[0] * v[2] - u[2] * v[0]),
        u[0] * v[1] - u[1] * v[0]
    };
}
Quaternion inverse_quaternion(const Quaternion& q) {
	return Quaternion{q[0], -1.0 * q[0], -1.0 * q[1], -1.0 * q[2]};
}

// https://gamedev.stackexchange.com/questions/28395/rotating-vector3-by-a-quaternion
void rotate_vector_by_quaternion(const Vector3& v, const Quaternion& q, Vector3& res) {
    Vector3 u{-1.0 * q[1], -1.0 * q[2], -1.0 * q[3]};

    auto dot_u_v = dot(u, v);
    auto dot_u_u = dot(u, u);
    auto cross_u_v = cross(u, v);

    for(size_t idx = 0; idx < 3; idx++) {
        res[idx] = 2.0f * dot_u_v * u[idx] + (q[0]*q[0] - dot_u_u) * v[idx] + 2.0f * q[0] * cross_u_v[idx];
    }
}
