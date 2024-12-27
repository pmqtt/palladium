#include "Lexer.h"
#include "purge.hpp"
#include <cstddef>
#include <vector>
PURGE_MAIN

SIMPLE_TEST_CASE(TokenKindConverter) {
  std::vector<std::string> tests = {
      "IDENTIFIER", "TEXT",        "INTEGER",    "DOUBLE",      "OP_ADD",          "OP_SUB",
      "OP_MULT",    "OP_DIV",      "OP_SET",     "OP_EQ",       "OP_NEQ",          "OP_NOT",
      "OP_LS",      "OP_LS_EQ",    "OP_GT",      "OP_GT_EQ",    "EDGE_CLAMP_OPEN", "EDGE_CLAMP_CLOSE",
      "CLAMP_OPEN", "CLAMP_CLOSE", "CURLY_OPEN", "CURLY_CLOSE", "SEMICOLON",       "ARROW",
      "COMMA",      "COLON",       "FN",         "LET",         "CONST",           "I32",
      "VOID",       "RETURN",      "WHILE",      "END_OF_FILE"};

  constexpr std::size_t max_token_kind = static_cast<std::size_t>(TokenKind::MAX_TOKEN_KIND);

  for (std::size_t i = 0; i < max_token_kind; ++i) {
    std::cout << "Tst:" << i << std::endl;
    REQUIRE(detail::to_string(static_cast<TokenKind>(i)) == tests[i]);
  }
  const std::string failure_text = "Token type not in map";

  REQUIRE(detail::to_string(TokenKind::MAX_TOKEN_KIND) == failure_text);

  for (std::size_t i = max_token_kind; i < max_token_kind + 10; ++i) {
    REQUIRE(detail::to_string(static_cast<TokenKind>(i)) == failure_text);
  }
}
