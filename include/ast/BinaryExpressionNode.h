#ifndef PALLADIUM_BINARYEXPRESSIONNODE_H
#define PALLADIUM_BINARYEXPRESSIONNODE_H

#include <memory>
#include "AstNode.h"

class BinaryExpressionNode : public AstNode, public std::enable_shared_from_this<BinaryExpressionNode> {
public:
  ~BinaryExpressionNode() = default;
  BinaryExpressionNode(const std::string& identifier, const AstPtr& op, const AstPtr& exp);
  BinaryExpressionNode(const std::string& identifier);
  void accept(const std::shared_ptr<Visitor>& v) override;
  auto identfier() const -> const std::string& {
    return _identifier;
  }

private:
  std::string _identifier;
  AstPtr _op;
  AstPtr _expression;
};

#endif // BINARYEXPRESSIONNODE_H
