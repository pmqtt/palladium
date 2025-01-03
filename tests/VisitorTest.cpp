#include "purge.hpp"
#include <memory>
#include "Codegeneration.h"
#include "Parser.h"
#include "VMType.h"

using VM = VirtualMachine<AggresivPolicy>;
PURGE_MAIN

SIMPLE_TEST_CASE(TEST_1) {
  Parser p("fn main() -> i32 { return 22; }");
  auto res = p.parse();
  auto visitor = std::make_shared<TranslationUnitVisitor>();
  res.result()->accept(visitor);
  VMPtr vm = visitor->vm();
  REQUIRE(vm->function_entry("main").name() == "main");
  vm->run();
  REQUIRE(std::get<VMPrimitive>(vm->stack_top()) == VMPrimitive(22));
}
