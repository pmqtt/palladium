#include "purge.hpp"
#include "Parser.h"
PURGE_MAIN

SIMPLE_TEST_CASE(ParserSimpleTest1) {
  Parser p("fn main() -> i32 { return 0; }");
  REQUIRE(p.parse().ok() == true);
}

SIMPLE_TEST_CASE(ParserSimpleTest_Failed_1) {
  Parser p("fn main() -> i32 { return 0 }");
  auto res = p.parse();
  REQUIRE(res.ok() == false);
}
