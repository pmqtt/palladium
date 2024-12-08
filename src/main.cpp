#include "Instruction.h"
#include "Lexer.h"
#include "Util.h"
#include "VirtualMachine.h"
#include <iostream>

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
  program.push_back(Halt());

  VirtualMachine vm(program);

  std::vector<InstructionType> fcode = {Push("Hallo Welt from function\n"),
                                        Print(), Halt()};
  vm.add_function("TestFunc", fcode);

  vm.init_registers(0, 10, 20);
  vm.run();

  std::cout << "Register[0]:" << std::get<int>(vm.reg_0()) << std::endl;

  return 0;
}
