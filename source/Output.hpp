#ifndef OUTPUT_HPP_
#define OUTPUT_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <vector>
#include <string>

class Output {
 public:
  template <class Input>
  Output(size_t size, const Input& input);
  ~Output() = default;

  Output(const Output&) = delete;
  Output(Output&&) = delete;

  Output& operator=(const Output&) = delete;
  Output& operator=(Output&&) = delete;

  char* buffer() {
    return buffer_.data();
  }

  void clear() {
    for (auto& i: buffer_) {
      i = 0xfe;
    }
  }

  size_t size() const {
    return buffer_.size();
  }

  void use_buffer(const char* name);

 private:
#if !defined(NDEBUG)
  std::string check_result_;
#endif
  std::vector<char> buffer_;
};

#endif // OUTPUT_HPP_
