cmake_minimum_required(VERSION 3.15.3)
project(example)

set(PLATFORM socketcan)

include(../../CMakeLists.txt)

include_directories(.)
add_executable(application main.cpp params.cpp ${CYPHAL_SRC})
add_compile_definitions(PORT_LIST_PUBLISHER=1)
set_target_properties(
    application
    PROPERTIES
    COMPILE_FLAGS "-g -O0 --coverage"
    LINK_FLAGS "--coverage"
)