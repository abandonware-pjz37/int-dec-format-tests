#ifndef ALGOS_TMPBUF_HPP_
#define ALGOS_TMPBUF_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <cassert> // assert
#include <limits> // std::numeric_limits
#include <array>

// 1) fill temporary buffer "from the end"
// 2) copy to destination
namespace tmpbuf {

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
inline void generate(Iterator& sink_out, Integer input_value) {
  using Unsigned = typename std::make_unsigned<Integer>::type;
  Unsigned value(input_value);

  Iterator sink = sink_out;

  if (input_value < 0) {
    *sink = '-';
    ++sink;
    value = 0 - value;
  }

  enum {
    MAX_DIGITS = std::numeric_limits<Integer>::digits10 +
        1 // round error
  };

  std::array<char, MAX_DIGITS> buffer;
  char* it = buffer.end();

  const char* cache = cache_digits();

  assert(value >= 0);
  while (value >= 100) {
    size_t index = (value % 100) * 2; // 0..198
    value /= 100;

    // reverse order
    --it;
    *it = cache[index + 1];

    --it;
    *it = cache[index];
  }

  if (value < 10) {
    // 0..9
    --it;
    *it = cache[static_cast<size_t>(value * 2 + 1)];
  }
  else {
    // 10..99
    size_t index = static_cast<size_t>(value * 2); // 20..198

    // reverse order
    --it;
    *it = cache[index + 1];

    --it;
    *it = cache[index];
  }

  // std::copy implemented using memcpy call
  // there is no visible befinits on such small memory chunks
  // (20 char maximum) sometimes it's even worse
  while (it != buffer.end()) {
    *sink = *it;
    ++sink;
    ++it;
  }

  sink_out = sink;
}

} // namespace tmpbuf

#endif // ALGOS_TMPBUF_HPP_
