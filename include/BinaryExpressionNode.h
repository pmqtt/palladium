#ifndef PALLADIUM_BINARYEXPRESSIONNODE_H
#define PALLADIUM_BINARYEXPRESSIONNODE_H

#include <memory>
#include "AstNode.h"

class BinaryExpressionNode : public AstNode, public std::enable_shared_from_this<BinaryExpressionNode> {
public:
  ~BinaryExpressionNode() = default;
  BinaryExpressionNode(const AstPtr& op, const AstPtr& exp);
  void accept(const std::shared_ptr<Visitor>& v) override;

private:
  AstPtr _op;
  AstPtr _expression;
};

#endif // BINARYEXPRESSIONNODE_H
