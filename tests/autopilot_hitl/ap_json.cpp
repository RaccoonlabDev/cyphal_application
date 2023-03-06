/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#include "ap_json.hpp"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

ArdupilotJson::ArdupilotJson() {
    _servo_pkt.magic = 18458;
    _servo_pkt.frame_rate = 1000;
    _servo_pkt.frame_count = 0;
}

bool ArdupilotJson::init() {
    if ((_client_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        std::cout << "Socket creation error" << std::endl;
        return false;
    }
    _server_addr.sin_family = AF_INET;
    _server_addr.sin_port = htons(9002);
    if (inet_pton(AF_INET, "127.0.0.1", &_server_addr.sin_addr) <= 0) {
        std::cout << "Invalid address" << std::endl;
        return false;
    }

    int status;
    if ((status = connect(_client_fd, (struct sockaddr*)&_server_addr, sizeof(_server_addr))) < 0) {
        std::cout << "Connection Failed" << std::endl;
        return false;
    }

    return true;
}

bool ArdupilotJson::send_servo(const std::array<uint16_t, 16>& servo_pwm) {
    std::copy(std::begin(servo_pwm), std::end(servo_pwm), std::begin(_servo_pkt.pwm));
    send(_client_fd, &_servo_pkt, sizeof(_servo_pkt), 0);
    return true;
}

bool ArdupilotJson::receive_sensors() {
    char buffer[1024];
    int buffer_size = read(_client_fd, buffer, 1024);
    _servo_pkt.frame_count++;
    buffer[buffer_size] = 0;
    parse_json(buffer, buffer_size);
    return true;
}

// {"timestamp":1131.743,
// "imu":{"gyro":[1.138086090003379e-17,-9.26007201697338e-18,-1.7638184005081048e-18],
// "accel_body":[3.1950471578911405e-15,-0.0000033948200292251887,-9.800000002272988]},
// "position":[-5.123849296223462e-19,-6.754994481824757e-8,-0.19499986873364534],
// "quaternion":[1.0,1.5628551970229893e-18,-4.2277906432607347e-19,2.0098806949291254e-19],
// "velocity":[-3.2982527747048776e-18,-3.370127907671133e-13,-9.728853853536417e-7]}
bool ArdupilotJson::parse_json(const char* buffer, size_t size) {
    if (buffer == nullptr || size == 0) {
        return false;
    }

    std::string str(buffer);

    int gyro_first_char_idx = str.find("gyro");
    int accel_first_char_idx = str.find("accel_body", gyro_first_char_idx);
    int position_first_char_idx = str.find("position", accel_first_char_idx);
    int quaternion_first_char_idx = str.find("quaternion", position_first_char_idx);
    int velocity_first_char_idx = str.find("velocity", quaternion_first_char_idx);

    std::vector<double> vector_3d = {0, 0, 0};
    std::vector<double> vector_4d = {0, 0, 0, 0};

    parse_json_list(str, gyro_first_char_idx + 6,           accel_first_char_idx - 3,       vector_3d);
    std::copy_n(vector_3d.begin(), 3, gyro.begin());

    parse_json_list(str, accel_first_char_idx + 12,         position_first_char_idx - 4,    vector_3d);
    std::copy_n(vector_3d.begin(), 3, accel.begin());

    parse_json_list(str, position_first_char_idx + 10,      quaternion_first_char_idx - 3,  vector_3d);
    std::copy_n(vector_3d.begin(), 3, position.begin());

    parse_json_list(str, quaternion_first_char_idx + 12,    velocity_first_char_idx - 3,    vector_4d);
    std::copy_n(vector_4d.begin(), 4, quaternion.begin());

    parse_json_list(str, velocity_first_char_idx + 10,      size - 3,                       vector_3d);
    std::copy_n(vector_3d.begin(), 3, velocity.begin());

    return true;
}

// 3d example: "[-5.123849296223462e-19,-6.754994481824757e-8,-0.19499986873364534]"
// 4d example: "[1.0,1.5628551970229893e-18,-4.2277906432607347e-19,2.0098806949291254e-19]"
void ArdupilotJson::parse_json_list(const std::string& str,
                                    size_t first_idx,
                                    size_t last_idx,
                                    std::vector<double>& numbers) const {
    // std::cout << str.substr(first_idx, last_idx - first_idx + 1) << std::endl;
    if (str[first_idx] != '[' || str[last_idx] != ']' ) {
        std::cout << "skip" << std::endl;
        return;
    }

    first_idx++;

    const size_t VECTOR_SIZE = numbers.size();
    int comma_idx[VECTOR_SIZE - 1];
    std::string str_numbers[VECTOR_SIZE];

    comma_idx[0] = str.find(',', first_idx);
    str_numbers[0] = str.substr(first_idx, comma_idx[0] - first_idx);
    for (size_t crnt_idx = 1; crnt_idx < VECTOR_SIZE - 1; crnt_idx++) {
        size_t prev_idx = crnt_idx - 1;
        comma_idx[crnt_idx] = str.find(',', comma_idx[prev_idx] + 1);
        str_numbers[crnt_idx] = str.substr(comma_idx[prev_idx] + 1, comma_idx[crnt_idx] - comma_idx[prev_idx] - 1);
    }
    str_numbers[VECTOR_SIZE - 1] = str.substr(comma_idx[VECTOR_SIZE - 2] + 1, last_idx - comma_idx[VECTOR_SIZE - 2] - 1);

    for (size_t number_idx = 0; number_idx < VECTOR_SIZE; number_idx++) {
        numbers[number_idx] = std::stof(str_numbers[number_idx]);
    }
}
