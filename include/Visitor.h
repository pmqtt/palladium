#ifndef PALLADIUM_VISITOR_H
#define PALLADIUM_VISITOR_H
#include "Util.h"
#include <memory>

using VisitResult = ResultOr<bool>;
class TranslationUnitNode;
class FunctionNode;
class StatementsNode;
class StatementNode;
class VariableDeclarationNode;
class ConstantDeclarationNode;
class LoopNode;
class ReturnStatementNode;
class ExpressionNode;
class ArrayInitializationNode;
class BinaryExpressionNode;
class ConditionNode;
class OperatorNode;
class TypeNode;

class Visitor : public std::enable_shared_from_this<Visitor> {
public:
  explicit Visitor() = default;
  virtual ~Visitor() = default;

  virtual auto begin(const std::shared_ptr<TranslationUnitNode>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }
  virtual auto visit(const std::shared_ptr<TranslationUnitNode>& node) -> std::shared_ptr<Visitor> {
    UNUSED(node);
    return shared_from_this();
  }
  virtual auto end(const std::shared_ptr<TranslationUnitNode>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }
  virtual auto begin(const std::shared_ptr<FunctionNode>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }
  virtual auto visit(const std::shared_ptr<FunctionNode>& node) -> std::shared_ptr<Visitor> {
    UNUSED(node);
    return shared_from_this();
  }
  virtual auto end(const std::shared_ptr<FunctionNode>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }
  virtual auto begin(const std::shared_ptr<StatementsNode>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }
  virtual auto visit(const std::shared_ptr<StatementsNode>& node) -> std::shared_ptr<Visitor> {
    UNUSED(node);
    return shared_from_this();
  }
  virtual auto end(const std::shared_ptr<StatementsNode>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }
  virtual auto begin(const std::shared_ptr<StatementNode>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }
  virtual auto visit(const std::shared_ptr<StatementNode>& node) -> std::shared_ptr<Visitor> {
    UNUSED(node);
    return shared_from_this();
  }
  virtual auto end(const std::shared_ptr<StatementNode>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }
  virtual auto begin(const std::shared_ptr<VariableDeclarationNode>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }
  virtual auto visit(const std::shared_ptr<VariableDeclarationNode>& node) -> std::shared_ptr<Visitor> {
    UNUSED(node);
    return shared_from_this();
  }
  virtual auto end(const std::shared_ptr<VariableDeclarationNode>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }
  virtual auto begin(const std::shared_ptr<ConstantDeclarationNode>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }
  virtual auto visit(const std::shared_ptr<ConstantDeclarationNode>& node) -> std::shared_ptr<Visitor> {
    UNUSED(node);
    return shared_from_this();
  }
  virtual auto end(const std::shared_ptr<ConstantDeclarationNode>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }
  virtual auto begin(const std::shared_ptr<LoopNode>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }
  virtual auto visit(const std::shared_ptr<LoopNode>& node) -> std::shared_ptr<Visitor> {
    UNUSED(node);
    return shared_from_this();
  }
  virtual auto end(const std::shared_ptr<LoopNode>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }
  virtual auto begin(const std::shared_ptr<ReturnStatementNode>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }
  virtual auto visit(const std::shared_ptr<ReturnStatementNode>& node) -> std::shared_ptr<Visitor> {
    UNUSED(node);
    return shared_from_this();
  }
  virtual auto end(const std::shared_ptr<ReturnStatementNode>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }
  virtual auto begin(const std::shared_ptr<ExpressionNode>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }
  virtual auto visit(const std::shared_ptr<ExpressionNode>& node) -> std::shared_ptr<Visitor> {
    UNUSED(node);
    return shared_from_this();
  }
  virtual auto end(const std::shared_ptr<ExpressionNode>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }
  virtual auto begin(const std::shared_ptr<ArrayInitializationNode>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }
  virtual auto visit(const std::shared_ptr<ArrayInitializationNode>& node) -> std::shared_ptr<Visitor> {
    UNUSED(node);
    return shared_from_this();
  }
  virtual auto end(const std::shared_ptr<ArrayInitializationNode>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }
  virtual auto begin(const std::shared_ptr<BinaryExpressionNode>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }
  virtual auto visit(const std::shared_ptr<BinaryExpressionNode>& node) -> std::shared_ptr<Visitor> {
    UNUSED(node);
    return shared_from_this();
  }
  virtual auto end(const std::shared_ptr<BinaryExpressionNode>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }
  virtual auto begin(const std::shared_ptr<ConditionNode>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }
  virtual auto visit(const std::shared_ptr<ConditionNode>& node) -> std::shared_ptr<Visitor> {
    UNUSED(node);
    return shared_from_this();
  }
  virtual auto end(const std::shared_ptr<ConditionNode>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }
  virtual auto begin(const std::shared_ptr<OperatorNode>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }
  virtual auto visit(const std::shared_ptr<OperatorNode>& node) -> std::shared_ptr<Visitor> {
    UNUSED(node);
    return shared_from_this();
  }
  virtual auto end(const std::shared_ptr<OperatorNode>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }

  virtual auto begin(const std::shared_ptr<TypeNode>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }
  virtual auto visit(const std::shared_ptr<TypeNode>& node) -> std::shared_ptr<Visitor> {
    UNUSED(node);
    return shared_from_this();
  }
  virtual auto end(const std::shared_ptr<TypeNode>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }
};

#endif
