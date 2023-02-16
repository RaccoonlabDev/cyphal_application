# Copyright (c) 2022 Dmitry Ponomarev
# Distributed under the MIT License, available in the file LICENSE.
# Author: Dmitry Ponomarev <ponomarevda96@gmail.com>
FDCAN_IMPLEMENTATION_DIR := $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))
BXCAN_LIB_DIR := $(FDCAN_IMPLEMENTATION_DIR)/../../Libs/platform_specific_components/stm32

CPP_SOURCES += $(wildcard ${FDCAN_IMPLEMENTATION_DIR}/*.cpp)
