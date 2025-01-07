#include "purge.hpp"
#include <memory>
#include "Codegeneration.h"
#include "Parser.h"
#include "VMType.h"

using VM = VirtualMachine<AggresivPolicy>;
PURGE_MAIN

SIMPLE_TEST_CASE(TEST_1) {
  Parser p("fn main() -> i32 { let a: i32 = 2; let b: i32 = 13; let c: i32 = 14; a = b + c; return 22; }");
  auto res = p.parse();
  if (res.ok() == false) {
    std::cout << res.error_value().msg() << std::endl;
    auto context = p.context();
    while (!context.empty()) {
      auto x = context.top();
      context.pop();
      std::cout << "context:" << x.context << " " << static_cast<std::size_t>(x.rule) << std::endl;
    }
  }
  auto visitor = std::make_shared<TranslationUnitVisitor>();
  res.result()->accept(visitor);
  VMPtr vm = visitor->vm();
  REQUIRE(vm->function_entry("main").name() == "main");
  vm->run();
  REQUIRE(std::get<VMPrimitive>(vm->stack_top()) == VMPrimitive(22));
  std::cout << vm->to_string() << std::endl;

  vm->print_stack();
}
