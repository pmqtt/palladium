#ifndef _PALLADIUM_VM_H
#define _PALLADIUM_VM_H
#include "Instruction.h"
#include <algorithm>
#include <iostream>
#include <memory>
#include <variant>

class VirtualMachine {
public:
  VirtualMachine(const std::vector<InstructionType> &program)
      : _program(program), _registers(10, 0), _pc(0), _sp(-1) {}

  void run() {
    std::size_t old_pc = 0;
    do {
      old_pc = _pc;
      std::visit(
          [&](auto &instruction) {
            instruction.execute(this);
            instruction.change_pc(this);
          },
          _program[_pc]);
    } while (_pc != old_pc);
  }

  template <class... ARG> auto init_registers(int x, ARG... args) {
    std::vector<int> tmp = {x, args...};
    std::copy_n(tmp.begin(), std::min(tmp.size(), _registers.size()),
                _registers.begin());
  }
  auto reg_0() const -> VMType { return _registers[0]; }
  auto registers() -> std::vector<VMType> & { return _registers; }
  void inc_pc(std::size_t inc = 1) { _pc += inc; }
  void inc_sp(int inc = 1) { _sp += inc; }
  void set_sp(int sp) { _sp = sp; }
  void set_pc(std::size_t pc) { _pc += pc; }
  auto stack_pointer() const -> int { return _sp; }
  auto stack_top() -> VMType & { return _stack[_sp]; }
  void stack_pop() { _sp -= 1; }
  void stack_push(const VMType &value) {
    _sp += 1;
    _stack.push_back(value);
  }

private:
  std::vector<InstructionType> _program;
  std::vector<VMType> _registers;
  std::size_t _pc;
  std::vector<VMType> _stack;
  int _sp;
};

#endif
