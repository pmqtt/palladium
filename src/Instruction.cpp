#include "Instruction.h"
#include "Util.h"
#include "VMType.h"
#include "VirtualMachine.h"
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <ostream>
#include <string>
#include <type_traits>
#include <variant>

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
  if (is_vm_type<int>(registers[_i])) {
    int index = vm_type_get<int>(registers[_i]);
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
  if (is_vm_type<int>(registers[_i])) {
    int index = vm_type_get<int>(registers[_i]);
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
  if (is_vm_type<int>(registers[_i])) {
    int index = vm_type_get<int>(registers[_i]);
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
  auto register_value = std::get<VMPrimitive>(registers[0]);
  auto value = std::get<VMPrimitive>(_value);
  bool condition = false;

  switch (_cond) {
  case 0:
    condition = (register_value < value);
    break;
  case 1:
    condition = (register_value > value);
    break;
  case 2:
    condition = (register_value == value);
    break;
  case 3:
    condition = (register_value != value);
    break;
  case 4:
    condition = (register_value <= value);
    break;
  case 5:
    condition = (register_value >= value);
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
    std::flush(std::cout);
    vm->inc_pc();
    return true;
  }
  return res.error_value();
}

//=====================================
// Print Instruction

PrintRegStructField::PrintRegStructField(std::size_t i, std::size_t adr)
    : _i(i), _adr(adr) {}

auto PrintRegStructField::execute(VirtualMachine *vm) -> InstructionResult {
  auto &v = std::get<VMStruct>(vm->registers()[_i]).get_field(_adr);
  VMType field_value = std::get<VMPrimitive>(v);
  auto res = to_string(field_value);
  if (res.ok()) {
    std::cout << res.result();
    std::flush(std::cout);
    vm->inc_pc();
    return true;
  }
  return res.error_value();
}

//=====================================
// Call Instruction

Call::Call(const VMType &fname) : _fname(fname) {}

auto Call::execute(VirtualMachine *vm) -> InstructionResult {
  std::string fname = vm_type_get<std::string>(_fname).result_or("");

  const FunctionEntry &entry = vm->function_entry(fname);
  vm->make_stack_frame();
  if (vm->registers().size() <= entry.argument_count()) {
    return err("Function " + fname +
               " not enough registers to store arguments");
  }
  for (uint8_t i = 0; i < entry.argument_count(); ++i) {
    auto value = vm->stack_top();
    vm->registers()[i] = value;
    vm->stack_pop();
  }
  vm->set_pc(entry.address());
  return true;
}

//=====================================
// CallNative Instruction

CallNative::CallNative(const VMType &fname) : _fname(fname) {}

auto CallNative::execute(VirtualMachine *vm) -> InstructionResult {
  std::string fname = vm_type_get<std::string>(_fname).result_or("");

  const NativeFunctionEntry &entry = vm->native_function_entry(fname);
  std::vector<VMType> args;
  for (uint8_t i = 0; i < entry.argument_count(); ++i) {
    auto value = vm->stack_top();
    args.push_back(value);
    vm->stack_pop();
  }
  auto res = entry(vm, args);
  if (!res) {
    return res;
  }
  vm->inc_pc();
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

//=====================================
// StructCreate Instruction

StructCreate::StructCreate(std::size_t i, std::size_t sz) : _i(i), _sz(sz) {}

auto StructCreate::execute(VirtualMachine *vm) -> InstructionResult {
  vm->registers()[_i] = VMStruct(_sz);
  vm->inc_pc();
  return true;
}

//=====================================
// AddField

AddField::AddField(std::size_t i, const VMStructTypes &type)
    : _i(i), _type(type) {}

auto AddField::execute(VirtualMachine *vm) -> InstructionResult {
  auto &s = std::get<VMStruct>(vm->registers()[_i]);
  s.add_field(_type);
  vm->inc_pc();
  return true;
}

//=====================================
// SetField

SetField::SetField(std::size_t i, std::size_t field_adr,
                   const VMStructTypes &type)
    : _i(i), _field_adr(field_adr), _type(type) {}

auto SetField::execute(VirtualMachine *vm) -> InstructionResult {
  auto &s = std::get<VMStruct>(vm->registers()[_i]);
  s.set_field(_field_adr, _type);
  vm->inc_pc();
  return true;
}
