#ifndef PALLADIUM_BINARYEXPRESSIONNODE_H
#define PALLADIUM_BINARYEXPRESSIONNODE_H

#include <memory>
#include <string>
#include "AstNode.h"

class BinaryExpressionNode : public AstNode, public std::enable_shared_from_this<BinaryExpressionNode> {
public:
  ~BinaryExpressionNode() = default;
  BinaryExpressionNode();
  void accept(const std::shared_ptr<Visitor>& v) override;
};

#endif // BINARYEXPRESSIONNODE_H
