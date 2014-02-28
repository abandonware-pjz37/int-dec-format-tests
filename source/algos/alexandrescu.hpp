#ifndef ALGOS_ALEXANDRESCU_HPP_
#define ALGOS_ALEXANDRESCU_HPP_

#include <cassert> // assert
#include <boost/config.hpp> // BOOST_CLANG

// https://www.facebook.com/notes/facebook-engineering/three-optimization-tips-for-c/10151361643253920

namespace alexandrescu {

using Iterator = char*;

template <class Integer>
inline int count_digits(Integer value) {
  static_assert(std::is_unsigned<Integer>::value, "");

  const uint64_t p01 = 10ull;
  const uint64_t p02 = 100ull;
  const uint64_t p03 = 1000ull;
  const uint64_t p04 = 10000ull;
  const uint64_t p05 = 100000ull;
  const uint64_t p06 = 1000000ull;
  const uint64_t p07 = 10000000ull;
  const uint64_t p08 = 100000000ull;
  const uint64_t p09 = 1000000000ull;
  const uint64_t p10 = 10000000000ull;
  const uint64_t p11 = 100000000000ull;
  const uint64_t p12 = 1000000000000ull;

  if (value < p01) {
    return 1;
  }

  if (value < p02) {
    return 2;
  }

  if (value < p03) {
    return 3;
  }

  if (value < p12) {
    if (value < p08) {
      if (value < p06) {
        if (value < p04) {
          return 4;
        }
        return 5 + (value >= p05);
      }
      return 7 + (value >= p07);
    }
    if (value < p10) {
      return 9 + (value >= p09);
    }
    return 11 + (value >= p11);
  }
  return 12 + count_digits(value / p12);
}

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

  int digits = count_digits(value);
  sink += digits;

  Iterator it = sink;
  const char* cache = cache_digits();

  assert(value >= 0);
  while (value >= 100) {
    int index = (value % 100) * 2; // 0..198
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
    int index = static_cast<int>(value * 2); // 20..198

    --it;
    *it = cache[index + 1];

    --it;
    *it = cache[index];
  }
}

} // namespace alexandrescu

#endif // ALGOS_ALEXANDRESCU_HPP_
