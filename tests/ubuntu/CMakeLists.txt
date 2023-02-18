cmake_minimum_required(VERSION 3.15.3)
project(example)

set(PLATFORM socketcan)

include(../../CMakeLists.txt)

add_executable(application ${SRC})
