/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#include <iostream>
#include "cyphal.hpp"
#include <chrono>
#include <thread>

uint32_t HAL_GetTick() {
    static auto time_start = std::chrono::steady_clock::now();
    auto time_now = std::chrono::steady_clock::now();
    auto elapsed_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(time_now - time_start).count();
    return elapsed_time_ms;
}

int main (int argc, char *argv[]) {
    Cyphal cyphal;
    int init_res = cyphal.init();
    if (init_res < 0) {
        std::cout << "Error: " << init_res << std::endl;
        return -1;
    }

    std::cout << "Hello, world." << std::endl;
    while(true) {
        cyphal.process();
    }
}
