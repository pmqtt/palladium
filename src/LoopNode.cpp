#include "Visitor.h"
#include "LoopNode.h"

LoopNode::LoopNode() {
  // Constructor implementation
}

void LoopNode::accept(const std::shared_ptr<Visitor>& v) {
  v->begin(shared_from_this());
  v->visit(shared_from_this());
  v->end(shared_from_this());
}
