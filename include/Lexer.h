#ifndef PALLADIUM_LEXER_H_
#define PALLADIUM_LEXER_H_
#include <istream>
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

class Token final {
public:
  Token(const TokenKind &tk, std::string value)
      : _kind(tk), _value(std::move(value)) {}

  auto operator==(const TokenKind &rhs) -> bool { return _kind == rhs; }

  auto kind() const -> TokenKind { return _kind; }
  auto value() const -> const std::string & { return _value; }

private:
  TokenKind _kind;
  std::string _value;
};

#endif
