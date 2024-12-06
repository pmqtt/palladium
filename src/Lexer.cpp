#include "Lexer.h"
#include "Util.h"
#include <cctype>
#include <iostream>
#include <optional>
#include <string>
#include <unordered_map>
#define RETURN_IF_VALID(F)                                                     \
  do {                                                                         \
    auto res = F;                                                              \
    if (res) {                                                                 \
      auto tokenRes = *res;                                                    \
      return tokenRes.map([&](const Token &tok) {                              \
        return Token(tok.kind(), tok.value(), _line, _pos);                    \
      });                                                                      \
    };                                                                         \
  } while (0);

#define ERROR_IF_CHAR(X, F, TEXT)                                              \
  do {                                                                         \
    if (X && F(*X)) {                                                          \
      return err(TEXT);                                                        \
    }                                                                          \
  } while (0);

#define ERROR_IF_CHAR_EQ(X, EXP, TEXT)                                         \
  do {                                                                         \
    if (X && *X == EXP) {                                                      \
      return err(TEXT);                                                        \
    }                                                                          \
  } while (0);

namespace detail {

auto is_digit = [](char x) { return std::isdigit(x); };
auto is_xdigit = [](char x) { return std::isxdigit(x); };

auto to_string(TokenKind tk) -> std::string {
  static const char *converter[] = {"IDENTIFIER",
                                    "TEXT",
                                    "INTEGER",
                                    "DOUBLE",
                                    "OP_ADD",
                                    "OP_SUB",
                                    "OP_MULT",
                                    "OP_DIV",
                                    "OP_SET",
                                    "OP_EQ",
                                    "OP_NEQ",
                                    "OP_NOT",
                                    "OP_LS",
                                    "OP_LS_EQ",
                                    "OP_GT",
                                    "OP_GT_EQ",
                                    "EDGE_CLAMP_OPEN",
                                    "EDGE_CLAMP_CLOSE",
                                    "CLAMP_OPEN",
                                    "CLAMP_CLOSE",
                                    "CURLY_OPEN",
                                    "CURLY_CLOSE",
                                    "SEMICOLON",
                                    "ARROW",
                                    "COMMA",
                                    "FN",
                                    "LET",
                                    "CONST",
                                    "I32",
                                    "VOID",
                                    "RETURN",
                                    "WHILE",
                                    "END_OF_FILE"};

  std::cout << "INT TK: " << static_cast<std::size_t>(tk) << " : "
            << std::size(converter) << std::endl;
  if (static_cast<std::size_t>(tk) < std::size(converter)) {
    std::cout << "A" << std::endl;
    return converter[static_cast<std::size_t>(tk)];
  }
  std::cout << "B" << std::endl;
  return "Token type not in map";
}

static const std::unordered_map<std::string, Token> KEYWORDS = {
    {"fn", Token(TokenKind::FN, "fn")},
    {"let", Token(TokenKind::LET, "let")},
    {"const", Token(TokenKind::CONST, "const")},
    {"i32", Token(TokenKind::I32, "i32")},
    {"void", Token(TokenKind::VOID, "void")},
    {"return", Token(TokenKind::RETURN, "return")},
    {"while", Token(TokenKind::WHILE, "while")},
};

static const std::unordered_map<char, Token> SIMPLE_CHAR_TO_TOKEN = {
    {'+', Token(TokenKind::OP_ADD, "+")},
    {'-', Token(TokenKind::OP_SUB, "-")},
    {'*', Token(TokenKind::OP_MULT, "*")},
    {'/', Token(TokenKind::OP_DIV, "/")},
    {'=', Token(TokenKind::OP_SET, "=")},
    {'!', Token(TokenKind::OP_NOT, "!")},
    {'<', Token(TokenKind::OP_LS, "<")},
    {'>', Token(TokenKind::OP_GT, ">")},
    {'[', Token(TokenKind::EDGE_CLAMP_OPEN, "[")},
    {']', Token(TokenKind::EDGE_CLAMP_CLOSE, "]")},
    {'(', Token(TokenKind::CLAMP_OPEN, "(")},
    {')', Token(TokenKind::CLAMP_CLOSE, ")")},
    {'{', Token(TokenKind::CURLY_OPEN, "{")},
    {'}', Token(TokenKind::CURLY_CLOSE, "}")},
    {';', Token(TokenKind::SEMICOLON, ";")},
    {',', Token(TokenKind::COMMA, ",")},
};

static const std::unordered_map<char, std::unordered_map<char, Token>>
    OPERATOR_MULTI_TRANSITION = {
        {'=', {{'=', Token(TokenKind::OP_EQ, "==")}}},
        {'!', {{'=', Token(TokenKind::OP_NEQ, "!=")}}},
        {'<', {{'=', Token(TokenKind::OP_LS_EQ, "<=")}}},
        {'>', {{'=', Token(TokenKind::OP_GT_EQ, ">=")}}},
        {'-', {{'>', Token(TokenKind::ARROW, "->")}}},
};

template <typename... ARG>
auto is_one_of(const std::optional<char> &opt, char c, ARG... args) -> bool {
  if (!opt) {
    return false;
  }
  const std::vector<char> test_cases = {c, args...};
  for (char iter : test_cases) {
    if (*opt == iter) {
      return true;
    }
  }
  return false;
}
template <class F>
auto read_while(LexStreamPtr &stream, std::string &value, const F &func)
    -> OptChar {
  auto opt_c = stream->next();
  while (opt_c && func(*opt_c)) {
    value += *opt_c;
    opt_c = stream->next();
  }
  return opt_c;
}

auto read_while_digit(LexStreamPtr &stream, std::string &value) -> OptChar {
  return read_while(stream, value, detail::is_digit);
}

auto read_while_xdigit(LexStreamPtr &stream, std::string &value) -> OptChar {
  return read_while(stream, value, detail::is_xdigit);
}
template <typename... ARG>
auto read_while_is_one_of(LexStreamPtr &stream, std::string &value, char c,
                          ARG... args) -> OptChar {
  auto opt_c = stream->next();
  while (opt_c && detail::is_one_of(opt_c, c, args...)) {
    value += *opt_c;
    opt_c = stream->next();
  }
  return opt_c;
}

} // namespace detail

auto operator<<(std::ostream &os, const Token &token) -> std::ostream & {
  os << "( kind = " << detail::to_string(token._kind);
  os << ", value = \"" << token._value << "\" )";
  return os;
}

auto operator<<(std::ostream &os, const TokenKind tk) -> std::ostream & {
  os << detail::to_string(tk);
  return os;
}

auto Lexer::lex_operator(char c) -> OptResult {
  auto it = detail::SIMPLE_CHAR_TO_TOKEN.find(c);
  if (it != detail::SIMPLE_CHAR_TO_TOKEN.cend()) {
    auto iter = detail::OPERATOR_MULTI_TRANSITION.find(c);
    if (iter != detail::OPERATOR_MULTI_TRANSITION.cend()) {
      const std::optional<char> opt_c = _stream->next();
      if (opt_c) {
        auto iter2 = iter->second.find(*opt_c);
        if (iter2 != iter->second.cend()) {
          _pos += iter2->second.value().size();
          return iter2->second;
        }
      }
      _stream->prev();
    }
    _pos++;
    return it->second;
  }
  return {};
}

auto Lexer::lex_identifier_and_keyword(char c) -> OptResult {
  if (c == '_' || std::isalpha(c)) {
    std::string value;
    value += c;
    detail::read_while(_stream, value,
                       [](char x) { return x == '_' || std::isalnum(x); });

    _pos += value.size();
    _stream->prev();
    auto it = detail::KEYWORDS.find(value);
    if (it != detail::KEYWORDS.cend()) {
      return it->second;
    }
    return Token(TokenKind::IDENTIFIER, value);
  }
  return {};
}

auto Lexer::lex_text(char c) -> OptResult {
  if (c == '"') {
    std::string value;
    OptChar opt_c = _stream->next();
    while (opt_c && *opt_c != '"') {
      if (*opt_c == '\\') {
        opt_c = _stream->next();
        if (opt_c) {
          value += *opt_c;
          opt_c = _stream->next();
          continue;
        }
        return err("Invalid escape sequence in string literal");
      }
      value += *opt_c;
      opt_c = _stream->next();
    }
    if (!opt_c) {
      return err("Unterminated string literal");
    }
    _pos += value.size();
    return Token(TokenKind::TEXT, value);
  }
  return {};
}
auto Lexer::lex_hex_number(OptChar &opt_c, std::string &value) -> OptResult {
  if (detail::is_one_of(opt_c, 'x', 'x')) {
    value += *opt_c;
    opt_c = detail::read_while_xdigit(_stream, value);
    ERROR_IF_CHAR(opt_c, std::isalpha, "Invalid hex number");
    _stream->prev();
    _pos += value.size();
    return Token(TokenKind::INTEGER, value);
  }
  return {};
}

auto Lexer::lex_bin_number(OptChar &opt_c, std::string &value) -> OptResult {
  if (detail::is_one_of(opt_c, 'b', 'B')) {
    value += *opt_c;
    opt_c = detail::read_while_is_one_of(_stream, value, '0', '1');
    ERROR_IF_CHAR(opt_c, std::isdigit, "Invalid bin number");
    _stream->prev();
    _pos += value.size();
    return Token(TokenKind::INTEGER, value);
  }
  return {};
}

auto Lexer::lex_float_number(OptChar &opt_c, std::string &value) -> OptResult {
  if (*opt_c == '.') {
    value += *opt_c;
    opt_c = detail::read_while_digit(_stream, value);
    ERROR_IF_CHAR_EQ(opt_c, '.', "Invalid floating point number");
    _stream->prev();
    _pos += value.size();
    return Token(TokenKind::DOUBLE, value);
  }

  return {};
}

auto Lexer::lex_number(char c) -> OptResult {
  if (std::isdigit(c)) {
    std::string value;
    if (c == '0') {
      value += c;
      OptChar opt_c = _stream->next();
      if (opt_c) {
        RETURN_IF_VALID(lex_hex_number(opt_c, value));
        RETURN_IF_VALID(lex_bin_number(opt_c, value));
        RETURN_IF_VALID(lex_float_number(opt_c, value));
        ERROR_IF_CHAR(opt_c, std::isdigit, "Invalid integer number");
        _stream->prev();
      }
      _pos += value.size();
      return Token(TokenKind::INTEGER, value);
    }
    value += c;
    auto opt_c = detail::read_while_digit(_stream, value);

    RETURN_IF_VALID(lex_float_number(opt_c, value));
    _stream->prev();
    _pos += value.size();
    return Token(TokenKind::INTEGER, value);
  }
  return {};
}

auto Lexer::next() -> ResultOr<Token> {
  if (!_lookahead) {
    if (_buffer.size() > 0) {
      auto res = _buffer.front();
      _buffer.erase(_buffer.begin());
      return res;
    }
  }
  OptChar opt_c = _stream->next();
  while (opt_c) {
    const char c = *opt_c;
    _pos++;
    if (std::isspace(c)) {
      if (c == '\n') {
        _line++;
        _pos = 0;
      }
      opt_c = _stream->next();
      continue;
    }
    RETURN_IF_VALID(lex_operator(c));
    RETURN_IF_VALID(lex_identifier_and_keyword(c));
    RETURN_IF_VALID(lex_text(c));
    RETURN_IF_VALID(lex_number(c));

    return err(std::string("Invalid character read: ") + c);
  }
  return Token(TokenKind::END_OF_FILE, "EOF");
}

Lexer::Lexer(const std::shared_ptr<LexerStream> &stream)
    : _stream(stream), _buffer(), _lookahead(false), _line(1), _pos(0) {}
