#include "ConditionNode.h"
#include "Visitor.h"

ConditionNode::ConditionNode(const AstPtr& expression) : _expression(expression) {
  // Constructor implementation
}

void ConditionNode::accept(const std::shared_ptr<Visitor>& v) {
  v->begin(shared_from_this());
  auto v_new = v->visit(shared_from_this());
  _expression->accept(v_new);
  v->end(shared_from_this());
}
