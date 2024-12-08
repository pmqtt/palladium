#include "Instruction.h"
#include "Util.h"
#include "VirtualMachine.h"
#include <type_traits>
#include <variant>

auto add(VMType &lhs, VMType &rhs) -> VMType {
  return std::visit(
      [](const auto &lhv, const auto &rhv) -> VMType {
        using L = std::remove_cvref_t<decltype(lhv)>;
        using R = std::remove_cvref_t<decltype(rhv)>;

        if constexpr (std::is_arithmetic_v<L> && std::is_arithmetic_v<R>) {
          using CommonType = std::common_type_t<L, R>;
          return static_cast<CommonType>(lhv) +
                 static_cast<CommonType>(rhv); // Numerische Addition
        } else if constexpr (std::is_same_v<L, std::string> &&
                             std::is_same_v<R, std::string>) {
          return lhv + rhv; // String-Konkatenation
        }
        panic(false);
        return -1;
      },
      lhs, rhs);
}

auto sub(VMType &lhs, VMType &rhs) -> VMType {
  return std::visit(
      [](const auto &lhv, const auto &rhv) -> VMType {
        using L = std::remove_cvref_t<decltype(lhv)>;
        using R = std::remove_cvref_t<decltype(rhv)>;

        if constexpr (std::is_arithmetic_v<L> && std::is_arithmetic_v<R>) {
          using CommonType = std::common_type_t<L, R>;
          return static_cast<CommonType>(lhv) -
                 static_cast<CommonType>(rhv); // Numerische Addition
        }
        panic(false);
        return -1;
      },
      lhs, rhs);
}

auto mult(VMType &lhs, VMType &rhs) -> VMType {
  return std::visit(
      [](const auto &lhv, const auto &rhv) -> VMType {
        using L = std::remove_cvref_t<decltype(lhv)>;
        using R = std::remove_cvref_t<decltype(rhv)>;
        if constexpr (std::is_arithmetic_v<L> && std::is_arithmetic_v<R>) {
          using CommonType = std::common_type_t<L, R>;
          return static_cast<CommonType>(lhv) +
                 static_cast<CommonType>(rhv); // Numerische Addition
        }
        panic(false);
        return -1;
      },
      lhs, rhs);
}

auto to_string(VMType &value) -> std::string {
  return std::visit(
      [](const auto &v) -> std::string {
        using L = std::remove_cvref_t<decltype(v)>;
        if constexpr (std::is_same_v<L, std::string>) {
          return v;
        } else if (std::is_arithmetic_v<L>) {
          return std::to_string(v);
        }
        panic(false);
        return "";
      },
      value);
}

// Load Instruction
Load::Load(std::size_t i) : _i(i) {}

void Load::execute(VirtualMachine *vm) {
  auto &registers = vm->registers();
  registers[0] = registers[_i];
  vm->inc_pc();
}
//======================================
// CLoad Instruction

CLoad::CLoad(const VMType &value) : _value(value) {}

void CLoad::execute(VirtualMachine *vm) {
  auto &registers = vm->registers();
  registers[0] = _value;
  vm->inc_pc();
}

//======================================
// INDLoad Instruction

INDLoad::INDLoad(std::size_t i) : _i(i) {}

void INDLoad::execute(VirtualMachine *vm) {
  auto &registers = vm->registers();
  if (std::holds_alternative<int>(registers[_i])) {
    int index = std::get<int>(registers[_i]);
    registers[0] = registers[index];
  }
  vm->inc_pc();
}

//======================================
// Load Instruction
SLoad::SLoad(std::size_t i) : _i(i) {}

void SLoad::execute(VirtualMachine *vm) {
  auto &registers = vm->registers();
  registers[0] = vm->stack_top();
  vm->stack_pop();
  vm->inc_pc();
}

//======================================
// Store Instruction

Store::Store(std::size_t i) : _i(i) {}

void Store::execute(VirtualMachine *vm) {
  auto &registers = vm->registers();
  registers[_i] = registers[0];
  vm->inc_pc();
}

//======================================
// INDStore Instruction

INDStore::INDStore(std::size_t i) : _i(i) {}

void INDStore::execute(VirtualMachine *vm) {
  auto &registers = vm->registers();
  if (std::holds_alternative<int>(registers[_i])) {
    int index = std::get<int>(registers[_i]);
    registers[index] = registers[0];
    vm->inc_pc();
  }
  panic(false);
}

//======================================
// Add Instruction

Add::Add(std::size_t i) : _i(i) {}
void Add::execute(VirtualMachine *vm) {
  auto &registers = vm->registers();
  registers[0] = add(registers[0], registers[_i]);
  vm->inc_pc();
}

//======================================
// CAdd Instruction
CAdd::CAdd(const VMType &i) : _i(i) {}
void CAdd::execute(VirtualMachine *vm) {
  auto &registers = vm->registers();
  registers[0] = add(registers[0], _i);
  vm->inc_pc();
}

//======================================
// INDAdd Instruction

INDAdd::INDAdd(std::size_t i) : _i(i) {}

void INDAdd::execute(VirtualMachine *vm) {
  auto &registers = vm->registers();
  if (std::holds_alternative<int>(registers[_i])) {
    int index = std::get<int>(registers[_i]);
    registers[0] = add(registers[0], registers[index]);
    vm->inc_pc();
  }
  panic(false);
}

//======================================
// If Instruction
If::If(std::size_t cond, const VMType &value, std::size_t target)
    : _cond(cond), _value(value), _target(target) {}

void If::execute(VirtualMachine *vm) {
  auto &registers = vm->registers();
  auto register_value = registers[0];
  bool condition = false;
  switch (_cond) {
  case 0:
    condition = (register_value < _value);
    break;
  case 1:
    condition = (register_value > _value);
    break;
  case 2:
    condition = (register_value == _value);
    break;
  case 3:
    condition = (register_value != _value);
    break;
  case 4:
    condition = (register_value <= _value);
    break;
  case 5:
    condition = (register_value >= _value);
    break;
  }
  if (condition) {
    vm->set_pc(_target);
  } else {
    vm->inc_pc();
  }
}

//======================================
// Goto Instruction

Goto::Goto(std::size_t i) : _i(i) {}
void Goto::execute(VirtualMachine *vm) { vm->set_pc(_i); }

//======================================
// Halt Instruction

Halt::Halt() {}
void Halt::execute(VirtualMachine *vm) {}

//======================================
// Push Instruction

Push::Push(const VMType &value) : _value(value) {}
void Push::execute(VirtualMachine *vm) {
  vm->stack_push(_value);
  vm->inc_pc();
}

//======================================
// Push Instruction

Pop::Pop() {}
void Pop::execute(VirtualMachine *vm) {
  vm->stack_pop();
  vm->inc_pc();
}

//=====================================
// Print Instruction

Print::Print() {}
void Print::execute(VirtualMachine *vm) {
  auto v = vm->stack_top();
  vm->stack_pop();
  std::cout << to_string(v);
  vm->inc_pc();
}

//=====================================
// Call Instruction

Call::Call(const VMType &fname) : _fname(fname) {}

void Call::execute(VirtualMachine *vm) {
  std::size_t adr = vm->function_address(std::get<std::string>(_fname));
  vm->set_pc(adr);
}
