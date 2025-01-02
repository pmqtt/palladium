#include "Visitor.h"
#include "VariableDeclarationNode.h"

VariableDeclarationNode::VariableDeclarationNode(const std::string& var_name, const AstPtr& expression)
    : _var_name(var_name), _expression(expression) {
  // Constructor implementation
}

void VariableDeclarationNode::accept(const std::shared_ptr<Visitor>& v) {
  v->begin(shared_from_this());
  auto v_next = v->visit(shared_from_this());
  _expression->accept(v_next);
  v->end(shared_from_this());
}
