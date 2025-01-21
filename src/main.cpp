#include "Instruction.h"
#include "Lexer.h"
#include "Util.h"
#include "VMType.h"
#include "VirtualMachine.h"
#include <iostream>

template <class VM> ResultOr<bool> palladium_write(VM* vm, const std::vector<VMType>& args) {
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

auto main(int argc, char** argv) -> int {
  UNUSED(argc);
  UNUSED(argv);
  using VM = VirtualMachine<AggresivPolicy>;
  std::vector<VM::InstructionTypeV*> program;
  program.push_back(new Load<VM>(1));
  program.push_back(new Add<VM>(2));
  program.push_back(new If<VM>(4, 200, 1));
  program.push_back(new Push<VM>("Hallo Welt\n"));
  program.push_back(new Print<VM>());
  program.push_back(new Call<VM>("TestFunc"));
  program.push_back(new Push<VM>("Ende of test Func\n"));
  program.push_back(new Print<VM>());
  program.push_back(new Push<VM>(12));
  program.push_back(new Push<VM>(13));
  program.push_back(new Call<VM>("add_nr"));
  program.push_back(new Print<VM>());
  program.push_back(new StructCreate<VM>(1, 1));
  program.push_back(new AddField<VM>(1, 3.145f));
  program.push_back(new PrintRegStructField<VM>(1, 0));
  program.push_back(new Push<VM>("\nnativewrite\n")); // string to write
  program.push_back(new Push<VM>(1));                 // fd stdout
  program.push_back(new CallNative<VM>("write"));

  program.push_back(new Allocate<VM>(static_cast<int>(sizeof(int))));
  program.push_back(new Push<VM>(32));
  program.push_back(new WriteMem<VM>());
  program.push_back(new Push<VM>(static_cast<int>(VMTypeKind::VM_INT))); // type
  program.push_back(new Push<VM>(static_cast<int>(sizeof(int))));        // bytes
  program.push_back(new ReadMem<VM>());
  program.push_back(new Print<VM>());
  program.push_back(new Deallocate<VM>());
  program.push_back(new Halt<VM>());

  VM vm(program, 128);
  vm.add_native_function("write", palladium_write<VM>, 2);

  std::vector<VM::InstructionTypeV*> fcode1 = {new Push<VM>("Hallo Welt from function\n"), new Print<VM>(),
                                               new RetVoid<VM>()};

  std::vector<VM::InstructionTypeV*> fcode2 = {new Add<VM>(1), new Return<VM>(0)};

  vm.add_function("TestFunc", fcode1, 0);
  vm.add_function("add_nr", fcode2, 2);

  vm.init_registers(0, 10, 20);
  vm.run();
  // vm.step();

  std::cout << std::endl;
  vm.print_memory();

  return 0;
}
