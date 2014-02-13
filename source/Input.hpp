#ifndef INPUT_HPP_
#define INPUT_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <limits> // std::numeric_limits
#include <random> // std::random_device

template <class Type>
class Input {
 public:
  static_assert(std::numeric_limits<Type>::is_integer, "");
  using Vector = std::vector<Type>;

  static const int digits_per_value = std::numeric_limits<Type>::digits10;
  static_assert(digits_per_value != 0, "");
  static const size_t bytes_per_value = digits_per_value +
      1 + // rounding error
      1 + // sign
      1; // terminate

  const Vector& values() const {
    return values_;
  }

  size_t size() const {
    return values().size();
  }

  Input(size_t output_size, int digit, bool have_sign) {
    values_.resize(output_size / bytes_per_value);
    assert(!values_.empty());

    Type min = 0;
    Type max = 0;
    switch(digit) {
      case 1:
        min = -9;
        max = 9;
        break;
      case 2:
        min = -99;
        max = 99;
        break;
      case 4:
        min = -999;
        max = 999;
        break;
      case 10:
        min = -999999999;
        max = 999999999;
      case 0:
        min = std::numeric_limits<Type>::min();
        max = std::numeric_limits<Type>::max();
        break;
      default:
        assert(false);
        break;
    }

    if (!have_sign) {
      min = 0;
    }

    std::random_device rd;
    std::uniform_int_distribution<Type> dist(min, max);
    for (auto& i: values_) {
      i = dist(rd);
    }

#if !defined(NDEBUG)
    std::cout << "Input: ";
    for (auto& i: values_) {
      std::cout << i << " ";
    }
    std::cout << std::endl;
#endif
  }

 private:
  Vector values_;
};

#endif // INPUT_HPP_
