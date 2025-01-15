#include "Parser.h"
#include <cmath>
#include <memory>
#include <optional>
#include <variant>
#include "ArrayInitializationNode.h"
#include "BinaryExpressionNode.h"
#include "ConditionNode.h"
#include "ConstantDeclarationNode.h"
#include "ExpressionNode.h"
#include "Lexer.h"
#include "LexerStream.h"
#include "LoopNode.h"
#include "OperatorNode.h"
#include "ReturnStatementNode.h"
#include "StatementNode.h"
#include "StatementsNode.h"
#include "TranslationUnitNode.h"
#include "TypeNode.h"
#include "VariableDeclarationNode.h"
#include "FunctionNode.h"

auto missing(TokenKind kind) -> Error {
  return err("Missing " + detail::to_string(kind));
}

auto is_produced(const ParserResult& res) -> bool {
  if (res.ok()) {
    if (res.result()) {
      return true;
    }
  }
  return false;
}

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
    return translation_unit;
  }
  return translation_unit.error_value();
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

auto Parser::sequence(const std::vector<TK>& tokens)
    -> std::pair<std::optional<ExpectResult>, std::vector<ExpectParserResult>> {

  std::vector<ExpectParserResult> parse_results;
  for (std::size_t i = 0; i < tokens.size(); ++i) {
    if (std::holds_alternative<TokenKind>(tokens[i])) {
      auto token = std::get<TokenKind>(tokens[i]);
      if (!accept(token)) {
        return {token, parse_results};
      }
      parse_results.emplace_back(_last_token.value());
    } else {
      auto pfunc = std::get<PARSE_FUNC>(tokens[i]);
      auto res = pfunc.parse_func();
      parse_results.push_back(res);
      if (!res.ok()) {
        return {res, parse_results};
      }
      if (!res.result() && !pfunc.ALLOW_EPSILON) {
        return {pfunc.err, parse_results};
      }
    }
  }
  return {{}, parse_results};
}

// translation_unit := (function)*
auto Parser::parse_translation_unit() -> ParserResult {
  _context.push({.context = "tranlsation unit", .rule = RuleType::TRANSLATION_UNIT});
  ParserResult node = parse_function();
  if (!node.ok()) {
    return node.error_value();
  }
  std::vector<AstPtr> nodes;
  while (is_produced(node)) {
    nodes.push_back(node.result());
    node = parse_function();
    if (!node.ok()) {
      return node.error_value();
    }
    if (!node.result()) {
      break;
    }
  }
  return {std::make_shared<TranslationUnitNode>(nodes)};
}
// function ::= "fn" identifier "(" ")" "->" type "{" statements "}"
auto Parser::parse_function() -> ParserResult {
  auto type_p = [&]() -> ParserResult { return parse_type(); };
  auto statements_p = [&]() -> ParserResult { return parse_statements(); };

  if (!accept(TokenKind::FN)) {
    return Epsilon;
  }
  if (!accept(TokenKind::IDENTIFIER)) {
    return missing(TokenKind::IDENTIFIER);
  }
  std::string fname = _last_token.value();
  _context.push({.context = fname, .rule = RuleType::FUNCTION});

  auto [expect_res, epr] = sequence({TokenKind::CLAMP_OPEN, TokenKind::CLAMP_CLOSE, TokenKind::ARROW,
                                     PARSE_FUNC(type_p, err("Missing Type")), TokenKind::CURLY_OPEN,
                                     PARSE_FUNC(statements_p), TokenKind::CURLY_CLOSE});

  if (!expect_res) {
    _context.pop();
    auto type = std::get<ParserResult>(epr[3]);
    auto statements = std::get<ParserResult>(epr[5]);
    return {std::make_shared<FunctionNode>(fname, type.result(), statements.result())};
  }

  if (std::holds_alternative<TokenKind>(*expect_res)) {
    return missing(std::get<TokenKind>(*expect_res));
  } else {
    return std::get<ParserResult>(*expect_res).error_value();
  }
}

auto Parser::parse_statements() -> ParserResult {
  _context.push({.context = "block", .rule = RuleType::BLOCK});
  std::vector<AstPtr> statements;
  ParserResult statement = parse_statement();
  if (!statement.ok()) {
    return statement.error_value();
  }
  while (is_produced(statement)) {
    statements.push_back(statement.result());
    statement = parse_statement();
    if (!statement.ok()) {
      return statement.error_value();
    }
    if (!statement.result()) {
      break; // Epsiolon production
    }
  }
  _context.pop();
  return {std::make_shared<StatementsNode>(statements)};
}

auto Parser::parse_statement() -> ParserResult {
  _context.push({.context = "statement", .rule = RuleType::STATEMENT});
  ParserResult var_declaration = parse_variable_declaration();
  if (is_produced(var_declaration)) {
    _context.pop();
    return {std::make_shared<StatementNode>(var_declaration.result(), StatementType::VAR_DEC)};
  }
  if (!var_declaration.ok()) {
    return var_declaration.error_value();
  }

  ParserResult constant_declaration = parse_constant_declaration();
  if (is_produced(constant_declaration)) {
    _context.pop();
    return {std::make_shared<StatementNode>(constant_declaration.result(), StatementType::CONST_DEC)};
  }
  if (!constant_declaration.ok()) {
    return constant_declaration.error_value();
  }

  ParserResult loop = parse_loop();
  if (is_produced(loop)) {
    _context.pop();
    return {std::make_shared<StatementNode>(loop.result(), StatementType::LOOP)};
  }

  if (!loop.ok()) {
    return loop.error_value();
  }

  ParserResult return_statement = parse_return_statement();
  if (is_produced(return_statement)) {
    _context.pop();
    return {std::make_shared<StatementNode>(return_statement.result(), StatementType::RETURN_STATEMENT)};
  }
  if (!return_statement.ok()) {
    return return_statement.error_value();
  }
  ParserResult expression = parse_expression();
  if (is_produced(expression)) {
    if (accept(TokenKind::SEMICOLON)) {
      _context.pop();
      return {std::make_shared<StatementNode>(expression.result(), StatementType::EXPRESSION)};
    }
    return missing(TokenKind::SEMICOLON);
  }
  if (!expression.ok()) {
    return expression.error_value();
  }

  _context.pop();
  return Epsilon;
}

auto Parser::parse_variable_declaration() -> ParserResult {
  auto type_p = [&]() -> ParserResult { return parse_type(); };
  auto expression_p = [&]() -> ParserResult { return parse_expression(); };
  if (accept(TokenKind::LET)) {
    if (!accept(TokenKind::IDENTIFIER)) {
      return missing(TokenKind::IDENTIFIER);
    }
    std::string var_name = _last_token.value();
    _context.push({.context = var_name, .rule = RuleType::VAR_DEC});

    auto [expect_res, epr] = sequence({TokenKind::COLON, PARSE_FUNC(type_p, err("Missing type")), TokenKind::OP_SET,
                                       PARSE_FUNC(expression_p, err("Missing expression")), TokenKind::SEMICOLON});

    if (!expect_res) {
      _context.pop();
      auto type = std::get<ParserResult>(epr[1]);
      auto exp = std::get<ParserResult>(epr[3]);
      return {std::make_shared<VariableDeclarationNode>(var_name, type.result(), exp.result())};
    }
    if (std::holds_alternative<TokenKind>(*expect_res)) {
      return missing(std::get<TokenKind>(*expect_res));
    } else {
      return std::get<ParserResult>(*expect_res).error_value();
    }
  }
  return Epsilon;
}

auto Parser::parse_constant_declaration() -> ParserResult {
  auto type_p = [&]() -> ParserResult { return parse_type(); };
  auto expression_p = [&]() -> ParserResult { return parse_expression(); };
  if (accept(TokenKind::CONST)) {
    if (!accept(TokenKind::IDENTIFIER)) {
      return missing(TokenKind::IDENTIFIER);
    }
    std::string var_name = _last_token.value();
    _context.push({.context = var_name, .rule = RuleType::VAR_DEC});

    auto [expect_res, epr] = sequence({TokenKind::COLON, PARSE_FUNC(type_p, err("Missing type")), TokenKind::OP_SET,
                                       PARSE_FUNC(expression_p, err("Missing expression")), TokenKind::SEMICOLON});

    if (!expect_res) {
      _context.pop();
      auto type = std::get<ParserResult>(epr[1]);
      auto exp = std::get<ParserResult>(epr[3]);
      return {std::make_shared<VariableDeclarationNode>(var_name, type.result(), exp.result())};
    }
    if (std::holds_alternative<TokenKind>(*expect_res)) {
      return missing(std::get<TokenKind>(*expect_res));
    } else {
      return std::get<ParserResult>(*expect_res).error_value();
    }
  }
  return Epsilon;
}

auto Parser::parse_loop() -> ParserResult {
  auto condition_p = [&]() -> ParserResult { return parse_condition(); };
  auto statements_p = [&]() -> ParserResult { return parse_statements(); };

  if (accept(TokenKind::WHILE)) {
    _context.push({.context = "while", .rule = RuleType::LOOP});
    auto [expect_res, epr] =
        sequence({TokenKind::CLAMP_OPEN, PARSE_FUNC(condition_p, err("Missing condition")), TokenKind::CLAMP_CLOSE,
                  TokenKind::CURLY_OPEN, PARSE_FUNC(statements_p), TokenKind::CURLY_CLOSE});

    if (!expect_res) {
      _context.pop();
      auto condition = std::get<ParserResult>(epr[1]);
      auto statements = std::get<ParserResult>(epr[4]);
      return {std::make_shared<LoopNode>(condition.result(), statements.result())};
    }

    if (std::holds_alternative<TokenKind>(*expect_res)) {
      return missing(std::get<TokenKind>(*expect_res));
    } else {
      return std::get<ParserResult>(*expect_res).error_value();
    }
  }

  return Epsilon;
}

auto Parser::parse_return_statement() -> ParserResult {
  if (accept(TokenKind::RETURN)) {
    _context.push({.context = "return", .rule = RuleType::RETURN_STATEMENT});
    ParserResult expression = parse_expression();
    if (expression) {
      if (!accept(TokenKind::SEMICOLON)) {
        return missing(TokenKind::SEMICOLON);
      }
      _context.pop();
      return {std::make_shared<ReturnStatementNode>(expression.result())};
    }
    return expression.error_value();
  }
  return Epsilon;
}

auto Parser::parse_expression() -> ParserResult {
  _context.push({.context = "expression", .rule = RuleType::EXPRESSION});
  if (accept(TokenKind::DOUBLE)) {
    _context.pop();
    return {std::make_shared<ExpressionNode>(_last_token.value(), ExpressionKind::CONST_DOUBLE)};
  }
  if (accept(TokenKind::INTEGER)) {
    _context.pop();
    return {std::make_shared<ExpressionNode>(_last_token.value(), ExpressionKind::CONST_INT)};
  }
  if (accept(TokenKind::TEXT)) {
    _context.pop();
    return {std::make_shared<ExpressionNode>(_last_token.value(), ExpressionKind::CONST_TEXT)};
  }
  ParserResult array_initialization = parse_array_initialization();
  if (is_produced(array_initialization)) {
    _context.pop();
    return {std::make_shared<ExpressionNode>(array_initialization.result(), ExpressionKind::ARRAY_INIT)};
  }
  if (!array_initialization.ok()) {
    return array_initialization.error_value();
  }

  ParserResult binary_expression = parse_binary_expression();
  if (is_produced(binary_expression)) {
    _context.pop();
    return {std::make_shared<ExpressionNode>(binary_expression.result(), ExpressionKind::BIN_OP)};
  }
  if (!binary_expression.ok()) {
    return binary_expression.error_value();
  }
  return Epsilon;
}

auto Parser::parse_array_initialization() -> ParserResult {
  if (accept(TokenKind::EDGE_CLAMP_OPEN)) {
    _context.push({.context = "array initialization", .rule = RuleType::ARRAY_INIT});
    ParserResult expression_left = parse_expression();
    if (!expression_left.ok()) {
      return expression_left.error_value();
    }
    if (!is_produced(expression_left)) {
      return err("Missing expression");
    }
    if (!accept(TokenKind::SEMICOLON)) {
      return missing(TokenKind::SEMICOLON);
    }
    ParserResult expression_right = parse_expression();
    if (!expression_right.ok()) {
      return expression_right.error_value();
    }
    if (!is_produced(expression_right)) {
      return err("Missing expression");
    }
    if (!accept(TokenKind::EDGE_CLAMP_CLOSE)) {
      return missing(TokenKind::EDGE_CLAMP_CLOSE);
    }
    _context.pop();
    return {std::make_shared<ArrayInitializationNode>(expression_left.result(), expression_right.result())};
  }
  return Epsilon;
}

auto Parser::parse_binary_expression() -> ParserResult {
  if (accept(TokenKind::IDENTIFIER)) {
    std::string identfier = _last_token.value();
    _context.push({.context = "identfier", .rule = RuleType::BIN_OP});
    ParserResult op = parse_operator();
    if (!is_produced(op)) {
      if (op.ok()) {
        return {std::make_shared<BinaryExpressionNode>(identfier)};
      }
      return op.error_value();
    }

    ParserResult expression = parse_expression();
    if (!is_produced(expression)) {
      if (expression.ok()) {
        return err("Missing expression");
      }
      return expression.error_value();
    }
    _context.pop();
    return {std::make_shared<BinaryExpressionNode>(identfier, op.result(), expression.result())};
  }
  return Epsilon;
}

auto Parser::parse_condition() -> ParserResult {
  _context.push({.context = "condition", .rule = RuleType::CONDITION});
  ParserResult bin_op = parse_binary_expression();
  if (is_produced(bin_op)) {
    _context.pop();
    return {std::make_shared<ConditionNode>(bin_op.result())};
  }
  if (!bin_op.ok()) {
    return bin_op.error_value();
  }
  return Epsilon;
}

auto Parser::parse_operator() -> ParserResult {
  if (accept(TokenKind::OP_LS)) {
    return {std::make_shared<OperatorNode>(OperatorKind::OP_LS)};
  }
  if (accept(TokenKind::OP_EQ)) {
    return {std::make_shared<OperatorNode>(OperatorKind::OP_EQ)};
  }
  if (accept(TokenKind::OP_SET)) {
    return {std::make_shared<OperatorNode>(OperatorKind::OP_SET)};
  }
  if (accept(TokenKind::OP_ADD)) {
    return {std::make_shared<OperatorNode>(OperatorKind::OP_ADD)};
  }
  return Epsilon;
}

auto Parser::parse_type() -> ParserResult {
  if (accept(TokenKind::I32)) {
    return {std::make_shared<TypeNode>("i32", TypeKind::BUILD_IN_I32)};
  }
  if (accept(TokenKind::IDENTIFIER)) {
    std::string identfier = _last_token.value();
    return {std::make_shared<TypeNode>(identfier, TypeKind::USER_IDENTIFIER)};
  }
  return Epsilon;
}
