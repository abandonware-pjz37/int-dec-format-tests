# Copyright (c) 2014, Ruslan Baratov
# All rights reserved.

include_directories(algos)

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
    algos/format.h
    algos/format.cc
    Algos.hpp
)

add_test(int_dec_format_test int_dec_format_test)
