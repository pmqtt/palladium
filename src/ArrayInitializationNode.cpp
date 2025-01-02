#include "ArrayInitializationNode.h"
#include "Visitor.h"

ArrayInitializationNode::ArrayInitializationNode(const AstPtr& exp_left, const AstPtr& exp_right)
    : _exp_left(exp_left), _exp_right(exp_right) {
  // Constructor implementation
}

void ArrayInitializationNode::accept(const std::shared_ptr<Visitor>& v) {
  v->begin(shared_from_this());
  auto v_new = v->visit(shared_from_this());
  _exp_left->accept(v_new);
  _exp_right->accept(v_new);
  v->end(shared_from_this());
}
