#include "Lexer.h"
#include "Util.h"
#include <string>
#include <unordered_map>
namespace detail {

auto to_string(TokenKind tk) -> std::string {
  static const char *converter[] = {
      "IDENTIFIER", "TEXT",   "INTEGER", "DOUBLE", "OP_ADD", "OP_SUB",
      "OP_MULT",    "OP_DIV", "OP_SET",  "OP_EQ",  "OP_NEQ", "END_OF_FILE"};

  if (static_cast<std::size_t>(tk) < std::size(converter)) {
    return converter[static_cast<std::size_t>(tk)];
  }
  return "Token type not in map";
}

} // namespace detail
