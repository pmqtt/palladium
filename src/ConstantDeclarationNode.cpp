#include "ConstantDeclarationNode.h"
#include "Visitor.h"

ConstantDeclarationNode::ConstantDeclarationNode() {
  // Constructor implementation
}

void ConstantDeclarationNode::accept(const std::shared_ptr<Visitor>& v) {
  v->begin(shared_from_this());
  v->visit(shared_from_this());
  v->end(shared_from_this());
}
