cmake_minimum_required(VERSION 3.15.3)
project(example)

set(PLATFORM socketcan)

include(../../CMakeLists.txt)

include_directories(.)
include_directories(..)
add_executable(application
    main.cpp
    application.cpp
    params.cpp
    ${CYPHAL_SRC}
)
