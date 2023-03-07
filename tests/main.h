/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#ifndef UBUNTU_MAIN_H_
#define UBUNTU_MAIN_H_

#include <stdint.h>

uint32_t HAL_GetTick();
uint32_t HAL_GetUIDw0();
uint32_t HAL_GetUIDw1();
uint32_t HAL_GetUIDw2();
void HAL_NVIC_SystemReset();

#endif  // UBUNTU_MAIN_H_
