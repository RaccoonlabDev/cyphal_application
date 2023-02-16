# Copyright (c) 2022 Dmitry Ponomarev
# Distributed under the MIT License, available in the file LICENSE.
# Author: Dmitry Ponomarev <ponomarevda96@gmail.com>
BXCAN_IMPLEMENTATION_DIR := $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))
FDCAN_LIB_DIR := $(BXCAN_IMPLEMENTATION_DIR)/../../Libs/platform_specific_components/stm32

CPP_SOURCES += $(wildcard ${BXCAN_IMPLEMENTATION_DIR}/*.cpp)
C_SOURCES += ${FDCAN_LIB_DIR}/libcanard/bxcan/src/bxcan.c
C_INCLUDES += -I${FDCAN_LIB_DIR}/libcanard/bxcan/src
