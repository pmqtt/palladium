#include "Lexer.h"
#include "purge.hpp"
#include <cstddef>
#include <vector>
PURGE_MAIN

SIMPLE_TEST_CASE(TokenKindConverter) {
  std::vector<std::string> tests = {
      "IDENTIFIER", "TEXT",   "INTEGER", "DOUBLE", "OP_ADD", "OP_SUB",
      "OP_MULT",    "OP_DIV", "OP_SET",  "OP_EQ",  "OP_NEQ", "END_OF_FILE",
  };

  for (int i = 0; i < static_cast<int>(TokenKind::MAX_TOKEN_KIND); ++i) {
    REQUIRE(detail::to_string(static_cast<TokenKind>(i)) == tests[i]);
  }
  REQUIRE(detail::to_string(TokenKind::MAX_TOKEN_KIND) ==
          "Token type not in map");
}
