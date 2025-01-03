#ifndef PALLADIUM_PARSER_H
#define PALLADIUM_PARSER_H
#include "AstNode.h"
#include "Lexer.h"
#include "Util.h"
#include <string>
#include <stack>

auto missing(TokenKind kind) -> Error;

using ParserResult = ResultOr<AstPtr>;
static const ParserResult Epsilon(nullptr);

auto is_produced(const ParserResult& res) -> bool;
enum class RuleType {
  TRANSLATION_UNIT,
  FUNCTION,
  BLOCK,
  STATEMENT,
  VAR_DEC,
  CONST_DEC,
  LOOP,
  RETURN_STATEMENT,
  EXPRESSION,
  ARRAY_INIT,
  BIN_OP,
  CONDITION,
  OPERATOR,
  TYPE
};

struct Context {
  std::string context; // function name, if, while, expression ...
  RuleType rule;
};

class Parser {
public:
  Parser(const std::string& code);

  auto parse() -> ParserResult;

private:
  auto parse_translation_unit() -> ParserResult;
  auto parse_function() -> ParserResult;
  auto parse_statements() -> ParserResult;
  auto parse_statement() -> ParserResult;
  auto parse_variable_declaration() -> ParserResult;
  auto parse_constant_declaration() -> ParserResult;
  auto parse_loop() -> ParserResult;
  auto parse_return_statement() -> ParserResult;
  auto parse_expression() -> ParserResult;
  auto parse_array_initialization() -> ParserResult;
  auto parse_binary_expression() -> ParserResult;
  auto parse_condition() -> ParserResult;
  auto parse_operator() -> ParserResult;
  auto parse_type() -> ParserResult;

  auto accept(TokenKind tk) -> bool;

private:
  Lexer _lexer;
  Token _current_token;
  Token _last_token;
  std::stack<Context> _context;
};

#endif
