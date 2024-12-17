#include "Instruction.h"
#include "Lexer.h"
#include "Util.h"
#include "VMType.h"
#include "VirtualMachine.h"
#include <iostream>

template <class VM>
ResultOr<bool> palladium_write(VM *vm, const std::vector<VMType> &args) {
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
  using VM = VirtualMachine<AggresivPolicy>;

  std::vector<VM::InstructionTypeV> program;
  program.push_back(Load<VM>(1));
  program.push_back(Add<VM>(2));
  program.push_back(If<VM>(4, 200, 1));
  program.push_back(Push<VM>("Hallo Welt\n"));
  program.push_back(Print<VM>());
  program.push_back(Call<VM>("TestFunc"));
  program.push_back(Push<VM>("Ende of test Func\n"));
  program.push_back(Print<VM>());
  program.push_back(Push<VM>(12));
  program.push_back(Push<VM>(13));
  program.push_back(Call<VM>("add_nr"));
  program.push_back(Print<VM>());
  program.push_back(StructCreate<VM>(1, 1));
  program.push_back(AddField<VM>(1, 3.145f));
  program.push_back(PrintRegStructField<VM>(1, 0));
  program.push_back(Push<VM>("\nnativewrite\n")); // string to write
  program.push_back(Push<VM>(1));                 // fd stdout
  program.push_back(CallNative<VM>("write"));
  program.push_back(Allocate<VM>(5));
  program.push_back(Push<VM>("HALLO"));
  program.push_back(WriteMem<VM>());
  program.push_back(Deallocate<VM>());
  program.push_back(Halt<VM>());

  VM vm(program, 128);
  vm.add_native_function("write", palladium_write<VM>, 2);

  std::vector<VM::InstructionTypeV> fcode1 = {
      Push<VM>("Hallo Welt from function\n"), Print<VM>(), RetVoid<VM>()};

  std::vector<VM::InstructionTypeV> fcode2 = {Add<VM>(1), Return<VM>(0)};

  vm.add_function("TestFunc", fcode1, 0);
  vm.add_function("add_nr", fcode2, 2);

  vm.init_registers(0, 10, 20);
  vm.run();
  // vm.step();
  vm.print_memory();

  return 0;
}
