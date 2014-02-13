#ifndef TIMER_HPP_
#define TIMER_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <chrono>

class Timer {
 public:
  using Clock = std::chrono::high_resolution_clock;
  using Duration = Clock::duration;
  using TimePoint = Clock::time_point;

  static TimePoint now() {
    return Clock::now();
  }
};

#endif // TIMER_HPP_
