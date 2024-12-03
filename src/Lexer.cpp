#include "Lexer.h"
#include "Util.h"
#include <string>
#include <unordered_map>
namespace detail {
auto to_string(TokenKind tk) -> std::string {
  static std::unordered_map<TokenKind, std::string> converter = {
      {TokenKind::IDENTIFIER, "IDENTIFIER"},
      {TokenKind::TEXT, "TEXT"},
      {TokenKind::INTEGER, "INTEGER"},
      {TokenKind::DOUBLE, "DOUBLE"},
      {TokenKind::OP_ADD, "OP_ADD"},
      {TokenKind::OP_SUB, "OP_SUB"},
      {TokenKind::OP_MULT, "OP_MULT"},
      {TokenKind::OP_DIV, "OP_DIV"},
      {TokenKind::OP_SET, "OP_SET"},
      {TokenKind::OP_EQ, "OP_EQ"},
      {TokenKind::OP_NEQ, "OP_NEQ"},
      {TokenKind::END_OF_FILE, "END_OF_FILE"},
  };

  if (converter.count(tk) <= 0) {
    return "Token type not in map";
  }
  return converter[tk];
}

} // namespace detail
