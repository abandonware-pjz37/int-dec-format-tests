#ifndef ALGOS_FORMAT_HYBRID_HPP_
#define ALGOS_FORMAT_HYBRID_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <cassert> // assert
#include <algorithm> // std::reverse

#include "CountDigits.hpp"
#include "PickFastest.hpp"

namespace hybrid {

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

// For big numbers it's better to count digits first,
// then fill sink "from the end"
template <int max_digits, class Integer>
inline void generate_big_numbers(Iterator& sink, Integer value) {
  static_assert(std::is_unsigned<Integer>::value, "");

  size_t digits = CountDigits<max_digits>::count(value);
  sink += digits;

  Iterator it = sink;
  const char* cache = cache_digits();

  assert(value >= 0);
  while (value >= 100) {
    size_t index = (value % 100) * 2; // 0..198
    value /= 100;

    --it;
    *it = cache[index + 1];

    --it;
    *it = cache[index];
  }

  if (value < 10) {
    // 0..9
    --it;
    *it = static_cast<char>('0' + value);
  }
  else {
    // 10..99
    size_t index = static_cast<size_t>(value * 2); // 20..198

    --it;
    *it = cache[index + 1];

    --it;
    *it = cache[index];
  }
}

// For small numbers it's better to fill buffer, then revert it in-situ
template <class Integer>
inline void generate_small_numbers(Iterator& sink, Integer value) {
  const char* cache = cache_digits();

  static_assert(std::is_unsigned<Integer>::value, "");
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
}

template <bool big_numbers, class Integer>
inline void generate(Iterator& sink, Integer input_value) {
  using Unsigned = typename std::make_unsigned<Integer>::type;
  Unsigned value(input_value);

  if (input_value < 0) {
    *sink = '-';
    ++sink;
    value = 0 - value;
  }

  assert(value >= 0); // sanity check

  if (big_numbers) {
    using Unsigned = typename std::make_unsigned<Integer>::type;
    static const int max_digits =
        std::numeric_limits<Unsigned>::digits10 +
        1; // rounding error
    return generate_big_numbers<max_digits>(sink, value);
  }

  return generate_small_numbers(sink, value);
}

} // namespace hybrid

#endif // ALGOS_FORMAT_HYBRID_HPP_
