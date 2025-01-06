#ifndef PALLADIUM_AST_NODE_H
#define PALLADIUM_AST_NODE_H
#include <memory>

class Visitor;

class AstNode {
public:
  virtual ~AstNode() = default;
  virtual void accept(const std::shared_ptr<Visitor>& v) = 0;
};

using AstPtr = std::shared_ptr<AstNode>;

#endif
