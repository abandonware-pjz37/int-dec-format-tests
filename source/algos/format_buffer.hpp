#ifndef ALGOS_FORMAT_BUFFER_HPP_
#define ALGOS_FORMAT_BUFFER_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <cassert> // assert

// 1) fill temporary buffer with cache indexes
// 2) fill sink with indexes in reversed order
namespace format_buffer {

using Iterator = char*;

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

template <class Integer>
inline void generate(Iterator& sink, Integer input_value) {
  const bool is_negative = (input_value < 0);
  if (is_negative) {
    *sink = '-';
    ++sink;
  }

  using Unsigned = typename std::make_unsigned<Integer>::type;
  Unsigned value = is_negative ? -input_value : input_value;

  enum {
    MAX_DIGITS = std::numeric_limits<Integer>::digits10 +
        1 // round error
  };
  int buffer[MAX_DIGITS];
  int* it = buffer;

  assert(value >= 0);
  while (value >= 100) {
    int index = (value % 100) * 2; // 0..198
    value /= 100;

    // reverse order
    *it = index + 1;
    ++it;

    *it = index;
    ++it;
  }

  if (value < 10) {
    // 0..9
    *it = static_cast<int>(value * 2 + 1);
    ++it;
  }
  else {
    // 10..99
    int index = static_cast<int>(value * 2); // 20..198

    // reverse order
    *it = index + 1;
    ++it;

    *it = index;
    ++it;
  }

  const char* cache = cache_digits();
  while (it > buffer) {
    --it;
    *sink = cache[*it];
    ++sink;
  }
}

} // namespace format_buffer

#endif // ALGOS_FORMAT_BUFFER_HPP_
