#include "BinaryExpressionNode.h"
#include "Vistor.h"
BinaryExpressionNode::BinaryExpressionNode() {
  // Constructor implementation
}

void BinaryExpressionNode::accept(const std::shared_ptr<Visitor>& v) {
  v->begin(shared_from_this());
  v->visit(shared_from_this());
  v->end(shared_from_this());
}
