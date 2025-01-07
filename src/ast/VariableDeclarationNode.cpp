#include "AstNode.h"
#include "Visitor.h"
#include "VariableDeclarationNode.h"

VariableDeclarationNode::VariableDeclarationNode(const std::string& var_name, const AstPtr& type,
                                                 const AstPtr& expression)
    : _var_name(var_name), _type(type), _expression(expression) {
  // Constructor implementation
}

void VariableDeclarationNode::accept(const std::shared_ptr<Visitor>& v) {
  v->begin(shared_from_this());
  auto v_next = v->visit(shared_from_this());
  _type->accept(v_next);
  _expression->accept(v_next);
  v->end(shared_from_this());
}
