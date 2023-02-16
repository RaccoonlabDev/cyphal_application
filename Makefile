# Copyright (c) 2022 Dmitry Ponomarev
# Distributed under the MIT License, available in the file LICENSE.
# Author: Dmitry Ponomarev <ponomarevda96@gmail.com>
ifndef LIBCYPHAL_PLATFORM
$(error LIBCYPHAL_PLATFORM is not set)
endif

CYPHAL_EXAMPLE_ABS_DIR := $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))

# This library source code
C_INCLUDES += -I${CYPHAL_EXAMPLE_ABS_DIR}/libcyphal
CPP_SOURCES += $(wildcard ${CYPHAL_EXAMPLE_ABS_DIR}/libcyphal/*.cpp)

# o1heap
C_SOURCES += ${CYPHAL_EXAMPLE_ABS_DIR}/Libs/o1heap/o1heap/o1heap.c
C_INCLUDES += -I${CYPHAL_EXAMPLE_ABS_DIR}/Libs/o1heap/o1heap

# libcanard
C_SOURCES += ${CYPHAL_EXAMPLE_ABS_DIR}/Libs/libcanard/libcanard/canard.c
C_INCLUDES += -I${CYPHAL_EXAMPLE_ABS_DIR}/Libs/libcanard/libcanard

# libparams
LIBPARAMS_PATH=${REPOSITORY_PATH}/Libs/libparams
C_INCLUDES += -I${LIBPARAMS_PATH}/libparams/ \
			  -I${LIBPARAMS_PATH}/platform_specific/${LIBPARAMS_PLATFORM}/
C_SOURCES += $(wildcard ${LIBPARAMS_PATH}/libparams/*.c) \
             $(wildcard ${LIBPARAMS_PATH}/platform_specific/${LIBPARAMS_PLATFORM}/*.c)

# Platform specific implementation
include ${CYPHAL_EXAMPLE_ABS_DIR}/platform_specific/${LIBCYPHAL_PLATFORM}/Makefile
