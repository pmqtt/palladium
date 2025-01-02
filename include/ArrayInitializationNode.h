#ifndef PALLADIUM_ARRAYINITIALIZATIONNODE_H
#define PALLADIUM_ARRAYINITIALIZATIONNODE_H

#include <memory>
#include <string>
#include "AstNode.h"

class ArrayInitializationNode : public AstNode {
public:
  ~ArrayInitializationNode() = default;
  ArrayInitializationNode();
  void accept(const std::shared_ptr<Visitor>& v) override;
};

#endif // ARRAYINITIALIZATIONNODE_H
