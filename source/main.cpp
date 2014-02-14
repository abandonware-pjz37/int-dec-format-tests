// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <cstdlib> // EXIT_SUCCESS
#include <iostream> // std::cerr

#include "Runner.hpp"
#include "Algos.hpp"
#include "Output.ipp"

template <class Type> const char* get_name();

template <>
const char* get_name<short>() {
  return "short";
}

template <>
const char* get_name<int>() {
  return "int";
}

template <>
const char* get_name<long>() {
  return "long";
}

template <>
const char* get_name<long long>() {
  return "long long";
}

template <class Type> void run_with_type(
    size_t output_size, int digit, bool have_sign
) {
  using In = Input<Type>;

  In input(output_size, digit, have_sign);
  Output output(output_size, input);

#if !defined(NDEBUG)
  std::cout << "*** DEBUG BUILD ***" << std::endl;
#endif

  std::cout << "Converting " << input.values().size() << " ";
  std::cout << get_name<Type>() << " with ";
  if (digit == 0) {
    std::cout << "ANY ";
  }
  else {
    std::cout << digit << " ";
  }
  std::cout << "base-10 digits ";
  if (!have_sign) {
    std::cout << "(no sign) ";
  }
  std::cout << "to buffer " << output.size() << " bytes" << std::endl;

  std::cout << "sizeof(short, int, long, long long): ";
  std::cout << sizeof(short) << " ";
  std::cout << sizeof(int) << " ";
  std::cout << sizeof(long) << " ";
  std::cout << sizeof(long long) << std::endl;

  Runner<In, AlgoFmtFormat> algo_fmt_format(input, output, "fmt::FormatInt");
  Runner<In, AlgoCppx> algo_cppx(input, output, "cppx::decimal_from");
  Runner<In, AlgoBoostKarma> algo_boost_karma(input, output, "boost::spirit::karma");

  std::cout << "Run tests ";

  std::cout << "#1 " << std::flush;
  algo_fmt_format.run();
  algo_cppx.run();
  algo_boost_karma.run();

  std::cout << "#2 " << std::flush;
  algo_fmt_format.run();
  algo_cppx.run();
  algo_boost_karma.run();

  std::cout << "#3 " << std::flush;
  algo_fmt_format.run();
  algo_cppx.run();
  algo_boost_karma.run();

  std::cout << "#4 " << std::flush;
  algo_fmt_format.run();
  algo_cppx.run();
  algo_boost_karma.run();

  std::cout << "#5 " << std::flush;
  algo_fmt_format.run();
  algo_cppx.run();
  algo_boost_karma.run();

  std::cout << "Results: " << std::endl;
  Timer::Duration algo_fmt_format_avg = algo_fmt_format.average();
  Timer::Duration algo_cppx_avg = algo_cppx.average();
  Timer::Duration algo_boost_karma_avg = algo_boost_karma.average();

  Timer::Duration min = std::min(
      {algo_fmt_format_avg,
      algo_cppx_avg,
      algo_boost_karma_avg}
  );

  // Output results
  algo_fmt_format.output_result(min);
  algo_cppx.output_result(min);
  algo_boost_karma.output_result(min);
}

int main() {
  try {
    std::vector<size_t> output_size_variants{30, 300, 4096};
    std::vector<int> digit_variants{0, 1, 2, 4, 10};
    std::vector<bool> sign_variants{true, false};

    for (auto output_size: output_size_variants) {
      for (auto digit: digit_variants) {
        for (auto sign: sign_variants) {
          // run_with_type<short>(output_size, digit, sign);
          run_with_type<int>(output_size, digit, sign);
          // run_with_type<long>(output_size, digit, sign);
          run_with_type<long long>(output_size, digit, sign);
        }
      }
    }
    return EXIT_SUCCESS;
  }
  catch (std::exception& exc) {
    std::cerr << exc.what() << std::endl;
    return EXIT_FAILURE;
  }
  catch (...) {
    std::cerr << "Unknown exception" << std::endl;
    return EXIT_FAILURE;
  }
}
