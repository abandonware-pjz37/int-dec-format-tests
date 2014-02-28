#ifndef COUNT_DIGITS_HPP_
#define COUNT_DIGITS_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

// Count decimal digits of unsigned type
// Optimized for big numbers (for small numbers used non-counting algorithm)
// Tuned for well-known standard types: uint8_t, uint16_t, uint32_t, uint64_t
template <int max_digits>
class CountDigits;

// For uint8_t
template <>
class CountDigits<3> {
 public:
  template <class T>
  static int count(T value) {
    static_assert(std::is_unsigned<T>::value, "");
    assert(value <= 999ull);

    const T p01 = 10;
    const T p02 = 100;

    if (value >= p02) {
      return 3;
    }

    return 1 + (value >= p01);
  }
};

// For uint16_t
template <>
class CountDigits<5> {
 public:
  template <class T>
  static int count(T value) {
    static_assert(std::is_unsigned<T>::value, "");
    assert(value <= 99999ull);

    const T p03 = 1000;
    const T p04 = 10000;

    if (value >= p03) {
      return 4 + (value >= p04);
    }

    return CountDigits<3>::count(value);
  }
};

// For uint32_t
template <>
class CountDigits<10> {
 public:
  template <class T>
  static int count(T value) {
    static_assert(std::is_unsigned<T>::value, "");
    assert(value <= 9999999999ull);

    const T p05 = 100000;
    const T p06 = 1000000;
    const T p07 = 10000000;
    const T p08 = 100000000;
    const T p09 = 1000000000;

    if (value >= p05) {
      if (value >= p07) {
        if (value >= p09) {
          return 10;
        }
        return 8 + (value >= p08);
      }
      return 6 + (value >= p06);
    }

    return CountDigits<5>::count(value);
  }
};

// unlimited variant for uint64_t and bigger
template <int max_decimal_digits>
class CountDigits {
 public:
  static_assert(max_decimal_digits > 10, "");

  template <class T>
  static int count(T value) {
    static_assert(std::is_unsigned<T>::value, "");
    static_assert(sizeof(T) >= sizeof(unsigned long long), "");
    const T p10 = 10000000000ull;

    if (value >= p10) {
      static const int next = max_decimal_digits - 10;
      value = value / p10;

      if (next <= 3) {
        return 10 + CountDigits<3>::count(value);
      }
      if (next <= 5) {
        return 10 + CountDigits<5>::count(value);
      }
      if (next <= 10) {
        return 10 + CountDigits<10>::count(value);
      }
      return 10 + CountDigits<next>::count(value);
    }

    return CountDigits<10>::count(value);
  }
};

#endif // COUNT_DIGITS_HPP_
