#include "Parser.h"
#include <memory>
#include "Lexer.h"
#include "LexerStream.h"

Parser::Parser(const std::string& code)
    : _lexer(std::make_shared<LexerStringStream>(code)), _current_token(TokenKind::MAX_TOKEN_KIND, ""),
      _last_token(_current_token) {
  auto res = _lexer.next();
  if (res) {
    _current_token = res.result();
  }
}

auto Parser::parse() -> ParserResult {
  ParserResult translation_unit = parse_translation_unit();
  if (translation_unit) {
    return true;
  }
  return false;
}

auto Parser::accept(TokenKind tk) -> bool {
  if (_current_token == tk) {
    std::cout << "Consume:" << _current_token.value() << std::endl;
    _last_token = _current_token;
    _current_token = _lexer.next().result();
    return true;
  }
  return false;
}

auto Parser::parse_translation_unit() -> ParserResult {
  ParserResult node = parse_function();

  if (node) {
    return true;
  }
  return false;
}

auto Parser::parse_function() -> ParserResult {
  if (accept(TokenKind::FN)) {
    if (accept(TokenKind::IDENTIFIER)) {
      if (accept(TokenKind::CLAMP_OPEN)) {
        if (accept(TokenKind::CLAMP_CLOSE)) {
          if (accept(TokenKind::ARROW)) {
            ParserResult type = parse_type();
            if (type) {
              if (accept(TokenKind::CURLY_OPEN)) {
                ParserResult statements = parse_statements();
                if (statements) {
                  if (accept(TokenKind::CURLY_CLOSE)) {
                    return true;
                  }
                }
              }
            }
          }
        }
      }
    }
    return false;
  }
  return false;
}

auto Parser::parse_statements() -> ParserResult {
  ParserResult statement = parse_statement();
  if (!statement) {
    return false;
  }
  while (statement) {
    if (!parse_statement()) {
      return true;
    }
  }
  return true;
}

auto Parser::parse_statement() -> ParserResult {
  ParserResult var_declaration = parse_variable_declaration();
  if (var_declaration) {
    return true;
  }
  ParserResult constant_declaration = parse_constant_declaration();
  if (constant_declaration) {
    return true;
  }
  ParserResult loop = parse_loop();
  if (loop) {
    return true;
  }
  ParserResult return_statement = parse_return_statement();
  if (return_statement) {
    return true;
  }
  return false;
}

auto Parser::parse_variable_declaration() -> ParserResult {
  if (accept(TokenKind::LET)) {
    if (accept(TokenKind::IDENTIFIER)) {
      if (accept(TokenKind::COLON)) {
        ParserResult type = parse_type();
        if (type) {
          if (accept(TokenKind::OP_SET)) {
            ParserResult expression = parse_expression();
            if (expression) {
              if (accept(TokenKind::SEMICOLON)) {
                return true;
              }
            }
          }
        }
      }
    }
  }
  return false;
}

auto Parser::parse_constant_declaration() -> ParserResult {
  if (accept(TokenKind::CONST)) {
    if (accept(TokenKind::IDENTIFIER)) {
      if (accept(TokenKind::COLON)) {
        ParserResult type = parse_type();
        if (type) {
          if (accept(TokenKind::OP_SET)) {
            ParserResult expression = parse_expression();
            if (expression) {
              if (accept(TokenKind::SEMICOLON)) {
                return true;
              }
            }
          }
        }
      }
    }
  }
  return false;
}

auto Parser::parse_loop() -> ParserResult {
  if (accept(TokenKind::WHILE)) {
    if (accept(TokenKind::CLAMP_OPEN)) {
      ParserResult condition = parse_condition();
      if (condition) {
        if (accept(TokenKind::CLAMP_CLOSE)) {
          if (accept(TokenKind::CURLY_OPEN)) {
            ParserResult statements = parse_statements();
            if (statements) {
              if (accept(TokenKind::CURLY_CLOSE)) {
                return true;
              }
            }
          }
        }
      }
    }
  }
  return false;
}

auto Parser::parse_return_statement() -> ParserResult {
  if (accept(TokenKind::RETURN)) {
    ParserResult expression = parse_expression();
    if (expression) {
      if (accept(TokenKind::SEMICOLON)) {
        return true;
      }
    }
  }
  return false;
}

auto Parser::parse_expression() -> ParserResult {
  if (accept(TokenKind::DOUBLE)) {
    return true;
  }
  if (accept(TokenKind::INTEGER)) {
    return true;
  }
  if (accept(TokenKind::TEXT)) {
    return true;
  }
  ParserResult array_initialization = parse_array_initialization();
  if (array_initialization) {
    return true;
  }
  ParserResult binary_expression = parse_binary_expression();
  if (binary_expression) {
    return true;
  }

  return false;
}

auto Parser::parse_array_initialization() -> ParserResult {
  if (accept(TokenKind::EDGE_CLAMP_OPEN)) {
    ParserResult expression_left = parse_expression();
    if (expression_left) {
      if (accept(TokenKind::SEMICOLON)) {
        ParserResult expression_right = parse_expression();
        if (expression_right) {
          if (accept(TokenKind::EDGE_CLAMP_CLOSE)) {
            return true;
          }
        }
      }
    }
  }
  return false;
}

auto Parser::parse_binary_expression() -> ParserResult {
  if (accept(TokenKind::IDENTIFIER)) {
    ParserResult op = parse_operator();
    if (op) {
      ParserResult expression = parse_expression();
      if (expression) {
        return true;
      }
    }
  }
  return false;
}

auto Parser::parse_condition() -> ParserResult {
  ParserResult bin_op = parse_binary_expression();
  if (bin_op) {
    return true;
  }
  return false;
}

auto Parser::parse_operator() -> ParserResult {
  if (accept(TokenKind::OP_LS)) {
    return true;
  }
  if (accept(TokenKind::OP_EQ)) {
    return true;
  }
  if (accept(TokenKind::OP_ADD)) {
    return true;
  }
  return false;
}

auto Parser::parse_type() -> ParserResult {
  if (accept(TokenKind::I32)) {
    return true;
  }
  if (accept(TokenKind::IDENTIFIER)) {
    return true;
  }
  return false;
}
