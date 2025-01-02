#ifndef PALLADIUM_BINARYEXPRESSIONNODE_H
#define PALLADIUM_BINARYEXPRESSIONNODE_H

#include <memory>
#include <string>
#include "AstNode.h"

class BinaryExpressionNode : public AstNode {
public:
  ~BinaryExpressionNode() = default;
  BinaryExpressionNode();
  void accept(const std::shared_ptr<Visitor>& v) override;
};

#endif // BINARYEXPRESSIONNODE_H
