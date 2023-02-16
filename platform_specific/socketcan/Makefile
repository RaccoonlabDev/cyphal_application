# Copyright (c) 2022 Dmitry Ponomarev
# Distributed under the MIT License, available in the file LICENSE.
# Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

FDCAN_IMPLEMENTATION_DIR := $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))
BXCAN_LIB_DIR := $(FDCAN_IMPLEMENTATION_DIR)/../../Libs/platform_specific_components/socketcan/libcanard/src

CPP_SOURCES += $(wildcard ${FDCAN_IMPLEMENTATION_DIR}/*.cpp)
C_SOURCES += ${BXCAN_LIB_DIR}/socketcan.c
C_INCLUDES += -I${BXCAN_LIB_DIR}/
