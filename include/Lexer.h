#ifndef PALLADIUM_LEXER_H_
#define PALLADIUM_LEXER_H_
#include "LexerStream.h"
#include "Util.h"
#include <memory>
#include <string>
enum class TokenKind {
  // Literals
  IDENTIFIER = 0,
  TEXT,
  INTEGER,
  DOUBLE,

  // Operators
  OP_ADD,   // +
  OP_SUB,   // -
  OP_MULT,  // *
  OP_DIV,   // /
  OP_SET,   // =
  OP_EQ,    // ==
  OP_NEQ,   // !=
  OP_NOT,   // !
  OP_LS,    // <
  OP_LS_EQ, // <=
  OP_GT,    // >
  OP_GT_EQ, // >=

  // Groups
  EDGE_CLAMP_OPEN,  // [
  EDGE_CLAMP_CLOSE, // [
  CLAMP_OPEN,       // (
  CLAMP_CLOSE,      // )
  CURLY_OPEN,       // {
  CURLY_CLOSE,      // }

  // Command symbols
  SEMICOLON, // ;
  ARROW,     // ->
  COMMA,     // ,

  // KEYWORDS
  FN,     // fn
  LET,    // let
  CONST,  // const
  I32,    // i32
  VOID,   // void
  RETURN, // return
  WHILE,  // while

  // secial tokens
  END_OF_FILE,
  MAX_TOKEN_KIND,
};

auto operator<<(std::ostream &os, const TokenKind tk) -> std::ostream &;

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

  friend auto operator<<(std::ostream &os, const Token &token)
      -> std::ostream &;

private:
  TokenKind _kind;
  std::string _value;
};

class Lexer final {
public:
  Lexer(const std::shared_ptr<LexerStream> &stream);

  auto next() -> ResultOr<Token>;

private:
  std::shared_ptr<LexerStream> _stream;
  std::size_t _pos;
};

#endif
