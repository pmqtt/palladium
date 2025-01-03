#include "ConstantDeclarationNode.h"
#include "Visitor.h"

ConstantDeclarationNode::ConstantDeclarationNode(const std::string& var_name, const AstPtr& expression)
    : _var_name(var_name), _expression(expression) {
  // Constructor implementation
}

void ConstantDeclarationNode::accept(const std::shared_ptr<Visitor>& v) {
  v->begin(shared_from_this());
  auto v_next = v->visit(shared_from_this());
  _expression->accept(v_next);
  v->end(shared_from_this());
}
