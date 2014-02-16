# Copyright (c) 2014, Ruslan Baratov
# All rights reserved.

cmake_minimum_required(VERSION 2.8)
project(IntDecFormat)

option(USE_LIBCXX "Use libcxx library" OFF)
option(BUILD32 "Build 32 bit test" OFF)

if(NOT MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

if(USE_LIBCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

if(BUILD32)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
endif()

if($ENV{TRAVIS})
  add_definitions(-DTOTAL_ITERATIONS=10000000)
endif()

enable_testing()
add_subdirectory(source)
