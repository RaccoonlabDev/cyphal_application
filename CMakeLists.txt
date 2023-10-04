cmake_minimum_required(VERSION 3.10.2)
project(cyphal_example)

if(NOT libparamsPath)
    set(libparamsPath ${CMAKE_CURRENT_LIST_DIR}/Libs/libparams)
endif()

FILE(GLOB cyphalSources                         ${CMAKE_CURRENT_LIST_DIR}/Cyphal/*.cpp)
FILE(GLOB libcanardSources                      ${CMAKE_CURRENT_LIST_DIR}/Libs/libcanard/libcanard/*.c)
FILE(GLOB libparamsSources                      ${libparamsPath}/libparams/*.c)
FILE(GLOB o1heapSources                         ${CMAKE_CURRENT_LIST_DIR}/Libs/o1heap/o1heap/*.c)

if(PLATFORM STREQUAL "bxcan")
    FILE(GLOB cyphalPlatformSpecificSources     ${CMAKE_CURRENT_LIST_DIR}/platform_specific/bxcan/*.cpp)
    FILE(GLOB libparamsPlatformSpecificSources  ${libparamsPath}/platform_specific/stm32f103/*.c)
    FILE(GLOB transportPlatformSpecificSources  ${CMAKE_CURRENT_LIST_DIR}/Libs/platform_specific_components/stm32/libcanard/bxcan/src/*.c)
    include_directories(${CMAKE_CURRENT_LIST_DIR}/platform_specific/bxcan)
    include_directories(${libparamsPath}/platform_specific/stm32f103)
    include_directories(${CMAKE_CURRENT_LIST_DIR}/Libs/platform_specific_components/stm32/libcanard/bxcan/src)
elseif(PLATFORM STREQUAL "fdcan")
    FILE(GLOB cyphalPlatformSpecificSources     ${CMAKE_CURRENT_LIST_DIR}/platform_specific/fdcan/*.cpp)
    FILE(GLOB libparamsPlatformSpecificSources  ${libparamsPath}/platform_specific/stm32g0b1/*.c)
    include_directories(${CMAKE_CURRENT_LIST_DIR}/platform_specific/fdcan)
    include_directories(${libparamsPath}/platform_specific/stm32g0b1)
elseif(PLATFORM STREQUAL "socketcan")
    FILE(GLOB cyphalPlatformSpecificSources     ${CMAKE_CURRENT_LIST_DIR}/platform_specific/socketcan/*.cpp)
    FILE(GLOB libparamsPlatformSpecificSources  ${libparamsPath}/platform_specific/ubuntu/*.cpp)
    FILE(GLOB transportPlatformSpecificSources  ${CMAKE_CURRENT_LIST_DIR}/Libs/platform_specific_components/socketcan/libcanard/src/*.c)
    include_directories(${CMAKE_CURRENT_LIST_DIR}/platform_specific/socketcan)
    include_directories(${libparamsPath}/platform_specific/ubuntu)
    include_directories(${CMAKE_CURRENT_LIST_DIR}/Libs/platform_specific_components/socketcan/libcanard/src)
else()
	message(SEND_ERROR "Platform is not specified!")
endif()

set(CYPHAL_SRC
    ${cyphalSources}
    ${libcanardSources}
    ${libparamsSources}
    ${o1heapSources}
    ${cyphalPlatformSpecificSources}
    ${libparamsPlatformSpecificSources}
    ${transportPlatformSpecificSources}
)

include_directories(${CMAKE_CURRENT_LIST_DIR}/Cyphal)
include_directories(${CMAKE_CURRENT_LIST_DIR}/Libs/libcanard/libcanard)
include_directories(${libparamsPath}/libparams)
include_directories(${CMAKE_CURRENT_LIST_DIR}/Libs/o1heap/o1heap)
include_directories(${CMAKE_CURRENT_LIST_DIR}/build/nunavut_out)
