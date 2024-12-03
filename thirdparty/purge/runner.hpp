//
// Created by Michael Burzan on 03.07.20.
//

#ifndef PURGE_RUNNER_HPP
#define PURGE_RUNNER_HPP
#include "fuzzer.hpp"
#include "log_book.hpp"
#include "test_registry.hpp"
#include <iostream>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
namespace purge {
class runner {
public:
  runner(int argc, char **argv) {}

  void run() {
    for (auto &[key, test_item] : purge::test_registry::test_storage) {
      // std::cout<<"Execute test:" <<key<<std::endl;
      log_book_entry entry;
      entry.test_name = key;
      entry.line = std::to_string(test_item.first);
      log_book::get_instance()->add_entry(entry);
      test_item.second();
    }
  }
};

template <class CONTAINER>
void simple_fuzz_runner(const item_rule<typename CONTAINER::value_type> &rule,
                        std::function<void(const CONTAINER &)> test_func) {
  purge::fuzzer_container<CONTAINER> fuzzer(rule);
  for (int i = 0; i < 1000; i++) {
    auto value = fuzzer.next();
    test_func(value);
  }
}

template <class CONTAINER>
void fuzz_runner(const item_rule<typename CONTAINER::value_type> &rule,
                 std::function<void(const CONTAINER &)> test_func) {
  purge::fuzzer_container<CONTAINER> fuzzer(rule);
  for (int i = 0; i < 10000; i++) {
    auto value = fuzzer.next();
    int pid = fork();
    if (pid == 0) {
      fclose(stdout);
      // fclose(stderr);
      test_func(value);
      exit(0);
    } else {
      int status;
      waitpid(pid, &status, WUNTRACED | WCONTINUED);
      if (WIFSIGNALED(status)) {
        std::cout << "WTERMSIG:" << WTERMSIG(status) << std::endl;
        if constexpr (std::is_same<decltype(value), std::string>::value)
          std::cout << " failed with value:'" << value << "'" << std::endl;
      }
    }
  }
}
} // namespace purge
#endif // PURGE_RUNNER_HPP
