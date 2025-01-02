#include "Vistor.h"
#include "ExpressionNode.h"

ExpressionNode::ExpressionNode() {
  // Constructor implementation
}

void ExpressionNode::accept(const std::shared_ptr<Visitor>& v) {
  v->begin(shared_from_this());
  v->visit(shared_from_this());
  v->end(shared_from_this());
}
