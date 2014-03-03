#ifndef ALGOS_COUNTING_HPP_
#define ALGOS_COUNTING_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <cassert> // assert
#include <type_traits> // std::make_unsigned
#include <limits> // std::numeric_limits

#include "CountDigits.hpp"

namespace counting {

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

// 1) count digits
// 2) fill sink "from the end"

// Iterator requirement: RandomAccessIterator
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

  static const int min = 1;
  static const int max = std::numeric_limits<Unsigned>::digits10;
  static const int checks_number = max - min + 1;

  // Note that `std::numeric_limits<...>::digits10` has a rounding error,
  // the real number of digits can be `digits10 + 1`.
  // Counting algorithm checks if `value >= 10^max_digits`, i.e.
  // can return `max_digits + 1`.
  size_t digits = CountDigits<checks_number>::template count<min, max>(value);

  sink += digits;
  sink_out = sink;

  assert(value >= 0); // sanity check

  const char* cache = cache_digits();

  assert(value >= 0);
  while (value >= 100) {
    size_t index = (value % 100) * 2; // 0..198
    value /= 100;

    --sink;
    *sink = cache[index + 1];

    --sink;
    *sink = cache[index];
  }

  if (value < 10) {
    // 0..9
    --sink;
    *sink = static_cast<char>('0' + value);
  }
  else {
    // 10..99
    size_t index = static_cast<size_t>(value * 2); // 20..198

    --sink;
    *sink = cache[index + 1];

    --sink;
    *sink = cache[index];
  }
}

} // namespace counting

#endif // ALGOS_COUNTING_HPP_
