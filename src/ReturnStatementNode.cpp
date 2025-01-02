#include "AstNode.h"
#include "Visitor.h"
#include "ReturnStatementNode.h"

ReturnStatementNode::ReturnStatementNode(const AstPtr& expression) : _expression(expression) {
  // Constructor implementation
}

void ReturnStatementNode::accept(const std::shared_ptr<Visitor>& v) {
  v->begin(shared_from_this());
  auto v_new = v->visit(shared_from_this());
  _expression->accept(v_new);
  v->end(shared_from_this());
}
