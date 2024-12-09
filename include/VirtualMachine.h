#ifndef _PALLADIUM_VM_H
#define _PALLADIUM_VM_H
#include "Instruction.h"
#include "Util.h"
#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <variant>

struct FunctionEntry {
  FunctionEntry(std::string name, uint8_t arg_count, std::size_t adr)
      : _name(std::move(name)), _argument_count(arg_count), _address(adr) {}

  auto name() const -> const std::string & { return _name; }
  auto argument_count() const -> uint8_t { return _argument_count; }
  auto address() const -> std::size_t { return _address; }

private:
  std::string _name;
  uint8_t _argument_count;
  std::size_t _address;
};

struct StackFrame {
  std::size_t pc;
  std::vector<VMType> registers;
};

class VirtualMachine {
public:
  VirtualMachine(const std::vector<InstructionType> &program)
      : _program(program), _registers(10, 0), _pc(0), _stack(10, 0), _sp(-1) {}

  void add_function(const std::string fname,
                    const std::vector<InstructionType> &code,
                    uint8_t arg_count) {

    _function_section.push_back({fname, arg_count, _program.size()});
    std::copy(code.cbegin(), code.cend(), std::back_inserter(_program));
  }
  auto function_entry(const std::string &fname) const -> const FunctionEntry {
    for (const auto &f_item : _function_section) {
      if (f_item.name() == fname) {
        return f_item;
      }
    }
    panic("function " + fname + " not exist");
  }

  void run() {
    std::size_t old_pc = 0;
    do {
      old_pc = _pc;
      std::visit(
          [&](auto &instruction) {
            InstructionResult res = instruction.execute(this);
            res.error([](const Error &err) {
              std::cerr << "Instruction failed: " << err.msg() << "\n";
              std::abort();
            });
          },
          _program[_pc]);
    } while (_pc != old_pc);
  }

  template <class... ARG> auto init_registers(ARG &&...args) {
    std::vector<int> tmp = {std::forward<ARG>(args)...};
    std::copy_n(tmp.begin(), std::min(tmp.size(), _registers.size()),
                _registers.begin());
  }
  auto reg_0() const -> VMType { return _registers[0]; }
  auto registers() -> std::vector<VMType> & { return _registers; }
  void inc_pc(std::size_t inc = 1) { _pc += inc; }
  auto pc() const -> std::size_t { return _pc; }
  void inc_sp(int inc = 1) { _sp += inc; }
  void set_sp(int sp) { _sp = sp; }
  void set_pc(std::size_t pc) { _pc = pc; }
  auto stack_pointer() const -> int { return _sp; }
  auto stack_top() -> VMType & { return _stack[_sp]; }
  void stack_pop() { _sp -= 1; }
  void make_stack_frame() {
    StackFrame frame = {.pc = _pc, .registers = _registers};
    _call_stack.push_back(frame);
  }
  void restore_from_call_stack() {
    StackFrame frame = _call_stack[_call_stack.size() - 1];
    _pc = frame.pc + 1;
    _registers = frame.registers;
    _call_stack.pop_back();
  }

  void stack_push(const VMType &value) {
    _sp += 1;
    if (std::cmp_less(_sp, _stack.size())) {
      _stack[_sp] = value;
    } else {
      _stack.push_back(value);
    }
  }

private:
  std::vector<InstructionType> _program;
  std::vector<VMType> _registers;
  std::size_t _pc;
  std::vector<VMType> _stack;
  int _sp;
  std::vector<FunctionEntry> _function_section;
  std::vector<StackFrame> _call_stack;
};

#endif
