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

execute_process(
  COMMAND git rev-parse --short=16 HEAD
  COMMAND_ERROR_IS_FATAL ANY
  OUTPUT_VARIABLE GIT_HASH_SHORT
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
set(GIT_HASH "0x${GIT_HASH_SHORT}")
add_definitions(-DGIT_HASH=${GIT_HASH})
