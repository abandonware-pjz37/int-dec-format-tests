#ifndef OUTPUT_IPP_
#define OUTPUT_IPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include "Output.hpp"

#include <iostream> // std::cout
#include <cassert>
#include <sstream> // std::ostringstream

template <class Input>
Output::Output(size_t size, const Input& input) {
  assert(size > 0);
  buffer_.resize(size);
#if !defined(NDEBUG)
  std::ostringstream stream;
  for (auto i: input.values()) {
    stream << i;
  }
  check_result_ = stream.str();
  assert(!check_result_.empty());
  assert(check_result_.size() < buffer_.size());

  std::cout << "Expected result: " << check_result_ << std::endl;
#endif
}

#endif // OUTPUT_IPP_
