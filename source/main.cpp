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
    size_t output_size, int digit, bool have_sign, bool same_size
) {
  using In = Input<Type>;

#if !defined(NDEBUG)
  std::cout << "*** DEBUG BUILD ***" << std::endl;
  // Print config before assertions
  std::cout << get_name<Type>() << " digit:" << digit << " ";
  std::cout << "sign:" << have_sign << std::endl;
#endif

  if (digit != 0) {
    if (std::pow(10, digit) > std::numeric_limits<Type>::max()) {
      std::cout << "Skip: too much digits(" << digit << ") ";
      std::cout << "for current type(" << get_name<Type>() << ")" << std::endl;
      return;
    }
  }

  if (same_size) {
    if (have_sign) {
      std::cout << "Skip: negative/positive with same size" << std::endl;
      return;
    }
    if (digit == 0) {
        std::cout << "Skip: same size with any number of digits" << std::endl;
      return;
    }
  }

  In input(output_size, digit, have_sign, same_size);
  Output output(output_size, input);

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
  if (same_size) {
    std::cout << "(same size) ";
  }
  std::cout << "to buffer " << output.size() << " bytes" << std::endl;

  std::cout << "sizeof(short, int, long, long long, void*): ";
  std::cout << sizeof(short) << " ";
  std::cout << sizeof(int) << " ";
  std::cout << sizeof(long) << " ";
  std::cout << sizeof(long long) << " ";
  std::cout << sizeof(void*) << std::endl;

  Runner<In, AlgoFmtFormat> algo_fmt_format(input, output, "fmt::FormatInt");
  Runner<In, AlgoBoostKarma> algo_boost_karma(input, output, "boost::spirit::karma");
  Runner<In, AlgoAlexandrescu> algo_alexandrescu(input, output, "alexandrescu");
  Runner<In, AlgoReverse> algo_reverse(input, output, "reverse");
  Runner<In, AlgoTmpbuf> algo_tmpbuf(input, output, "tmpbuf");
  Runner<In, AlgoCounting> algo_counting(input, output, "counting");

  std::cout << "Run tests ";

  std::cout << "#1 " << std::flush;
  algo_fmt_format.run();
  algo_boost_karma.run();
  algo_alexandrescu.run();
  algo_reverse.run();
  algo_tmpbuf.run();
  algo_counting.run();

  std::cout << "#2 " << std::flush;
  algo_fmt_format.run();
  algo_boost_karma.run();
  algo_alexandrescu.run();
  algo_counting.run();
  algo_tmpbuf.run();
  algo_reverse.run();

  std::cout << "#3 " << std::flush;
  algo_alexandrescu.run();
  algo_fmt_format.run();
  algo_counting.run();
  algo_boost_karma.run();
  algo_reverse.run();
  algo_tmpbuf.run();

  std::cout << "#4 " << std::flush;
  algo_fmt_format.run();
  algo_alexandrescu.run();
  algo_boost_karma.run();
  algo_reverse.run();
  algo_counting.run();
  algo_tmpbuf.run();

  std::cout << "#5 " << std::flush;
  algo_counting.run();
  algo_reverse.run();
  algo_fmt_format.run();
  algo_boost_karma.run();
  algo_tmpbuf.run();
  algo_alexandrescu.run();

  std::cout << "Results: " << std::endl;
  Timer::Duration algo_fmt_format_avg = algo_fmt_format.average();
  Timer::Duration algo_boost_karma_avg = algo_boost_karma.average();
  Timer::Duration algo_alexandrescu_avg = algo_alexandrescu.average();
  Timer::Duration algo_reverse_avg = algo_reverse.average();
  Timer::Duration algo_tmpbuf_avg = algo_tmpbuf.average();
  Timer::Duration algo_counting_avg = algo_counting.average();

  Timer::Duration min = std::min(
      {
      algo_fmt_format_avg,
      algo_boost_karma_avg,
      algo_alexandrescu_avg,
      algo_reverse_avg,
      algo_tmpbuf_avg,
      algo_counting_avg
      }
  );

  // Output results
  algo_fmt_format.output_result(min);
  algo_boost_karma.output_result(min);
  algo_alexandrescu.output_result(min);
  algo_reverse.output_result(min);
  algo_tmpbuf.output_result(min);
  algo_counting.output_result(min);
}

int main() {
  try {
    std::vector<size_t> output_size_variants{30, 300, 4096};
    std::vector<int> digit_variants{0, 1, 2, 4, 10};
    std::vector<bool> sign_variants{true, false};
    std::vector<bool> same_size_variants{true, false};

#if defined(LONG_TEST) && defined(NDEBUG)
    output_size_variants.push_back(8 * 1024 * 1024);
#endif

    bool run_any = false;

    for (auto output_size: output_size_variants) {
      for (auto digit: digit_variants) {
        for (auto sign: sign_variants) {
          for (auto same_size: same_size_variants) {
            run_any = true;

            run_with_type<short>(output_size, digit, sign, same_size);
            run_with_type<int>(output_size, digit, sign, same_size);
            run_with_type<long>(output_size, digit, sign, same_size);
            run_with_type<long long>(output_size, digit, sign, same_size);
          }
        }
      }
    }

    if (!run_any) {
      throw std::runtime_error("Test list is empty");
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
