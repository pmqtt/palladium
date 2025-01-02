#include "Visitor.h"
#include "VariableDeclarationNode.h"

VariableDeclarationNode::VariableDeclarationNode() {
  // Constructor implementation
}

void VariableDeclarationNode::accept(const std::shared_ptr<Visitor>& v) {
  v->begin(shared_from_this());
  v->visit(shared_from_this());
  v->end(shared_from_this());
}
