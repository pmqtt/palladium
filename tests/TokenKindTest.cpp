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

  constexpr std::size_t max_token_kind =
      static_cast<std::size_t>(TokenKind::MAX_TOKEN_KIND);

  for (std::size_t i = 0; i < max_token_kind; ++i) {
    REQUIRE(detail::to_string(static_cast<TokenKind>(i)) == tests[i]);
  }
  const std::string failure_text = "Token type not in map";

  REQUIRE(detail::to_string(TokenKind::MAX_TOKEN_KIND) == failure_text);

  for (std::size_t i = max_token_kind; i < max_token_kind + 10; ++i) {
    REQUIRE(detail::to_string(static_cast<TokenKind>(i)) == failure_text);
  }
}
