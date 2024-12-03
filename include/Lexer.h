#ifndef PALLADIUM_LEXER_H_
#define PALLADIUM_LEXER_H_
#include <string>

enum class TokenKind {
  // Literals
  IDENTIFIER = 0,
  TEXT,
  INTEGER,
  DOUBLE,

  // Operators
  OP_ADD,
  OP_SUB,
  OP_MULT,
  OP_DIV,
  OP_SET, // =
  OP_EQ,  // ==
  OP_NEQ,

  // Special tokens
  END_OF_FILE,
  MAX_TOKEN_KIND,
};

namespace detail {
auto to_string(TokenKind tk) -> std::string;
}

#endif
