# Copyright (c) 2022 Dmitry Ponomarev
# Distributed under the MIT License, available in the file LICENSE.
# Author: Dmitry Ponomarev <ponomarevda96@gmail.com>
ifndef LIBCYPHAL_PATH
$(error LIBCYPHAL_PATH is not set)
endif
ifndef REPOSITORY_PATH
$(error REPOSITORY_PATH is not set)
endif

C_INCLUDES += -I${LIBCYPHAL_PATH}/platform_specific/bxcan
CPP_SOURCES += $(wildcard ${LIBCYPHAL_PATH}/platform_specific/bxcan/*.cpp)

BXCAN_DIR=${REPOSITORY_PATH}/Libs/platform_specific_components/stm32
C_SOURCES += ${BXCAN_DIR}/libcanard/bxcan/src/bxcan.c
C_INCLUDES += -I${BXCAN_DIR}/libcanard/bxcan/src
