/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#include <stdint.h>

/// Functions below are auxiliary.
/// You may provide their implementation outside if you need.
__attribute__ ((weak)) uint32_t HAL_GetUIDw0() { return 0; }
__attribute__ ((weak)) uint32_t HAL_GetUIDw1() { return 0; }
__attribute__ ((weak)) uint32_t HAL_GetUIDw2() { return 0; }
__attribute__ ((weak)) void HAL_NVIC_SystemReset() { }
