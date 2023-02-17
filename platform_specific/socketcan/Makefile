# Copyright (c) 2022 Dmitry Ponomarev
# Distributed under the MIT License, available in the file LICENSE.
# Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

SOCKETCAN_IMPLEMENTATION_DIR := $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))
PLATFORM_SPECIFIC_DIR := $(patsubst %/,%,$(dir $(SOCKETCAN_IMPLEMENTATION_DIR)))
CYPHAL_DIR := $(patsubst %/,%,$(dir $(PLATFORM_SPECIFIC_DIR)))
SOCKETCAN_LIB_DIR := $(CYPHAL_DIR)/Libs/platform_specific_components/socketcan/libcanard/src

CPP_SOURCES += $(wildcard ${SOCKETCAN_IMPLEMENTATION_DIR}/*.cpp)
C_SOURCES += ${SOCKETCAN_LIB_DIR}/socketcan.c
INC += -I${SOCKETCAN_LIB_DIR}
