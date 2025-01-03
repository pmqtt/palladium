#include "Codegeneration.h"
#include <memory>
#include "ExpressionNode.h"
#include "ReturnStatementNode.h"
#include "StatementNode.h"
#include "StatementsNode.h"
#include "TranslationUnitNode.h"
#include "Util.h"

using VM = VirtualMachine<AggresivPolicy>;
TranslationUnitVisitor::TranslationUnitVisitor() : _vm(std::make_shared<VirtualMachine<AggresivPolicy>>()) {
  _vm->add_program({Call<VM>("main"), Halt<VM>()});
}

auto TranslationUnitVisitor::begin(const std::shared_ptr<TranslationUnitNode>& node) -> VisitResult {
  UNUSED(node);
  return true;
}
auto TranslationUnitVisitor::visit(const std::shared_ptr<TranslationUnitNode>& node) -> std::shared_ptr<Visitor> {
  UNUSED(node);
  _func_visitor = std::make_shared<FunctionVisitor>(_vm);
  return _func_visitor;
}
auto TranslationUnitVisitor::end(const std::shared_ptr<TranslationUnitNode>& node) -> VisitResult {
  UNUSED(node);
  return true;
}

//-----------------------------------------------------
auto FunctionVisitor::begin(const std::shared_ptr<FunctionNode>& node) -> VisitResult {
  UNUSED(node);
  return true;
}
auto FunctionVisitor::visit(const std::shared_ptr<FunctionNode>& node) -> std::shared_ptr<Visitor> {
  UNUSED(node);
  _statements_visitor = std::make_shared<StatementsVisitor>();
  return _statements_visitor;
}
auto FunctionVisitor::end(const std::shared_ptr<FunctionNode>& node) -> VisitResult {
  auto fname = node->function_name();
  _vm->add_function(fname, _statements_visitor->block(), 0);
  return true;
}

//-----------------------------------------------------
auto StatementsVisitor::begin(const std::shared_ptr<StatementsNode>& node) -> VisitResult {
  UNUSED(node);
  return true;
}
auto StatementsVisitor::visit(const std::shared_ptr<StatementsNode>& node) -> std::shared_ptr<Visitor> {
  UNUSED(node);
  _statement_visitor = std::make_shared<StatementVisitor>();
  return _statement_visitor;
}
auto StatementsVisitor::end(const std::shared_ptr<StatementsNode>& node) -> VisitResult {
  UNUSED(node);
  _block.insert(_block.end(), _statement_visitor->code().begin(), _statement_visitor->code().end());
  return true;
}

//-----------------------------------------------------
auto StatementVisitor::begin(const std::shared_ptr<StatementNode>& node) -> VisitResult {
  UNUSED(node);
  return true;
}
auto StatementVisitor::visit(const std::shared_ptr<StatementNode>& node) -> std::shared_ptr<Visitor> {
  UNUSED(node);
  _return_statement_visitor = std::make_shared<ReturnStatementVisitor>();
  return _return_statement_visitor;
}
auto StatementVisitor::end(const std::shared_ptr<StatementNode>& node) -> VisitResult {
  UNUSED(node);
  _code.insert(_code.end(), _return_statement_visitor->code().begin(), _return_statement_visitor->code().end());
  return true;
}

//-----------------------------------------------------
auto ReturnStatementVisitor::begin(const std::shared_ptr<ReturnStatementNode>& node) -> VisitResult {
  UNUSED(node);
  return true;
}
auto ReturnStatementVisitor::visit(const std::shared_ptr<ReturnStatementNode>& node) -> std::shared_ptr<Visitor> {
  UNUSED(node);
  _expression_visitor = std::make_shared<ExpressionVisitor>();
  return _expression_visitor;
}
auto ReturnStatementVisitor::end(const std::shared_ptr<ReturnStatementNode>& node) -> VisitResult {
  UNUSED(node);
  _code.insert(_code.end(), _expression_visitor->code().begin(), _expression_visitor->code().end());
  _code.push_back(Return<VM>(0));
  return true;
}

//-----------------------------------------------------
auto ExpressionVisitor::begin(const std::shared_ptr<ExpressionNode>& node) -> VisitResult {
  switch (node->kind()) {
  case ExpressionKind::CONST_INT:
    _code.push_back(CLoad<VM>(std::atoi(node->constante().c_str())));
    break;
  case ExpressionKind::CONST_DOUBLE:
  case ExpressionKind::CONST_TEXT:
  case ExpressionKind::ARRAY_INIT:
  case ExpressionKind::BIN_OP:
    break;
  }

  return true;
}
auto ExpressionVisitor::visit(const std::shared_ptr<ExpressionNode>& node) -> std::shared_ptr<Visitor> {
  UNUSED(node);
  return shared_from_this();
}
auto ExpressionVisitor::end(const std::shared_ptr<ExpressionNode>& node) -> VisitResult {
  UNUSED(node);
  return true;
}
