#ifndef PALLADIUM_CODEGENERATION_H
#define PALLADIUM_CODEGENERATION_H
#include <memory>
#include "FunctionNode.h"
#include "StatementsNode.h"
#include "VMPolicy.h"
#include "Visitor.h"
#include "VirtualMachine.h"

using VMPtr = std::shared_ptr<VirtualMachine<AggresivPolicy>>;
using Code = std::vector<InstructionType<VirtualMachine<AggresivPolicy>>>;
//------------------FORWARD DECLARATION--------------------
class FunctionVisitor;
class StatementsVisitor;
class StatementVisitor;
class ReturnStatementVisitor;
class ExpressionVisitor;
//---------------------------------------------------------

class TranslationUnitVisitor : public Visitor {
public:
  TranslationUnitVisitor();
  auto begin(const std::shared_ptr<TranslationUnitNode>& node) -> VisitResult override;
  auto visit(const std::shared_ptr<TranslationUnitNode>& node) -> std::shared_ptr<Visitor> override;
  auto end(const std::shared_ptr<TranslationUnitNode>& node) -> VisitResult override;

  using Visitor::begin;
  using Visitor::end;
  using Visitor::visit;

public:
  auto vm() const {
    return _vm;
  }

private:
  VMPtr _vm;
  std::shared_ptr<FunctionVisitor> _func_visitor;
};

//-----------------------------------------------------
class FunctionVisitor : public Visitor {
public:
  FunctionVisitor(const VMPtr& vm) : _vm(vm) {
  }
  auto begin(const std::shared_ptr<FunctionNode>& node) -> VisitResult override;
  auto visit(const std::shared_ptr<FunctionNode>& node) -> std::shared_ptr<Visitor> override;
  auto end(const std::shared_ptr<FunctionNode>& node) -> VisitResult override;

  using Visitor::begin;
  using Visitor::end;
  using Visitor::visit;

private:
  VMPtr _vm;
  std::shared_ptr<StatementsVisitor> _statements_visitor;
};

//-----------------------------------------------------
class StatementsVisitor : public Visitor {
public:
  StatementsVisitor() = default;
  auto begin(const std::shared_ptr<StatementsNode>& node) -> VisitResult override;
  auto visit(const std::shared_ptr<StatementsNode>& node) -> std::shared_ptr<Visitor> override;
  auto end(const std::shared_ptr<StatementsNode>& node) -> VisitResult override;

  using Visitor::begin;
  using Visitor::end;
  using Visitor::visit;

public:
  auto block() const -> const Code& {
    return _block;
  }

private:
  Code _block;
  std::shared_ptr<StatementVisitor> _statement_visitor;
};

//-----------------------------------------------------
class StatementVisitor : public Visitor {
public:
  StatementVisitor() = default;
  auto begin(const std::shared_ptr<StatementNode>& node) -> VisitResult override;
  auto visit(const std::shared_ptr<StatementNode>& node) -> std::shared_ptr<Visitor> override;
  auto end(const std::shared_ptr<StatementNode>& node) -> VisitResult override;

  using Visitor::begin;
  using Visitor::end;
  using Visitor::visit;

public:
  auto code() const -> const Code& {
    return _code;
  }

private:
  Code _code;
  std::shared_ptr<ReturnStatementVisitor> _return_statement_visitor;
};

//-----------------------------------------------------
class ReturnStatementVisitor : public Visitor {
public:
  ReturnStatementVisitor() = default;
  auto begin(const std::shared_ptr<ReturnStatementNode>& node) -> VisitResult override;
  auto visit(const std::shared_ptr<ReturnStatementNode>& node) -> std::shared_ptr<Visitor> override;
  auto end(const std::shared_ptr<ReturnStatementNode>& node) -> VisitResult override;

  using Visitor::begin;
  using Visitor::end;
  using Visitor::visit;

public:
  auto code() const -> const Code& {
    return _code;
  }

private:
  Code _code;
  std::shared_ptr<ExpressionVisitor> _expression_visitor;
};

//-----------------------------------------------------
class ExpressionVisitor : public Visitor {
public:
  ExpressionVisitor() = default;
  auto begin(const std::shared_ptr<ExpressionNode>& node) -> VisitResult override;
  auto visit(const std::shared_ptr<ExpressionNode>& node) -> std::shared_ptr<Visitor> override;
  auto end(const std::shared_ptr<ExpressionNode>& node) -> VisitResult override;

  using Visitor::begin;
  using Visitor::end;
  using Visitor::visit;

public:
  auto code() const -> const Code& {
    return _code;
  }

private:
  Code _code;
};

#endif