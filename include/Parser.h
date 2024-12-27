#ifndef PALLADIUM_PARSER_H
#define PALLADIUM_PARSER_H
#include <optional>
#include "Lexer.h"
#include <string>

using AstNode = bool;

class Parser {
public:
  Parser(const std::string& code);

  auto parse() -> bool;

private:
  auto parse_translation_unit() -> AstNode;
  auto parse_function() -> AstNode;
  auto parse_statements() -> AstNode;
  auto parse_statement() -> AstNode;
  auto parse_variable_declaration() -> AstNode;
  auto parse_constant_declaration() -> AstNode;
  auto parse_loop() -> AstNode;
  auto parse_return_statement() -> AstNode;
  auto parse_expression() -> AstNode;
  auto parse_array_initialization() -> AstNode;
  auto parse_binary_expression() -> AstNode;
  auto parse_condition() -> AstNode;
  auto parse_operator() -> AstNode;
  auto parse_type() -> AstNode;

  auto accept(TokenKind tk) -> bool;

private:
  Lexer _lexer;
  Token _current_token;
  Token _last_token;
};

#endif
