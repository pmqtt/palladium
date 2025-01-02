#ifndef PALLADIUM_ARRAYINITIALIZATIONNODE_H
#define PALLADIUM_ARRAYINITIALIZATIONNODE_H

#include <memory>
#include <string>
#include "AstNode.h"

class ArrayInitializationNode : public AstNode, public std::enable_shared_from_this<ArrayInitializationNode> {
public:
  ~ArrayInitializationNode() = default;
  ArrayInitializationNode(const AstPtr& exp_left, const AstPtr& exp_right);
  void accept(const std::shared_ptr<Visitor>& v) override;

private:
  AstPtr _exp_left;
  AstPtr _exp_right;
};

#endif // ARRAYINITIALIZATIONNODE_H
