# Copyright (c) 2023 Dmitry Ponomarev
# Distributed under the MIT License, available in the file LICENSE.
# Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

UBUNTU_TESTS_ABS_DIR := $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))
TESTS_DIR := $(patsubst %/,%,$(dir $(UBUNTU_TESTS_ABS_DIR)))
REPOSITORY_DIR := $(patsubst %/,%,$(dir $(TESTS_DIR)))
BUILD_DIR=$(REPOSITORY_DIR)/build
BUILD_SRC_DIR=$(BUILD_DIR)/tests/ubuntu
NUNAVUT_BUILD_DIR=$(BUILD_DIR)/nunavut_out
BUILD_OBJ_DIR=$(BUILD_DIR)/obj
BUILD_EXECUTABLE_PATH=$(BUILD_DIR)/shell

INC =
C_DEFS =

# Config
CYPHAL_PLATFORM=socketcan
LIBPARAMS_PLATFORM=ubuntu
C_DEFS += -DFLASH_DRIVER_STORAGE_FILE=\"../../Boards/${SW_TARGET}/default_params.yml\"

define build_c_objects
	mkdir -p ${BUILD_OBJ_DIR}
	for src_file in $(1); do \
		gcc $(C_DEFS) $(CFLAGS) -c -fPIC $(INC) $$src_file -o ${BUILD_OBJ_DIR}/$${src_file##*/}.o; \
	done
endef

define build_cpp_objects
	mkdir -p ${BUILD_OBJ_DIR}
	for src_file in $(1); do \
		g++ $(C_DEFS) $(CPPFLAGS) -c -fPIC $(INC) $$src_file -o ${BUILD_OBJ_DIR}/$${src_file##*/}.o; \
	done
endef

# This library
include $(REPOSITORY_DIR)/Makefile

# Nunavut build output
INC += -I${NUNAVUT_BUILD_DIR} -I${BUILD_SRC_DIR}

# Tests
INC += -I${UBUNTU_TESTS_ABS_DIR}
INC += -I${TESTS_DIR}

# # user params
# CXX+=${REPOSITORY_DIR}/tests/params.c

#
CPP_SOURCES += params.cpp main.cpp

COMMON_LIBS=-lpthread -pthread
CPPFLAGS = -Wall -std=c++14

build: build_other_c_sources build_other_cpp_sources build_executable

all: build_other_c_sources build_other_cpp_sources build_executable execute

build_other_c_sources:
	$(call build_c_objects,$(C_SOURCES))

build_other_cpp_sources:
	$(call build_cpp_objects,$(CPP_SOURCES))

build_executable: build_other_c_sources build_other_cpp_sources
	g++ $(CPPFLAGS) $(COMMON_LIBS) -o $(BUILD_EXECUTABLE_PATH) $(wildcard ${BUILD_OBJ_DIR}/*.o)

execute: build_executable
	$(BUILD_EXECUTABLE_PATH)

clean:
	rm -rf ${BUILD_DIR}
