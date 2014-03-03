#ifndef COUNT_DIGITS_HPP_
#define COUNT_DIGITS_HPP_

#include <cstddef> // size_t

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

template <int power>
class Power10;

template <>
class Power10<0> {
 public:
  static const unsigned long long value = 1;
};

template <int power>
class Power10 {
 public:
  static_assert(power > 0, "");

  static const unsigned long long value = 10ull * Power10<power - 1>::value;
};

// Count decimal digits of value without using division operation.
// Bisection algorithm used (maximum number of comparisons is 5 for uint64)

// max - maximum boundary value that need to be checked, i.e.
//   * value definitely lower than 10^(max+1)
//   * value can be bigger than 10^max (in this case number of digits `max + 1`)
// min - minimum boundary value that need to be checked, i.e.
//   * value definitely bigger than 10^(min-1)
//   * value can be lower than 10^min (in this case number of digits `min`)

// Numbers layout:
//  10^(min-1) ?? 10^min ?? 10^(min+1) ?? ... ?? 10^max ?? 10^(max+1) XX
//              ^                                        ^
//              |                                        |
//             `min` digits                             `max + 1` digits

template <int checks_number> // = max - min + 1
class CountDigits;

// Only one boundary need to be checked.
// Possible results: `min` or `min + 1`
// Numbers layout:
//   ?? 10^min = 10^max ??
//   ^                  ^
//   |                  |
//   min                min + 1
template <>
class CountDigits<1> {
 public:
  template <int min, int max, class T>
  static size_t count(T value) {
    static_assert(max == min, "");
    static_assert(min > 0, "");
    return min + (value >= Power10<min>::value);
  }
};

// Two boundaries need to be checked.
// Possible results: `min`, `max`, `max + 1`
// Numbers layout:
//  ?? 10^min ?? 10^max ??
//   ^                   ^
//   |                   |
//   min                 max + 1
template <>
class CountDigits<2> {
 public:
  template <int min, int max, class T>
  static size_t count(T value) {
    static_assert(min > 0, "");
    static_assert(max == min + 1, "");
    if (value >= Power10<max>::value) {
      // xx 10^min xx 10^max !!
      return max + 1;
    }
    // ?? 10^min ?? 10^max xx
    return CountDigits<1>::template count<min, min>(value);
  }
};

// For more than 2 boundaries - check median.
// Numbers layout:
//  ?? 10^min ?? 10^(min+1) ?? ... ?? 10^max ??
//   ^                                        ^
//   |                                        |
//   min                                      max + 1
template <int checks_number>
class CountDigits {
 public:
  template <int min, int max, class T>
  static size_t count(T value) {
    static const int median = Median<min, max>::value;

    if (value >= Power10<median>::value) {
      static const int n_max = max;
      static const int n_min = median + 1;
      static const int n_checks_number = n_max - n_min + 1;
      // xx 10^min xx ... xx 10^median ?? 10^n_min ?? ... ?? 10^max ??
      return CountDigits<n_checks_number>::template count<n_min, n_max>(value);
    }

    static const int n_max = median - 1;
    static const int n_min = min;
    static const int n_checks_number = n_max - n_min + 1;
    // ?? 10^min ?? ... ?? 10^n_max ?? 10^median xx ... xx 10^max xx
    return CountDigits<n_checks_number>::template count<n_min, n_max>(value);
  }

 private:
  template <int min, int max>
  struct Median {
   private:
    static_assert(min <= max, "");
    static_assert(0 < min, "");

    static const int diff = max - min;
    static const bool even = ((diff % 2) == 0);

    static_assert(diff >= 2, "");

    // 0, 1, 2, ... M
    // Prefer upper bound:
    //   M = 2N   -> N
    //   M = 2N+1 -> N + 1
    static const int half = diff / 2;
    static const int median = even ? half : half + 1;

   public:
    static const int value = min + median;
  };
};

#endif // COUNT_DIGITS_HPP_
