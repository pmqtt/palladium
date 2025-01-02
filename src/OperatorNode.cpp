#include "OperatorNode.h"

OperatorNode::OperatorNode(OperatorKind kind) : _kind(kind) {
  // Constructor implementation
}

void OperatorNode::accept(const std::shared_ptr<Visitor>& v) {
  v->begin(shared_from_this());
  auto v_new = v->visit(shared_from_this());
  v->end(shared_from_this());
}
