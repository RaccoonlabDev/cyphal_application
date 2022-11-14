# Copyright (c) 2022 Dmitry Ponomarev
# Distributed under the MIT License, available in the file LICENSE.
# Author: Dmitry Ponomarev <ponomarevda96@gmail.com>
ifndef LIBCYPHAL_PATH
$(error LIBCYPHAL_PATH is not set)
endif
ifndef LIBCYPHAL_PLATFORM
$(error LIBCYPHAL_PLATFORM is not set)
endif
ifndef REPOSITORY_PATH
$(error REPOSITORY_PATH is not set)
endif

C_INCLUDES += -I${LIBCYPHAL_PATH}/libcyphal
CPP_SOURCES += $(wildcard ${LIBCYPHAL_PATH}/libcyphal/*.cpp)

include ${LIBCYPHAL_PATH}/platform_specific/${LIBCYPHAL_PLATFORM}/Makefile
