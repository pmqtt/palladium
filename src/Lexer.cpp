#include "Lexer.h"
#include "Util.h"
#include <cctype>
#include <iostream>
#include <optional>
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

static const std::unordered_map<char, Token> SIMPLE_CHAR_TO_TOKEN = {
    {'+', Token(TokenKind::OP_ADD, "+")},
    {'-', Token(TokenKind::OP_SUB, "-")},
    {'*', Token(TokenKind::OP_MULT, "*")},
    {'/', Token(TokenKind::OP_DIV, "/")},
};

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

Lexer::Lexer(const std::shared_ptr<LexerStream> &stream)
    : _stream(stream), _pos(0) {}

auto Lexer::next() -> ResultOr<Token> {
  std::optional<char> opt_c = _stream->next();
  while (opt_c) {
    const char c = *opt_c;
    auto it = detail::SIMPLE_CHAR_TO_TOKEN.find(c);
    if (it != detail::SIMPLE_CHAR_TO_TOKEN.cend()) {
      return it->second;
    }
    if (c == '=') {
      const std::optional<char> opt_nextC = _stream->next();
      if (opt_nextC && *opt_nextC == '=') {
        return Token(TokenKind::OP_EQ, "==");
      }
      _stream->prev();
      return Token(TokenKind::OP_SET, "=");
    }
    if (c == '_' || std::isalpha(c)) {
      std::string value;
      value += c;
      std::optional<char> opt_nextC = _stream->next();
      while (opt_nextC && (*opt_nextC == '_' || std::isalnum(*opt_nextC))) {
        value += *opt_nextC;
        opt_nextC = _stream->next();
      }
      if (value.size() > 1) {
        _stream->prev();
      }
      return Token(TokenKind::IDENTIFIER, value);
    }
    if (c == '"') {
      std::string value;
      std::optional<char> opt_nextC = _stream->next();
      while (opt_nextC && *opt_nextC != '"') {
        if (*opt_nextC == '\\') {
          opt_nextC = _stream->next();
          if (opt_nextC) {
            value += *opt_nextC;
            opt_nextC = _stream->next();
            continue;
          }
          return err("Invalid escape sequence in string literal");
        }
        value += *opt_nextC;
        opt_nextC = _stream->next();
      }
      if (!opt_nextC) {
        return err("Unterminated string literal");
      }
      return Token(TokenKind::TEXT, value);
    }
    if (std::isdigit(c)) {
      std::string value;
      if (c == '0') {
        value += c;
        std::optional<char> opt_nextC = _stream->next();
        if (opt_nextC) {
          if (*opt_nextC == 'x' || *opt_nextC == 'X') {
            // hex
            value += *opt_nextC;
            opt_nextC = _stream->next();
            while (opt_nextC && std::isxdigit(*opt_nextC)) {
              value += *opt_nextC;
              opt_nextC = _stream->next();
            }
            if (opt_nextC && std::isalpha(*opt_nextC)) {
              return err("Invalid hexadicimal integer number");
            }

            _stream->prev();
            return Token(TokenKind::INTEGER, value);
          }
          if (*opt_nextC == 'b' || *opt_nextC == 'B') {
            // bin
            value += *opt_nextC;
            opt_nextC = _stream->next();
            while (opt_nextC && (*opt_nextC == '0' || *opt_nextC == '1')) {
              value += *opt_nextC;
              opt_nextC = _stream->next();
            }
            if (opt_nextC && std::isdigit(*opt_nextC)) {
              return err("Invalid binary integer number");
            }

            _stream->prev();
            return Token(TokenKind::INTEGER, value);
          }
          // floating point case
          if (opt_nextC && *opt_nextC == '.') {
            // floating number
            value += *opt_nextC;
            opt_nextC = _stream->next();
            while (opt_nextC && std::isdigit(*opt_nextC)) {
              value += *opt_nextC;
              opt_nextC = _stream->next();
            }
            if (opt_nextC && *opt_nextC == '.') {
              return err("Invalid floating point number");
            }
            _stream->prev();
            return Token(TokenKind::DOUBLE, value);
          }
          if (opt_nextC && std::isdigit(*opt_nextC)) {
            return err("Invalid integer number");
          }
          _stream->prev();
        }
        return Token(TokenKind::INTEGER, value);
      }
      value += c;
      std::optional<char> opt_nextC = _stream->next();
      while (opt_nextC && std::isdigit(*opt_nextC)) {
        value += *opt_nextC;
        opt_nextC = _stream->next();
      }
      if (opt_nextC && *opt_nextC == '.') {
        // floating number
        value += *opt_nextC;
        opt_nextC = _stream->next();
        while (opt_nextC && std::isdigit(*opt_nextC)) {
          value += *opt_nextC;
          opt_nextC = _stream->next();
        }
        if (opt_nextC && *opt_nextC == '.') {
          return err("Invalid floating point number");
        }

        _stream->prev();
        return Token(TokenKind::DOUBLE, value);
      }
      _stream->prev();
      return Token(TokenKind::INTEGER, value);
    }
    if (std::isspace(c)) {
      opt_c = _stream->next();
      continue;
    }
  }

  return Token(TokenKind::END_OF_FILE, "EOF");
}
