# Coppied relevent parts from boost-experimental/sml repo
#
# Copyright (c) 2016-2019 Jean Davy
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
# Generate stage:
#   $mkdirBUILD; cd BUILD; cmake ..
#   Re-generate after changes:
#     $cmake .
#   Show each build command line as it is launched
#   $cmake -DCMAKE_VERBOSE_MAKEFILE=ON ..
#   Generate a release version: $cmake -DCMAKE_BUILD_TYPE=Release ..

# Build stage:
#   Linux: $cmake --build . -- -j
#   Windows: $cmake --build . -- /maxcpucount
#   Build only test/action_defer:
#   $cmake --build . --target actions_defer

# Test stage:
#    Launch all tests: $ctest --verbose
#    To run tests with memcheck: $ctest -T memcheck
#    List available test: $ctest -N
#    Launch benchmark tests: $ctest -R benchmark*

###########################################################################################
cmake_minimum_required(VERSION 3.5.1 FATAL_ERROR)
project(sml_example LANGUAGES CXX)
include(CTest)

# sml use hana library, according to hana documentation GCC >= 6.0.0
# http://www.boost.org/doc/libs/release/libs/hana/doc/html/index.html#tutorial-installation-requirements
# https://github.com/boostorg/hana/wiki/General-notes-on-compiler-support
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" AND "${CMAKE_CXX_COMPILER_VERSION}" VERSION_LESS 6.0.0)
  message(FATAL_ERROR "sml requires GCC >= 6.0.0")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" AND "${CMAKE_CXX_COMPILER_VERSION}" VERSION_LESS 3.5.0)
  message(FATAL_ERROR "sml requires Clang >= 3.5.0")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 19.0)
  message(FATAL_ERROR "sml requires Visual Studio 14 2015 at least")
endif()

# c++14 required
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag(-std=c++14 HAS_CXX14_FLAG)
check_cxx_compiler_flag(-std=c++17 HAS_CXX17_FLAG)
check_cxx_compiler_flag(-std=c++2a HAS_CXX20_FLAG)

if(HAS_CXX20_FLAG)
  set(CMAKE_CXX_STANDARD 20)
elseif(HAS_CXX17_FLAG)
  set(CMAKE_CXX_STANDARD 17)
elseif(HAS_CXX14_FLAG)
  set(CMAKE_CXX_STANDARD 14)
else()
  message(FATAL_ERROR "sml requires c++14")
endif()

if(NOT (DEFINED CMAKE_CXX_STANDARD) OR CMAKE_CXX_STANDARD STREQUAL "" OR CMAKE_CXX_STANDARD LESS 14)
    message(FATAL_ERROR "sml requires c++14")
endif()

set(IS_COMPILER_GCC_LIKE FALSE)
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang" OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" )

    set(IS_COMPILER_GCC_LIKE TRUE)
endif()

include(GNUInstallDirs)
install(DIRECTORY "include/boost" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

set(CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED)

add_library(sml INTERFACE)

include_directories($<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

# example applications
add_subdirectory(example)
