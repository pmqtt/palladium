#include "Instruction.h"
#include "Util.h"
#include "VirtualMachine.h"
#include <cstdint>
#include <iterator>
#include <string>
#include <type_traits>
#include <variant>

template <class L, class R>
auto create_operator_error(const std::string &str) -> Error {
  std::string e = str;
  e += " not permitted (";
  e += typeid(L).name();
  e += ",";
  e += typeid(R).name();
  e += ")";
  return err(e);
}

auto add(VMType &lhs, VMType &rhs) -> ResultOr<VMType> {
  return std::visit(
      [](const auto &lhv, const auto &rhv) -> ResultOr<VMType> {
        using L = std::remove_cvref_t<decltype(lhv)>;
        using R = std::remove_cvref_t<decltype(rhv)>;

        if constexpr (std::is_arithmetic_v<L> && std::is_arithmetic_v<R>) {
          using CommonType = std::common_type_t<L, R>;
          return {static_cast<CommonType>(lhv) +
                  static_cast<CommonType>(rhv)}; // Numerische Addition
        } else if constexpr (std::is_same_v<L, std::string> &&
                             std::is_same_v<R, std::string>) {
          return {lhv + rhv}; // String-Konkatenation
        }
        return create_operator_error<L, R>("add");
      },
      lhs, rhs);
}

auto sub(VMType &lhs, VMType &rhs) -> ResultOr<VMType> {
  return std::visit(
      [](const auto &lhv, const auto &rhv) -> ResultOr<VMType> {
        using L = std::remove_cvref_t<decltype(lhv)>;
        using R = std::remove_cvref_t<decltype(rhv)>;

        if constexpr (std::is_arithmetic_v<L> && std::is_arithmetic_v<R>) {
          using CommonType = std::common_type_t<L, R>;
          return {static_cast<CommonType>(lhv) -
                  static_cast<CommonType>(rhv)}; // Numerische Addition
        }
        return create_operator_error<L, R>("sub");
      },
      lhs, rhs);
}

auto mult(VMType &lhs, VMType &rhs) -> ResultOr<VMType> {
  return std::visit(
      [](const auto &lhv, const auto &rhv) -> ResultOr<VMType> {
        using L = std::remove_cvref_t<decltype(lhv)>;
        using R = std::remove_cvref_t<decltype(rhv)>;
        if constexpr (std::is_arithmetic_v<L> && std::is_arithmetic_v<R>) {
          using CommonType = std::common_type_t<L, R>;
          return {static_cast<CommonType>(lhv) *
                  static_cast<CommonType>(rhv)}; // Numerische Addition
        }
        return create_operator_error<L, R>("mult");
      },
      lhs, rhs);
}

auto to_string(VMType &value) -> ResultOr<std::string> {
  return std::visit(
      [](const auto &v) -> ResultOr<std::string> {
        using L = std::remove_cvref_t<decltype(v)>;
        if constexpr (std::is_same_v<L, std::string>) {
          return v;
        } else if (std::is_arithmetic_v<L>) {
          return std::to_string(v);
        }
        return err("to_string not permitted on type " +
                   std::string(typeid(L).name()));
      },
      value);
}

// Load Instruction
Load::Load(std::size_t i) : _i(i) {}

auto Load::execute(VirtualMachine *vm) -> InstructionResult {
  auto &registers = vm->registers();
  registers[0] = registers[_i];
  vm->inc_pc();
  return true;
}
//======================================
// CLoad Instruction

CLoad::CLoad(const VMType &value) : _value(value) {}

auto CLoad::execute(VirtualMachine *vm) -> InstructionResult {
  auto &registers = vm->registers();
  registers[0] = _value;
  vm->inc_pc();
  return true;
}

//======================================
// INDLoad Instruction

INDLoad::INDLoad(std::size_t i) : _i(i) {}

auto INDLoad::execute(VirtualMachine *vm) -> InstructionResult {
  auto &registers = vm->registers();
  if (std::holds_alternative<int>(registers[_i])) {
    int index = std::get<int>(registers[_i]);
    registers[0] = registers[index];
  }
  vm->inc_pc();
  return true;
}

//======================================
// Load Instruction
SLoad::SLoad(std::size_t i) : _i(i) {}

auto SLoad::execute(VirtualMachine *vm) -> InstructionResult {
  auto &registers = vm->registers();
  registers[0] = vm->stack_top();
  vm->stack_pop();
  vm->inc_pc();

  return true;
}

//======================================
// Store Instruction

Store::Store(std::size_t i) : _i(i) {}

auto Store::execute(VirtualMachine *vm) -> InstructionResult {
  auto &registers = vm->registers();
  registers[_i] = registers[0];
  vm->inc_pc();
  return true;
}

//======================================
// INDStore Instruction

INDStore::INDStore(std::size_t i) : _i(i) {}

auto INDStore::execute(VirtualMachine *vm) -> InstructionResult {
  auto &registers = vm->registers();
  if (std::holds_alternative<int>(registers[_i])) {
    int index = std::get<int>(registers[_i]);
    registers[index] = registers[0];
    vm->inc_pc();
  }
  return err("expected int in register reg(" + std::to_string(_i) + ")");
}

//======================================
// Add Instruction

Add::Add(std::size_t i) : _i(i) {}
auto Add::execute(VirtualMachine *vm) -> InstructionResult {
  auto &registers = vm->registers();

  auto res = add(registers[0], registers[_i]);
  if (res.ok()) {
    registers[0] = res.result();
    vm->inc_pc();
    return true;
  }
  return res.error_value();
}

//======================================
// CAdd Instruction
CAdd::CAdd(const VMType &i) : _i(i) {}
auto CAdd::execute(VirtualMachine *vm) -> InstructionResult {
  auto &registers = vm->registers();
  auto res = add(registers[0], _i);
  if (res.ok()) {
    registers[0] = res.result();
    vm->inc_pc();
    return true;
  }
  return res.error_value();
}

//======================================
// INDAdd Instruction

INDAdd::INDAdd(std::size_t i) : _i(i) {}

auto INDAdd::execute(VirtualMachine *vm) -> InstructionResult {
  auto &registers = vm->registers();
  if (std::holds_alternative<int>(registers[_i])) {
    int index = std::get<int>(registers[_i]);
    auto res = add(registers[0], registers[index]);
    if (res.ok()) {
      registers[0] = res.result();
      vm->inc_pc();
      return true;
    }
    return res.error_value();
  }
  return err("expected int in register reg(" + std::to_string(_i) + ")");
}

//======================================
// If Instruction
If::If(std::size_t cond, const VMType &value, std::size_t target)
    : _cond(cond), _value(value), _target(target) {}

auto If::execute(VirtualMachine *vm) -> InstructionResult {
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
  return true;
}

//======================================
// Goto Instruction

Goto::Goto(std::size_t i) : _i(i) {}
auto Goto::execute(VirtualMachine *vm) -> InstructionResult {
  vm->set_pc(_i);
  return true;
}

//======================================
// Halt Instruction

Halt::Halt() {}
auto Halt::execute(VirtualMachine *vm) -> InstructionResult {
  UNUSED(vm);
  return true;
}

//======================================
// Push Instruction

Push::Push(const VMType &value) : _value(value) {}
auto Push::execute(VirtualMachine *vm) -> InstructionResult {
  vm->stack_push(_value);
  vm->inc_pc();
  return true;
}

//======================================
// Push Instruction

Pop::Pop() {}
auto Pop::execute(VirtualMachine *vm) -> InstructionResult {
  vm->stack_pop();
  vm->inc_pc();
  return true;
}

//=====================================
// Print Instruction

Print::Print() {}
auto Print::execute(VirtualMachine *vm) -> InstructionResult {
  auto v = vm->stack_top();
  vm->stack_pop();
  auto res = to_string(v);
  if (res.ok()) {
    std::cout << res.result();
    vm->inc_pc();
    return true;
  }
  return res.error_value();
}

//=====================================
// Call Instruction

Call::Call(const VMType &fname) : _fname(fname) {}

auto Call::execute(VirtualMachine *vm) -> InstructionResult {
  std::string fname = std::get<std::string>(_fname);

  const FunctionEntry &entry = vm->function_entry(fname);
  vm->make_stack_frame();
  vm->set_pc(entry.address());
  if (vm->registers().size() <= entry.argument_count()) {
    return err("Function " + fname +
               " not enough registers to store arguments");
  }
  for (uint8_t i = 0; i < entry.argument_count(); ++i) {
    auto value = vm->stack_top();
    vm->registers()[i] = value;
    vm->stack_pop();
  }
  return true;
}

//=====================================
// RetVoid Instruction

RetVoid::RetVoid() {}

auto RetVoid::execute(VirtualMachine *vm) -> InstructionResult {
  vm->restore_from_call_stack();
  return true;
}

//=====================================
// RetVoid Instruction

Return::Return(std::size_t i) : _i(i) {}

auto Return::execute(VirtualMachine *vm) -> InstructionResult {
  const VMType ret_value = vm->registers()[_i];
  vm->restore_from_call_stack();
  vm->stack_push(ret_value);
  return true;
}
