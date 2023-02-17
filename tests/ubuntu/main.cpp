/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#include <iostream>
#include "cyphal.hpp"

uint32_t HAL_GetTick() {return 0;}
uint32_t HAL_GetUIDw0() {return 0;}
uint32_t HAL_GetUIDw1() {return 0;}
uint32_t HAL_GetUIDw2() {return 0;}
void HAL_NVIC_SystemReset() {}

int main (int argc, char *argv[]) {
    Cyphal cyphal;
    cyphal.init();
    cyphal.process();
    std::cout << "hello, world" << std::endl;
}
