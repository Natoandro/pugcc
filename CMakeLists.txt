cmake_policy(SET CMP0048 NEW)
cmake_minimum_required(VERSION 3.0)

project(pugcc
  VERSION 1.0
  DESCRIPTION "C++ pug compiler"
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(libpugcc)

include_directories(${CURRENT_SOURCE_DIR}/libpugcc/include)

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  enable_testing()
  include(gtest.cmake)

  add_subdirectory(libpugcc_tests)
endif()

