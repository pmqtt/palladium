#ifndef _PALLADIUM_INSTRUCTION_H
#define _PALLADIUM_INSTRUCTION_H

#include "Util.h"
#include "VMType.h"
#include <cstddef>
#include <string>
#include <variant>
#include <vector>

class VirtualMachine;

using InstructionResult = ResultOr<bool>;

struct Instruction {
  Instruction() = default;
  virtual ~Instruction() = default;
  virtual InstructionResult execute(VirtualMachine *vm) = 0;
};

// c(0) = c(i)
struct Load : public Instruction {
  Load(std::size_t i);
  auto execute(VirtualMachine *vm) -> InstructionResult override;

private:
  std::size_t _i;
};

// c(0) = i
struct CLoad : public Instruction {
  CLoad(const VMType &value);
  auto execute(VirtualMachine *vm) -> InstructionResult override;

private:
  VMType _value;
};

// c(0) = c(c(i))
struct INDLoad : public Instruction {
  INDLoad(std::size_t i);
  auto execute(VirtualMachine *vm) -> InstructionResult override;

private:
  std::size_t _i;
};

// Load top of the stack to register i
// c(i) = top(stack)
struct SLoad : public Instruction {
  SLoad(std::size_t i);
  auto execute(VirtualMachine *vm) -> InstructionResult override;

private:
  std::size_t _i;
};

// c(i) = c(0)
struct Store : public Instruction {
  Store(std::size_t i);
  auto execute(VirtualMachine *vm) -> InstructionResult override;

private:
  std::size_t _i;
};

// c(c(i)) = c(0)
struct INDStore : public Instruction {
  INDStore(std::size_t i);
  auto execute(VirtualMachine *vm) -> InstructionResult override;

private:
  std::size_t _i;
};

// c(0) = c(0) +c(i)
struct Add : public Instruction {
  Add(std::size_t i);
  auto execute(VirtualMachine *vm) -> InstructionResult override;

private:
  std::size_t _i;
};

// c(0) = c(0) + i
struct CAdd : public Instruction {
  CAdd(const VMType &i);
  auto execute(VirtualMachine *vm) -> InstructionResult override;

private:
  VMType _i;
};

// c(0) = c(0) + c(c(i))
struct INDAdd : public Instruction {
  INDAdd(std::size_t i);
  auto execute(VirtualMachine *vm) -> InstructionResult override;

private:
  std::size_t _i;
};

// c(0) op i
// in (0 is <, 1 is >,2 is ==,3 is !=,4 is <=, 5 is >=)
struct If : public Instruction {
  If(std::size_t cond, const VMType &value, std::size_t target);
  auto execute(VirtualMachine *vm) -> InstructionResult override;

private:
  int _cond;
  VMType _value;
  std::size_t _target;
};

struct Goto : public Instruction {
  Goto(std::size_t i);
  auto execute(VirtualMachine *vm) -> InstructionResult override;

private:
  std::size_t _i;
};

struct Halt : public Instruction {
  Halt();
  auto execute(VirtualMachine *vm) -> InstructionResult override;
};

struct Push : public Instruction {
  Push(const VMType &value);
  auto execute(VirtualMachine *vm) -> InstructionResult override;

private:
  VMType _value;
};

struct Pop : public Instruction {
  Pop();
  auto execute(VirtualMachine *vm) -> InstructionResult override;
};

// print top of stack
struct Print : public Instruction {
  Print();
  auto execute(VirtualMachine *vm) -> InstructionResult override;
};

struct PrintRegStructField : public Instruction {
  PrintRegStructField(std::size_t i, std::size_t adr);
  auto execute(VirtualMachine *vm) -> InstructionResult override;

private:
  std::size_t _i;
  std::size_t _adr;
};

struct Call : public Instruction {
  Call(const VMType &fname);
  auto execute(VirtualMachine *vm) -> InstructionResult override;

private:
  VMType _fname;
};

struct CallNative : public Instruction {
  CallNative(const VMType &fname);
  auto execute(VirtualMachine *vm) -> InstructionResult override;

private:
  VMType _fname;
};

struct RetVoid : public Instruction {
  RetVoid();
  auto execute(VirtualMachine *vm) -> InstructionResult override;
};

struct Return : public Instruction {
  Return(std::size_t i);
  auto execute(VirtualMachine *vm) -> InstructionResult override;

public:
  std::size_t _i;
};

struct StructCreate : public Instruction {
  StructCreate(std::size_t i, std::size_t sz);
  auto execute(VirtualMachine *vm) -> InstructionResult override;

private:
  std::size_t _i;
  std::size_t _sz;
};

struct AddField : public Instruction {
  AddField(std::size_t i, const VMStructTypes &type);
  auto execute(VirtualMachine *vm) -> InstructionResult override;

private:
  std::size_t _i;
  VMStructTypes _type;
};

struct SetField : public Instruction {
  SetField(std::size_t i, std::size_t field_adr, const VMStructTypes &type);
  auto execute(VirtualMachine *vm) -> InstructionResult override;

private:
  std::size_t _i;
  std::size_t _field_adr;
  VMStructTypes _type;
};

using InstructionType =
    std::variant<Load, CLoad, INDLoad, SLoad, Store, INDStore, Add, CAdd,
                 INDAdd, If, Goto, Halt, Push, Pop, Print, PrintRegStructField,
                 Call, CallNative, RetVoid, Return, StructCreate, AddField,
                 SetField>;
#endif
