#ifndef RUNNER_HPP_
#define RUNNER_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <iomanip> // std::setw
#include <chrono>
#include <cmath> // std::sqrt
#include <numeric> // std::accumulate
#include <cassert>
#include <stdexcept> // std::runtime_error
#include <sstream> // std::ostringstream

#include "Input.hpp"
#include "Output.hpp"
#include "Timer.hpp"

template <class Input, class Algo>
class Runner {
 public:
  using Duration = Timer::Duration;
  using Durations = std::vector<Duration>;

#if !defined(TOTAL_ITERATIONS)
# define TOTAL_ITERATIONS 30000000
#endif

#if defined(NDEBUG)
  static const int TIMED_RUN = 10;
#else
  static const int TIMED_RUN = 1;
#endif

  Runner(const Input& input, Output& output, const char* name):
      input_(input),
      output_(output),
#if defined(NDEBUG)
      timer_iterations_(TOTAL_ITERATIONS / static_cast<int>(input.size()) / TIMED_RUN),
#else
      timer_iterations_(1),
#endif
      name_(name)
  {
    assert(timer_iterations_ > 0);
#if !defined(NDEBUG)
    test_algo();
#endif
  }

  void run() {
    if (!Algo::enabled) {
      return;
    }
#if !defined(NDEBUG)
    output_.clear();
#endif
    for (int i = 0; i < TIMED_RUN; ++i) {
      const Timer::TimePoint start = Timer::now();

      for (int j = 0; j < timer_iterations_; ++j) {
        Algo::run(output_.buffer(), input_.values());

        // prevent compiler optimization
        output_.use_buffer(name_);
      }

      const Timer::TimePoint stop = Timer::now();
      durations_.push_back(stop - start);
    }
  }

  Duration average() const {
    return average(durations_);
  }

  const Durations& durations() const {
    return durations_;
  }

  static const Durations& pick_best_durations(
      const Durations& x1, const Durations& x2
  ) {
    if (average(x1) < average(x2)) {
      return x1;
    }
    else {
      return x2;
    }
  }

  void output_result(Duration best) const {
    output_result(best, name_, durations_, timer_iterations_, input_);
  }

  int timer_iterations() const {
    return timer_iterations_;
  }

  static void output_result(
      Duration best,
      std::string name,
      const Durations& durations,
      int timer_iterations,
      const Input& input
  ) {
    if (!Algo::enabled) {
      return;
    }

    const Duration avg = average(durations);

    std::string output_name;
    if (avg == best) {
      output_name += "(!)";
    }
    else {
      assert(best < avg);
      assert(best.count() > 0);
      auto overhead = 100 * (avg - best).count() / best.count();
      output_name += "(+" + std::to_string(overhead) + "%)";
    }

    output_name += " ";
    output_name += name;

    double avg_tick = static_cast<double>(avg.count());
    double sd(0);
    for (auto i: durations) {
      double x(i.count() - avg_tick);
      sd += x * x;
    }
    sd = sd / durations.size();
    sd = std::sqrt(sd);
    const double var = (sd / avg_tick) * 100.0;

    uint64_t input_size = input.values().size();
    uint64_t conv_ops = durations.size() * timer_iterations * input_size;
    using Ms = std::chrono::milliseconds;
    std::cout << std::setw(40) << output_name << " [";
    std::cout << "avg:" << avg.count() << " ";
    std::cout << "var:" << var << "% ";
    std::cout << "ms:" << std::chrono::duration_cast<Ms>(avg).count() << " ";
    std::cout << "runs:" << durations.size() << " ";
    std::cout << "conv_ops:" << conv_ops;
    std::cout << "]" << std::endl;
  }

 private:
  using value_t = typename Input::value_t;

  static Duration average(const Durations& durations) {
    if (!Algo::enabled) {
      return Duration::max();
    }

    if (durations.empty()) {
      throw std::runtime_error("No run detected");
    }
    Duration sum = std::accumulate(
        durations.begin(), durations.end(), Duration(0)
    );
    Duration result = sum / durations.size();
    if (result.count() != 0) {
      return result;
    }

#if defined(NDEBUG)
    throw std::runtime_error("Too fast (:");
#else
    return Duration(1);
#endif
  }

  void test_algo() {
    test_algo_iteration(0);
    test_algo_iteration(-10);
    test_algo_iteration(10);
    test_algo_iteration(std::numeric_limits<value_t>::max());
    test_algo_iteration(std::numeric_limits<value_t>::min());
    test_algo_iteration(std::numeric_limits<value_t>::max() - 1);
    test_algo_iteration(std::numeric_limits<value_t>::min() + 1);
    test_algo_iteration(std::numeric_limits<value_t>::max() / 2);
    test_algo_iteration(std::numeric_limits<value_t>::min() / 2);
  }

  void test_algo_iteration(value_t input_value) {
    typename Input::Vector vector;
    vector.push_back(input_value);

    std::ostringstream result;
    result << input_value;

    std::vector<char> buffer;
    buffer.resize(result.str().size() * 2);
    Algo::run(buffer.data(), vector);

    if (std::string(buffer.data()) != result.str()) {
      std::cerr << "Convert " << input_value;
      std::cerr << " failed for " << name_ << std::endl;
      throw std::runtime_error("Incorrect algorithm");
    }
  }

  const Input& input_;
  Output& output_;

  const int timer_iterations_;
  const char* name_;
  Durations durations_;
};

#endif // RUNNER_HPP_
