cmake_minimum_required(VERSION 3.15.3)
project(example)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(PLATFORM socketcan)

include(../../CMakeLists.txt)

include_directories(.)
include_directories(..)
include_directories(../../Udral)
add_executable(application

    ../../Udral/actuator.cpp
    ../../Udral/barometer.cpp
    ../../Udral/gnss.cpp
    ../../Udral/magnetometer.cpp
    ../../Udral/imu.cpp

    main.cpp
    ap_json.cpp
    hitl_application.cpp
    params.cpp
    ${CYPHAL_SRC}
)
