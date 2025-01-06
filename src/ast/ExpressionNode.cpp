#include "AstNode.h"
#include "Visitor.h"
#include "ExpressionNode.h"

ExpressionNode::ExpressionNode(const AstPtr& exp, ExpressionKind kind) : _exp(exp), _kind(kind) {
  // Constructor implementation
}

void ExpressionNode::accept(const std::shared_ptr<Visitor>& v) {
  v->begin(shared_from_this());
  auto v_new = v->visit(shared_from_this());
  if (_exp) {
    _exp->accept(v_new);
  }
  v->end(shared_from_this());
}
