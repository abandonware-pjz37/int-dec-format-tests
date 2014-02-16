# Copyright (c) 2014, Ruslan Baratov
# All rights reserved.

include_directories(algos)
include_directories(../third_party/format)

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

add_executable(
    int_dec_format_test
    Input.hpp
    Output.hpp
    Output.cpp
    Runner.hpp
    Timer.hpp
    main.cpp
    ../third_party/format/format.h
    ../third_party/format/format.cc
    algos/cppx.hpp
    Algos.hpp
)

add_test(int_dec_format_test int_dec_format_test)
