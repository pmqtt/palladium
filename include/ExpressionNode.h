#ifndef PALLADIUM_EXPRESSIONNODE_H
#define PALLADIUM_EXPRESSIONNODE_H

#include <memory>
#include <string>
#include "AstNode.h"

class ExpressionNode : public AstNode {
public:
  ~ExpressionNode() = default;
  ExpressionNode();
  void accept(const std::shared_ptr<Visitor>& v) override;
};

#endif // EXPRESSIONNODE_H
