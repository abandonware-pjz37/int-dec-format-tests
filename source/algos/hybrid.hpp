#ifndef ALGOS_FORMAT_HYBRID_HPP_
#define ALGOS_FORMAT_HYBRID_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <cassert> // assert
#include <algorithm> // std::reverse

namespace hybrid {

template <class Type, class Enable = Type>
class PickStdType;

template <class T>
class PickStdType<
    T, typename std::enable_if<(sizeof(T) == sizeof(uint8_t)), T>::type
> {
 public:
  static_assert(std::is_unsigned<T>::value, "");
  using Type = uint8_t;
};

template <class T>
class PickStdType<
    T, typename std::enable_if<(sizeof(T) == sizeof(uint16_t)), T>::type
> {
 public:
  static_assert(std::is_unsigned<T>::value, "");
  using Type = uint16_t;
};

template <class T>
class PickStdType<
    T, typename std::enable_if<(sizeof(T) == sizeof(uint32_t)), T>::type
> {
 public:
  static_assert(std::is_unsigned<T>::value, "");
  using Type = uint32_t;
};

template <class T>
class PickStdType<
    T, typename std::enable_if<(sizeof(T) > sizeof(uint32_t)), T>::type
> {
 public:
  static_assert(std::is_unsigned<T>::value, "");
  using Type = T;
};

template <class Integer>
inline int count_digits_impl(Integer value) {
  static_assert(sizeof(Integer) > sizeof(uint32_t), "");
  static_assert(sizeof(Integer) >= sizeof(unsigned long long), "");
  static_assert(std::is_unsigned<Integer>::value, "");

  const Integer p01 = 10ull;
  const Integer p02 = 100ull;
  const Integer p03 = 1000ull;
  const Integer p04 = 10000ull;
  const Integer p05 = 100000ull;
  const Integer p06 = 1000000ull;
  const Integer p07 = 10000000ull;
  const Integer p08 = 100000000ull;
  const Integer p09 = 1000000000ull;
  const Integer p10 = 10000000000ull;

  if (value >= p10) {
    return 10 + count_digits_impl(value / p10);
  }

  if (value < p05) {
    if (value < p03) {
      if (value < p02) {
        return 1 + (value >= p01);
      }
      return 3;
    }
    return 4 + (value >= p04);
  }

  if (value < p08) {
    if (value < p07) {
      return 6 + (value >= p06);
    }
    return 8;
  }

  return 9 + (value >= p09);
}

inline int count_digits_impl(uint8_t value) {
  const uint8_t p01 = 10;
  const uint8_t p02 = 100;

  if (value < p01) {
    return 1;
  }

  return 2 + (value >= p02);
}

inline int count_digits_impl(uint16_t value) {
  const uint16_t p01 = 10;
  const uint16_t p02 = 100;
  const uint16_t p03 = 1000;
  const uint16_t p04 = 10000;

  if (value < p03) {
    if (value < p02) {
      return 1 + (value >= p01);
    }
    return 3;
  }

  return 4 + (value >= p04);
}

inline int count_digits_impl(uint32_t value) {
  const uint32_t p01 = 10;
  const uint32_t p02 = 100;
  const uint32_t p03 = 1000;
  const uint32_t p04 = 10000;
  const uint32_t p05 = 100000;
  const uint32_t p06 = 1000000;
  const uint32_t p07 = 10000000;
  const uint32_t p08 = 100000000;
  const uint32_t p09 = 1000000000;

  if (value < p05) {
    if (value < p03) {
      if (value < p02) {
        return 1 + (value >= p01);
      }
      return 3;
    }
    return 4 + (value >= p04);
  }

  if (value < p08) {
    if (value < p07) {
      return 6 + (value >= p06);
    }
    return 8;
  }

  return 9 + (value >= p09);
}

template <class T>
inline int count_digits(T v) {
  static_assert(std::is_unsigned<T>::value, "");
  using Type = typename PickStdType<T>::Type;
  Type value(v);
  return count_digits_impl(value);
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

using Iterator = char*;

template <class Integer>
inline void generate_with_counting(Iterator& sink, Integer input_value) {
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
    int index = value * 2; // 20..198

    --it;
    *it = cache[index + 1];

    --it;
    *it = cache[index];
  }
}

template <class Integer>
static void generate_with_reverse(Iterator& sink, Integer input_value) {
  const bool is_negative = (input_value < 0);
  if (is_negative) {
    *sink = '-';
    ++sink;
  }

  using Unsigned = typename std::make_unsigned<Integer>::type;
  Unsigned value = is_negative ? -input_value : input_value;

  Iterator reverse_start = sink;
  const char* cache = cache_digits();

  assert(value >= 0);
  while (value >= 100) {
    int index = (value % 100) * 2; // 0..198
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
    int index = value * 2; // 20..198

    // reverse order
    *sink = cache[index + 1];
    ++sink;

    *sink = cache[index];
    ++sink;
  }

  std::reverse(reverse_start, sink);
}

template <bool use_digits_counter, class Integer>
inline void generate(Iterator& sink, Integer input_value) {
  if (use_digits_counter) {
    return generate_with_counting(sink, input_value);
  }
  else {
    return generate_with_reverse(sink, input_value);
  }
}

} // namespace hybrid

#endif // ALGOS_FORMAT_HYBRID_HPP_
