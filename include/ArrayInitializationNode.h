#ifndef PALLADIUM_ARRAYINITIALIZATIONNODE_H
#define PALLADIUM_ARRAYINITIALIZATIONNODE_H

#include <memory>
#include <string>
#include "AstNode.h"

class ArrayInitializationNode : public AstNode, public std::enable_shared_from_this<ArrayInitializationNode> {
public:
  ~ArrayInitializationNode() = default;
  ArrayInitializationNode();
  void accept(const std::shared_ptr<Visitor>& v) override;
};

#endif // ARRAYINITIALIZATIONNODE_H
