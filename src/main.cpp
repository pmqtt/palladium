#include "Instruction.h"
#include "Lexer.h"
#include "Util.h"
#include "VMType.h"
#include "VirtualMachine.h"
#include <iostream>

ResultOr<bool> palladium_write(VirtualMachine *vm,
                               const std::vector<VMType> &args) {
  UNUSED(vm);
  auto fd = get_primitive_t<int>(args[0]);
  if (!fd) {
    return fd.error_value();
  }
  auto value = get_primitive_t<std::string>(args[1]);
  if (!value) {
    return value.error_value();
  }
  auto str = value.result();
  if (::write(fd.result(), str.c_str(), str.length()) < 0) {
    return err("write failed ");
  }
  return true;
}

auto main(int argc, char **argv) -> int {
  UNUSED(argc);
  UNUSED(argv);

  std::vector<InstructionType> program;
  program.push_back(Load(1));
  program.push_back(Add(2));
  program.push_back(If(4, 200, 1));
  program.push_back(Push("Hallo Welt\n"));
  program.push_back(Print());
  program.push_back(Call("TestFunc"));
  program.push_back(Push("Ende of test Func\n"));
  program.push_back(Print());
  program.push_back(Push(12));
  program.push_back(Push(13));
  program.push_back(Call("add_nr"));
  program.push_back(Print());
  program.push_back(StructCreate(1, 1));
  program.push_back(AddField(1, 3.145f));
  program.push_back(PrintRegStructField(1, 0));
  program.push_back(Push("\nnativewrite\n")); // string to write
  program.push_back(Push(1));                 // fd stdout
  program.push_back(CallNative("write"));
  program.push_back(Halt());

  VirtualMachine vm(program);
  vm.add_native_function("write", palladium_write, 2);

  std::vector<InstructionType> fcode1 = {Push("Hallo Welt from function\n"),
                                         Print(), RetVoid()};

  std::vector<InstructionType> fcode2 = {Add(1), Return(0)};

  vm.add_function("TestFunc", fcode1, 0);
  vm.add_function("add_nr", fcode2, 2);

  vm.init_registers(0, 10, 20);
  vm.run();

  std::cout << "\nRegister[0]:" << vm_type_get<int>(vm.reg_0()) << std::endl;

  return 0;
}
