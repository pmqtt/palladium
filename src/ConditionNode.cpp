#include "ConditionNode.h"
#include "Vistor.h"

ConditionNode::ConditionNode() {
  // Constructor implementation
}

void ConditionNode::accept(const std::shared_ptr<Visitor>& v) {
  v->begin(shared_from_this());
  v->visit(shared_from_this());
  v->end(shared_from_this());
}
