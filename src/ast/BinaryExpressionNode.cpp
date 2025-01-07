#include "BinaryExpressionNode.h"
#include "Visitor.h"
BinaryExpressionNode::BinaryExpressionNode(const std::string& identfier, const AstPtr& op, const AstPtr& exp)
    : _identifier(identfier), _op(op), _expression(exp) {
  // Constructor implementation
}

BinaryExpressionNode::BinaryExpressionNode(const std::string& identfier) : _identifier(identfier) {
  // Constructor implementation
}

void BinaryExpressionNode::accept(const std::shared_ptr<Visitor>& v) {
  v->begin(shared_from_this());
  auto v_new = v->visit(shared_from_this());
  if (_op) {
    _op->accept(v_new);
    _expression->accept(v_new);
  }
  v->end(shared_from_this());
}
