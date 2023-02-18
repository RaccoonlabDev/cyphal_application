cmake_minimum_required(VERSION 3.15.3)

project(cyphal_example)

FILE(GLOB libcyphalSources                  ../../libcyphal/*.cpp)
FILE(GLOB libcanardSources                  ../../Libs/libcanard/libcanard/*.c)
FILE(GLOB libparamsSources                  ../../Libs/libparams/libparams/*.c)
FILE(GLOB o1heapSources                     ../../Libs/o1heap/o1heap/*.c)

if(PLATFORM STREQUAL "bxcan")
	message(SEND_ERROR "bxcan support coming soon!")
elseif(PLATFORM STREQUAL "fdcan")
	message(SEND_ERROR "fdcan support coming soon!")
elseif(PLATFORM STREQUAL "socketcan")
    FILE(GLOB libcyphalPlatformSpecificSources  ../../platform_specific/socketcan/*.cpp)
    FILE(GLOB libparamsPlatformSpecificSources  ../../Libs/libparams/platform_specific/ubuntu/*.cpp)
    FILE(GLOB transportPlatformSpecificSources  ../../Libs/platform_specific_components/socketcan/libcanard/src/*.c)
    include_directories(../../platform_specific/socketcan)
    include_directories(../../Libs/libparams/platform_specific/ubuntu)
    include_directories(../../Libs/platform_specific_components/socketcan/libcanard/src)
else()
	message(SEND_ERROR "Platform is not specified!")
endif()

set(SRC
    main.cpp
    params.cpp
    ${libcyphalSources}
    ${libcanardSources}
    ${libparamsSources}
    ${o1heapSources}
    ${libcyphalPlatformSpecificSources}
    ${libparamsPlatformSpecificSources}
    ${transportPlatformSpecificSources}
)

include_directories(.)
include_directories(../../libcyphal)
include_directories(../../Libs/libcanard/libcanard)
include_directories(../../Libs/libparams/libparams)
include_directories(../../Libs/o1heap/o1heap)
include_directories(../../build/nunavut_out)
