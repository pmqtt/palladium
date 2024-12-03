//
// Created by Michael Burzan on 03.07.20.
//

#ifndef PURGE_FUZZER_HPP
#define PURGE_FUZZER_HPP

#include <functional>
#include <random>
#include <string>
#include <vector>

namespace purge {

template <class T, int MIN, int MAX> class scalar_fuzzer {
public:
  scalar_fuzzer() : gen(rd()), value_dist(MIN, MAX) {}

  std::random_device
      rd; // Will be used to obtain a seed for the random number engine
  std::mt19937 gen; // Standard mersenne_twister_engine seeded with rd()
  std::uniform_int_distribution<> value_dist;
};

template <class T> struct item_rule {
  item_rule(std::function<bool(T &&)> fun) : use_value_func(fun) {}
  std::function<bool(T &&)> use_value_func;
};

template <class CONTAINER> struct fuzzer_container {
  fuzzer_container(const item_rule<typename CONTAINER::value_type> &r)
      : rule(r), gen(rd()), value_dist(0, 255), size_dist(0, 65536) {
    generated.push_back({});
  }

  CONTAINER next() {
    CONTAINER value = generated[generated.size() - 1];
    generated.push_back(generate());
    return value;
  }

  CONTAINER generate() {
    int len = size_dist(gen);
    CONTAINER result{};
    for (int i = 0; i < len; ++i) {
      result.push_back(value_dist(gen));
    }
    return result;
  }

  item_rule<typename CONTAINER::value_type> rule;
  std::vector<CONTAINER> generated;
  std::random_device
      rd; // Will be used to obtain a seed for the random number engine
  std::mt19937 gen; // Standard mersenne_twister_engine seeded with rd()
  std::uniform_int_distribution<> value_dist;
  std::uniform_int_distribution<> size_dist;
};

} // namespace purge
#endif // PURGE_FUZZER_HPP
