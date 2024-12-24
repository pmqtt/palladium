#ifndef PALLADIUM_VM_POLICY_H
#define PALLADIUM_VM_POLICY_H

#include "Util.h"
#include <cstddef>
#include <string>
#include <utility>

struct AggresivPolicy {
  static void check_stack_bounds([[maybe_unused]] int sp, [[maybe_unused]] std::size_t max_size) {
  }
  static void print_dbg([[maybe_unused]] const std::string& inst) {
  }
  template <class VM> static void check_register_bounds([[maybe_unused]] VM* vm, [[maybe_unused]] std::size_t index) {
  }
  static void check_memory_bound([[maybe_unused]] std::size_t index, [[maybe_unused]] std::size_t sz) {
  }

  static void check_memory_adress(std::size_t base, std::size_t destination) {
    if (base > destination) {
      panic("Illigal memory access at " + std::to_string(destination));
    }
  }

  static void check_equal_adress(std::size_t first, std::size_t second) {
    if (first != second) {
      panic("Illigal memory adress " + std::to_string(first));
    }
  }
};

struct DebugPolicy {
  static void check_stack_bounds(int sp, std::size_t max_size) {
    if (sp < -1) {
      panic("Stack underflow");
    }
    if (std::cmp_greater_equal(sp, max_size)) {
      panic("Stack overflow");
    }
  }

  static void print_dbg(const std::string& inst) {
    std::string msg;
    for (auto& c : inst) {
      if (std::isspace(c) && c != ' ') {
        msg += "\\n";
      } else {
        msg += c;
      }
    }
    std::cout << msg << std::endl;
  }

  static void check_memory_bound(std::size_t index, std::size_t sz) {
    if (index >= sz) {
      panic("Illigal memory access at index " + std::to_string(index));
    }
  }

  template <class VM> static void check_register_bounds(VM* vm, std::size_t index) {
    if (index >= vm->registers().size()) {
      panic("index " + std::to_string(index) + "out of bounds");
    }
  }

  static void check_memory_adress(std::size_t base, std::size_t destination) {
    if (base > destination) {
      panic("Illigal memory access at " + std::to_string(destination));
    }
  }

  static void check_equal_adress(std::size_t first, std::size_t second) {
    if (first != second) {
      panic("Illigal memory adress " + std::to_string(first));
    }
  }
};

#endif
