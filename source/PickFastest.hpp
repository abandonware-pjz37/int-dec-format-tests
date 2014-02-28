#ifndef PICK_FASTEST_HPP_
#define PICK_FASTEST_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

// Get fastest available integer type by given type size

template <class T, class Enable = T>
class PickFastest;

template <class T>
class PickFastest<
    T, typename std::enable_if<(sizeof(T) == sizeof(uint8_t)), T>::type
> {
 public:
  static const bool is_sign = std::is_signed<T>::value;
  using Type = typename std::conditional<
      is_sign, int_fast8_t, uint_fast8_t
  >::type;
};

template <class T>
class PickFastest<
    T, typename std::enable_if<(sizeof(T) == sizeof(uint16_t)), T>::type
> {
 public:
  static const bool is_sign = std::is_signed<T>::value;
  using Type = typename std::conditional<
      is_sign, int_fast16_t, uint_fast16_t
  >::type;
};

template <class T>
class PickFastest<
    T, typename std::enable_if<(sizeof(T) == sizeof(uint32_t)), T>::type
> {
 public:
  static const bool is_sign = std::is_signed<T>::value;
  using Type = typename std::conditional<
      is_sign, int_fast32_t, uint_fast32_t
  >::type;
};

template <class T>
class PickFastest<
    T, typename std::enable_if<(sizeof(T) == sizeof(uint64_t)), T>::type
> {
 public:
  static const bool is_sign = std::is_signed<T>::value;
  using Type = typename std::conditional<
      is_sign, int_fast64_t, uint_fast64_t
  >::type;
};

#endif // PICK_FASTEST_HPP_
