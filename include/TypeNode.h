#ifndef PALLADIUM_TYPENODE_H
#define PALLADIUM_TYPENODE_H

#include <memory>
#include <string>
#include "AstNode.h"

class TypeNode : public AstNode, public std::enable_shared_from_this<TypeNode> {
public:
  ~TypeNode() = default;
  TypeNode();
  void accept(const std::shared_ptr<Visitor>& v) override;
};

#endif // TYPENODE_H
