#ifndef PALLADIUM_LEXER_H_
#define PALLADIUM_LEXER_H_
#include "LexerStream.h"
#include "Util.h"
#include <cstddef>
#include <deque>
#include <memory>
#include <string>
#include <vector>

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
  Token(const TokenKind &tk, std::string value, std::size_t line = 1,
        std::size_t pos = 0)
      : _kind(tk), _value(std::move(value)), _line(line), _pos(pos) {}

  auto operator==(const TokenKind &rhs) -> bool { return _kind == rhs; }

  auto kind() const -> TokenKind { return _kind; }
  auto value() const -> const std::string & { return _value; }
  auto line() const -> std::size_t { return _line; }
  auto pos() const -> std::size_t { return _pos; }

  friend auto operator<<(std::ostream &os, const Token &token)
      -> std::ostream &;

private:
  TokenKind _kind;
  std::string _value;
  std::size_t _line;
  std::size_t _pos;
};

using LexStreamPtr = std::shared_ptr<LexerStream>;
using OptChar = std::optional<char>;
using OptResult = std::optional<ResultOr<Token>>;

class Lexer final {
public:
  Lexer(const LexStreamPtr &stream);

  auto next() -> ResultOr<Token>;

  template <typename... ARG>
  auto lookahead(const Token &tk, ARG &&...arg) -> bool;

private:
  auto lex_operator(char c) -> OptResult;
  auto lex_identifier_and_keyword(char c) -> OptResult;
  auto lex_text(char c) -> OptResult;
  auto lex_hex_number(OptChar &opt_c, std::string &value) -> OptResult;
  auto lex_bin_number(OptChar &opt_c, std::string &value) -> OptResult;
  auto lex_float_number(OptChar &opt_c, std::string &value) -> OptResult;
  auto lex_number(char c) -> OptResult;

private:
  std::shared_ptr<LexerStream> _stream;
  std::vector<ResultOr<Token>> _buffer;
  bool _lookahead;
  std::size_t _line;
  std::size_t _pos;
};

template <typename... ARG>
auto Lexer::lookahead(const Token &tk, ARG &&...arg) -> bool {
  Switch<bool> lookahead_switch(_lookahead, true, false);
  std::deque<Token> rules = {tk, std::forward<ARG>(arg)...};
  if (_buffer.size() < rules.size()) {
    std::size_t max_token_read = rules.size() - _buffer.size();
    do {
      auto next_token = next();
      max_token_read--;
      if (!next_token) {
        return false;
      }
      _buffer.push_back(next_token);
    } while (max_token_read != 0);
  }
  if (_buffer.size() >= rules.size()) {
    int index = 0;
    for (auto rule : rules) {
      auto res = _buffer[index]
                     .map([&](const Token &tok) -> bool {
                       return tok.kind() == rule.kind();
                     })
                     .result_or(false);
      if (!res) {
        return false;
      }
      index++;
    }
    return true;
  }
  return false;
}
#endif
