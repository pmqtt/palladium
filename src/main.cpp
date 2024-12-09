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
  program.push_back(Push("Ende of test Func\n"));
  program.push_back(Print());
  program.push_back(Push(12));
  program.push_back(Push(13));
  program.push_back(Call("add_nr"));
  program.push_back(Print());
  program.push_back(Halt());

  VirtualMachine vm(program);

  std::vector<InstructionType> fcode1 = {Push("Hallo Welt from function\n"),
                                         Print(), RetVoid()};

  std::vector<InstructionType> fcode2 = {Add(1), Return(0)};

  vm.add_function("TestFunc", fcode1, 0);
  vm.add_function("add_nr", fcode2, 2);

  vm.init_registers(0, 10, 20);
  vm.run();

  std::cout << "\nRegister[0]:" << std::get<int>(vm.reg_0()) << std::endl;

  return 0;
}
