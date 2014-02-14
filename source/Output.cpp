// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <stdexcept> // std::runtime_error

#include "Output.ipp"

void Output::use_buffer(const char* name) {
#if !defined(NDEBUG)
  std::string temp(buffer());
  if (temp != check_result_) {
    std::cerr << "expected: " << check_result_ << std::endl;
    std::cerr << "got: " << temp << std::endl;
    std::cerr << "algo: " << name << std::endl;
    throw std::runtime_error("check failed");
  }
#endif
}
