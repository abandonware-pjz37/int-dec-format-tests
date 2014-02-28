# Copyright (c) 2014, Ruslan Baratov
# All rights reserved.

cmake_minimum_required(VERSION 2.8)
project(IntDecFormat)

option(USE_LIBCXX "Use libcxx library" OFF)
option(BUILD32 "Build 32 bit test" OFF)
option(FAST_TEST "Reduce number of iterations" OFF)

if(NOT MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")
endif()

if(USE_LIBCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

if(BUILD32)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
endif()

if(FAST_TEST)
  add_definitions(-DTOTAL_ITERATIONS=5000000)
endif()

if(LONG_TEST)
  add_definitions(-DLONG_TEST=1)
  add_definitions(-DTOTAL_ITERATIONS=100000000)
endif()

enable_testing()
add_subdirectory(source)
