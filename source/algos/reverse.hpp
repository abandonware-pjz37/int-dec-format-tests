#ifndef ALGOS_REVERSE_HPP_
#define ALGOS_REVERSE_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <cassert> // assert
#include <algorithm> // std::reverse

namespace reverse {

inline const char* cache_digits() {
  return
      "00010203040506070809"
      "10111213141516171819"
      "20212223242526272829"
      "30313233343536373839"
      "40414243444546474849"
      "50515253545556575859"
      "60616263646566676869"
      "70717273747576777879"
      "80818283848586878889"
      "90919293949596979899"
  ;
}

using Iterator = char*;

// 1) fill buffer
// 2) revert in-situ
template <class Integer>
inline void generate(Iterator& sink_out, Integer input_value) {
  using Unsigned = typename std::make_unsigned<Integer>::type;
  Unsigned value(input_value);

  Iterator sink = sink_out;

  if (input_value < 0) {
    *sink = '-';
    ++sink;
    value = 0 - value;
  }

  assert(value >= 0); // sanity check
  const char* cache = cache_digits();

  Iterator reverse_start = sink;

  while (value >= 100) {
    size_t index = (value % 100) * 2; // 0..198
    value /= 100;

    // reverse order
    *sink = cache[index + 1];
    ++sink;

    *sink = cache[index];
    ++sink;
  }

  if (value < 10) {
    // 0..9
    *sink = static_cast<char>('0' + value);
    ++sink;
  }
  else {
    // 10..99
    size_t index = static_cast<size_t>(value * 2); // 20..198

    // reverse order
    *sink = cache[index + 1];
    ++sink;

    *sink = cache[index];
    ++sink;
  }

  std::reverse(reverse_start, sink);

  sink_out = sink;
}

} // namespace reverse

#endif // ALGOS_REVERSE_HPP_
