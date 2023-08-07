cmake_minimum_required(VERSION 3.15.3)
project(example)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic -Werror")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(PLATFORM socketcan)

set(CYPHAL_REPO_DIR ../..)
include(${CYPHAL_REPO_DIR}/CMakeLists.txt)

include_directories(.)
include_directories(..)
include_directories(${CYPHAL_REPO_DIR}/Udral)

add_executable(application
    ${CYPHAL_REPO_DIR}/Udral/actuator.cpp
    ${CYPHAL_REPO_DIR}/Udral/barometer.cpp
    ${CYPHAL_REPO_DIR}/Udral/gnss.cpp
    ${CYPHAL_REPO_DIR}/Udral/magnetometer.cpp
    ${CYPHAL_REPO_DIR}/Udral/imu.cpp

    main.cpp
    math.cpp
    ap_json.cpp
    hitl_application.cpp
    params.cpp
    ${CYPHAL_SRC}
)
