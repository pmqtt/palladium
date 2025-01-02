#ifndef PALLADIUM_FUNCTIONNODE_H
#define PALLADIUM_FUNCTIONNODE_H

#include <memory>
#include <string>
#include "AstNode.h"

class FunctionNode : public AstNode, public std::enable_shared_from_this<FunctionNode> {
public:
  ~FunctionNode() = default;
  FunctionNode();
  void accept(const std::shared_ptr<Visitor>& v) override;
};

#endif // FUNCTIONNODE_H
