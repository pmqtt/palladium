#include "Visitor.h"
#include "LoopNode.h"

LoopNode::LoopNode(const AstPtr& condition, const AstPtr& statements) : _condition(condition), _statements(statements) {
  // Constructor implementation
}

void LoopNode::accept(const std::shared_ptr<Visitor>& v) {
  v->begin(shared_from_this());
  auto v_new = v->visit(shared_from_this());
  _condition->accept(v_new);
  _statements->accept(v_new);
  v->end(shared_from_this());
}
