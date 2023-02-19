# Copyright (c) 2022 Dmitry Ponomarev
# Distributed under the MIT License, available in the file LICENSE.
# Author: Dmitry Ponomarev <ponomarevda96@gmail.com>
CYPHAL_EXAMPLE_ABS_DIR := $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))

ifndef CYPHAL_PLATFORM
$(error CYPHAL_PLATFORM must be specified: $(notdir $(wildcard $(CYPHAL_EXAMPLE_ABS_DIR)/platform_specific/*)))
endif
ifndef LIBPARAMS_PLATFORM
$(error LIBPARAMS_PLATFORM must be specified: $(notdir $(wildcard $(CYPHAL_EXAMPLE_ABS_DIR)/Libs/libparams/platform_specific/*)))
endif

# This library source code
INC += -I${CYPHAL_EXAMPLE_ABS_DIR}/Cyphal
CPP_SOURCES += $(wildcard ${CYPHAL_EXAMPLE_ABS_DIR}/Cyphal/*.cpp)

# o1heap
C_SOURCES += ${CYPHAL_EXAMPLE_ABS_DIR}/Libs/o1heap/o1heap/o1heap.c
INC += -I${CYPHAL_EXAMPLE_ABS_DIR}/Libs/o1heap/o1heap

# libcanard
C_SOURCES += ${CYPHAL_EXAMPLE_ABS_DIR}/Libs/libcanard/libcanard/canard.c
INC += -I${CYPHAL_EXAMPLE_ABS_DIR}/Libs/libcanard/libcanard

# libparams
LIBPARAMS_PATH=${CYPHAL_EXAMPLE_ABS_DIR}/Libs/libparams
INC += -I${LIBPARAMS_PATH}/libparams/ \
			  -I${LIBPARAMS_PATH}/platform_specific/${LIBPARAMS_PLATFORM}/
C_SOURCES += $(wildcard ${LIBPARAMS_PATH}/libparams/*.c) \
             $(wildcard ${LIBPARAMS_PATH}/platform_specific/${LIBPARAMS_PLATFORM}/*.c)
CPP_SOURCES += $(wildcard ${LIBPARAMS_PATH}/platform_specific/${LIBPARAMS_PLATFORM}/*.cpp)

# Platform specific implementation
include ${CYPHAL_EXAMPLE_ABS_DIR}/platform_specific/${CYPHAL_PLATFORM}/Makefile
