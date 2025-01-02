#ifndef PALLADIUM_VISITOR_H
#define PALLADIUM_VISITOR_H
#include "TranslationUnitNode.h"
#include "Util.h"

using VisitResult = ResultOr<bool>;

class Visitor {
public:
  explicit Visitor() = default;
  virtual ~Visitor() = default;

  virtual auto begin(const std::shared_ptr<TranslationUnitNode>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }
  virtual auto visit(const std::shared_ptr<TranslationUnitNode>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }

  virtual auto end(const std::shared_ptr<TranslationUnitNode>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }

  virtual auto begin(const std::shared_ptr<FunctionNode>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }
  virtual auto visit(const std::shared_ptr<FunctionNode>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }
  virtual auto end(const std::shared_ptr<FunctionNode>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }

  virtual auto begin(const std::shared_ptr<StatementsNode>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }
  virtual auto visit(const std::shared_ptr<StatementsNode>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }
  virtual auto end(const std::shared_ptr<StatementsNode>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }
  virtual auto begin(const std::shared_ptr<StatementNode>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }
  virtual auto visit(const std::shared_ptr<StatementNode>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }
  virtual auto end(const std::shared_ptr<StatementNode>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }
  virtual auto begin(const std::shared_ptr<VariableDeclarationNode>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }
  virtual auto visit(const std::shared_ptr<VariableDeclarationNode>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }
  virtual auto end(const std::shared_ptr<VariableDeclarationNode>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }
  virtual auto begin(const std::shared_ptr<ConstantDeclarationNode>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }
  virtual auto visit(const std::shared_ptr<ConstantDeclarationNode>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }
  virtual auto end(const std::shared_ptr<ConstantDeclarationNode>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }
  virtual auto begin(const std::shared_ptr<LoopNode>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }
  virtual auto visit(const std::shared_ptr<LoopNode>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }
  virtual auto end(const std::shared_ptr<LoopNode>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }
  virtual auto begin(const std::shared_ptr<ReturnStatement>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }
  virtual auto visit(const std::shared_ptr<ReturnStatement>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }
  virtual auto end(const std::shared_ptr<ReturnStatement>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }
  virtual auto begin(const std::shared_ptr<ExpressionNode>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }
  virtual auto visit(const std::shared_ptr<ExpressionNode>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }
  virtual auto end(const std::shared_ptr<ExpressionNode>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }
  virtual auto begin(const std::shared_ptr<ArrayInitializationNode>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }
  virtual auto visit(const std::shared_ptr<ArrayInitializationNode>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }
  virtual auto end(const std::shared_ptr<ArrayInitializationNode>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }
  virtual auto begin(const std::shared_ptr<BinarayExpressionNode>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }
  virtual auto visit(const std::shared_ptr<BinarayExpressionNode>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }
  virtual auto end(const std::shared_ptr<BinarayExpressionNode>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }
  virtual auto begin(const std::shared_ptr<ConditionNode>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }
  virtual auto visit(const std::shared_ptr<ConditionNode>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }
  virtual auto end(const std::shared_ptr<ConditionNode>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }
  virtual auto begin(const std::shared_ptr<TypeNode>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }
  virtual auto visit(const std::shared_ptr<TypeNode>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }
  virtual auto end(const std::shared_ptr<TypeNode>& node) -> VisitResult {
    UNUSED(node);
    return true;
  }
};

#endif
