# Copyright (c) 2014, Ruslan Baratov
# All rights reserved.

include_directories(algos)
include_directories(../third_party/format)
include_directories("./")

find_package(Boost REQUIRED)
include_directories(SYSTEM ${Boost_INCLUDE_DIR})

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
    algos/alexandrescu.hpp
    algos/tmpbuf.hpp
    algos/counting.hpp
    algos/reverse.hpp
    Algos.hpp
    PickFastest.hpp
)

add_test(int_dec_format_test int_dec_format_test)
