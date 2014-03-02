#ifndef ALGOS_HPP_
#define ALGOS_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include "format.h"
#include <boost/spirit/include/karma.hpp>
#include "alexandrescu.hpp"
#include "reverse.hpp"
#include "tmpbuf.hpp"
#include "counting.hpp"

class AlgoFmtFormat {
 public:
  static const bool enabled = true;

  template <class Vector>
  static void run(char* buffer, const Vector& in) {
    for(auto& i: in) {
      fmt::FormatDec(buffer, i);
    }
    *buffer = '\0';
  }
};

class AlgoBoostKarma {
 public:
  static const bool enabled = true;

  template <class Vector>
  static void run(char* buffer, const Vector& in) {
    for(auto& i: in) {
      using namespace boost::spirit;
      karma::generate(buffer, int_, i);
    }
    *buffer = '\0';
  }
};

class AlgoAlexandrescu {
 public:
  static const bool enabled = true;

  template <class Vector>
  static void run(char* buffer, const Vector& in) {
    for(auto& i: in) {
      alexandrescu::generate(buffer, i);
    }
    *buffer = '\0';
  }
};

class AlgoCounting {
 public:
  static const bool enabled = true;

  template <class Vector>
  static void run(char* buffer, const Vector& in) {
    for (auto& i: in) {
      counting::generate(buffer, i);
    }
    *buffer = '\0';
  }
};

class AlgoReverse {
 public:
  static const bool enabled = true;

  template <class Vector>
  static void run(char* buffer, const Vector& in) {
    for (auto& i: in) {
      reverse::generate(buffer, i);
    }
    *buffer = '\0';
  }
};

class AlgoTmpbuf {
 public:
  static const bool enabled = true;

  template <class Vector>
  static void run(char* buffer, const Vector& in) {
    for (auto& i: in) {
      tmpbuf::generate(buffer, i);
    }
    *buffer = '\0';
  }
};

#endif // ALGOS_HPP_
