#include "Parser.h"
#include <memory>
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

auto Parser::parse_translation_unit() -> ParserResult {
  ParserResult node = parse_function();
  if (node) {
    return {std::make_shared<TranslationUnitNode>(node.result())};
  }
  return node.error_value();
}

auto Parser::parse_function() -> ParserResult {
  if (accept(TokenKind::FN)) {
    if (accept(TokenKind::IDENTIFIER)) {
      std::string fname = _last_token.value();
      if (accept(TokenKind::CLAMP_OPEN)) {
        if (accept(TokenKind::CLAMP_CLOSE)) {
          if (accept(TokenKind::ARROW)) {
            ParserResult type = parse_type();
            if (type) {
              if (accept(TokenKind::CURLY_OPEN)) {
                ParserResult statements = parse_statements();
                if (statements) {
                  if (accept(TokenKind::CURLY_CLOSE)) {
                    return {std::make_shared<FunctionNode>(fname, type.result(), statements.result())};
                  }
                }
              }
            }
          }
        }
      }
    }
    return err("Missing identfier after fn");
  }
  return err("Missing fn");
}

auto Parser::parse_statements() -> ParserResult {
  std::vector<AstPtr> statements;
  ParserResult statement = parse_statement();
  if (!statement.ok()) {
    return statement.error_value();
  }
  while (statement && statement.result()) {
    statements.push_back(statement.result());
    statement = parse_statement();
    if (!statement.ok()) {
      return statement.error_value();
    }
    if (!statement.result()) {
      break; // Epsiolon production
    }
    statements.push_back(statement.result());
  }
  return {std::make_shared<StatementsNode>(statements)};
}

auto Parser::parse_statement() -> ParserResult {
  ParserResult var_declaration = parse_variable_declaration();
  if (var_declaration && var_declaration.result()) {
    return {std::make_shared<StatementNode>(var_declaration.result(), StatementType::VAR_DEC)};
  }
  if (!var_declaration.ok()) {
    return var_declaration.error_value();
  }

  ParserResult constant_declaration = parse_constant_declaration();
  if (constant_declaration && constant_declaration.result()) {
    return {std::make_shared<StatementNode>(constant_declaration.result(), StatementType::CONST_DEC)};
  }
  if (!constant_declaration.ok()) {
    return constant_declaration.error_value();
  }

  ParserResult loop = parse_loop();
  if (loop && loop.result()) {
    return {std::make_shared<StatementNode>(loop.result(), StatementType::LOOP)};
  }

  if (!loop.ok()) {
    return loop.error_value();
  }

  ParserResult return_statement = parse_return_statement();
  if (return_statement && return_statement.result()) {
    return {std::make_shared<StatementNode>(return_statement.result(), StatementType::RETURN_STATEMENT)};
  }
  if (!return_statement.ok()) {
    return return_statement.error_value();
  }
  return ParserResult(nullptr);
}

auto Parser::parse_variable_declaration() -> ParserResult {
  if (accept(TokenKind::LET)) {
    if (accept(TokenKind::IDENTIFIER)) {
      std::string var_name = _last_token.value();
      if (accept(TokenKind::COLON)) {
        ParserResult type = parse_type();
        if (type) {
          if (accept(TokenKind::OP_SET)) {
            ParserResult expression = parse_expression();
            if (expression) {
              if (accept(TokenKind::SEMICOLON)) {
                return {std::make_shared<VariableDeclarationNode>(var_name, expression.result())};
              }
            }
          }
        }
      }
    }
  }
  return ParserResult(nullptr);
}

auto Parser::parse_constant_declaration() -> ParserResult {
  if (accept(TokenKind::CONST)) {
    if (accept(TokenKind::IDENTIFIER)) {
      std::string var_name = _last_token.value();
      if (accept(TokenKind::COLON)) {
        ParserResult type = parse_type();
        if (type) {
          if (accept(TokenKind::OP_SET)) {
            ParserResult expression = parse_expression();
            if (expression) {
              if (accept(TokenKind::SEMICOLON)) {
                return {std::make_shared<ConstantDeclarationNode>(var_name, expression.result())};
              }
            }
          }
        }
      }
    }
  }
  return ParserResult(nullptr);
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
                return {std::make_shared<LoopNode>(condition.result(), statements.result())};
              }
            }
          }
        }
      }
    }
  }
  return ParserResult(nullptr);
}

auto Parser::parse_return_statement() -> ParserResult {
  if (accept(TokenKind::RETURN)) {
    ParserResult expression = parse_expression();
    if (expression) {
      if (accept(TokenKind::SEMICOLON)) {
        return {std::make_shared<ReturnStatementNode>(expression.result())};
      }
      return err("Missing semicolon after expression");
    }
  }
  return ParserResult(nullptr);
}

auto Parser::parse_expression() -> ParserResult {
  if (accept(TokenKind::DOUBLE)) {
    return {std::make_shared<ExpressionNode>(_last_token.value(), ExpressionKind::CONST_DOUBLE)};
  }
  if (accept(TokenKind::INTEGER)) {
    return {std::make_shared<ExpressionNode>(_last_token.value(), ExpressionKind::CONST_INT)};
  }
  if (accept(TokenKind::TEXT)) {
    return {std::make_shared<ExpressionNode>(_last_token.value(), ExpressionKind::CONST_TEXT)};
  }
  ParserResult array_initialization = parse_array_initialization();
  if (array_initialization) {
    return {std::make_shared<ExpressionNode>(array_initialization.result(), ExpressionKind::ARRAY_INIT)};
  }
  ParserResult binary_expression = parse_binary_expression();
  if (binary_expression) {
    return {std::make_shared<ExpressionNode>(binary_expression.result(), ExpressionKind::BIN_OP)};
  }
  return ParserResult(nullptr);
}

auto Parser::parse_array_initialization() -> ParserResult {
  if (accept(TokenKind::EDGE_CLAMP_OPEN)) {
    ParserResult expression_left = parse_expression();
    if (expression_left) {
      if (accept(TokenKind::SEMICOLON)) {
        ParserResult expression_right = parse_expression();
        if (expression_right) {
          if (accept(TokenKind::EDGE_CLAMP_CLOSE)) {
            return {std::make_shared<ArrayInitializationNode>(expression_left.result(), expression_right.result())};
          }
        }
      }
    }
  }
  return ParserResult(nullptr);
}

auto Parser::parse_binary_expression() -> ParserResult {
  if (accept(TokenKind::IDENTIFIER)) {
    ParserResult op = parse_operator();
    if (op) {
      ParserResult expression = parse_expression();
      if (expression) {
        return {std::make_shared<BinaryExpressionNode>(op.result(), expression.result())};
      }
    }
  }
  return ParserResult(nullptr);
}

auto Parser::parse_condition() -> ParserResult {
  ParserResult bin_op = parse_binary_expression();
  if (bin_op) {
    return {std::make_shared<ConditionNode>(bin_op.result())};
  }
  return ParserResult(nullptr);
}

auto Parser::parse_operator() -> ParserResult {
  if (accept(TokenKind::OP_LS)) {
    return {std::make_shared<OperatorNode>(OperatorKind::OP_LS)};
  }
  if (accept(TokenKind::OP_EQ)) {
    return {std::make_shared<OperatorNode>(OperatorKind::OP_EQ)};
  }
  if (accept(TokenKind::OP_ADD)) {
    return {std::make_shared<OperatorNode>(OperatorKind::OP_ADD)};
  }
  return ParserResult(nullptr);
}

auto Parser::parse_type() -> ParserResult {
  if (accept(TokenKind::I32)) {
    return {std::make_shared<TypeNode>("i32", TypeKind::BUILD_IN_I32)};
  }
  if (accept(TokenKind::IDENTIFIER)) {
    return {std::make_shared<TypeNode>(_last_token.value(), TypeKind::USER_IDENTIFIER)};
  }
  return ParserResult(nullptr);
}
