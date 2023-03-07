/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#ifndef AP_JSON_
#define AP_JSON_

#include <stdint.h>
#include <cstddef>
#include <arpa/inet.h>
#include <array>
#include <vector>
#include <string>

struct servo_packet {
    uint16_t magic; // 18458 expected magic value
    uint16_t frame_rate;
    uint32_t frame_count;
    uint16_t pwm[16];
};

class ArdupilotJson{
public:
    ArdupilotJson();
    bool init();
    bool send_servo(const std::array<uint16_t, 16>& servo_pwm);
    bool receive_sensors();

    double timestamp;
    std::array<double, 3> gyro = {0, 0, 0};
    std::array<double, 3> accel = {0, 0, 0};
    std::array<double, 3> position = {0, 0, 0};
    std::array<double, 4> quaternion = {1, 0, 0, 0};
    std::array<double, 3> velocity = {0, 0, 0};
    std::array<double, 3> mag = {0, 0, 0};

private:
    bool parse_json(const char* buffer, int size);

    bool parse_json_list(const std::string& str,
                         size_t first_idx,
                         size_t last_idx,
                         std::vector<double>& numbers) const;
    int _client_fd;
    servo_packet _servo_pkt;
    struct sockaddr_in _server_addr;
};

typedef std::array<double, 3> Vector3;
typedef std::array<double, 4> Quaternion;
void rotate_vector_by_quaternion(const Vector3& v, const Quaternion& q, Vector3& res);

#endif  // AP_JSON_
