#include "AstNode.h"
#include "Visitor.h"
#include "FunctionNode.h"

FunctionNode::FunctionNode(const std::string& fname, const AstPtr& returnType, const AstPtr& statements)
    : _fname(fname), _returnType(returnType), _statements(statements) {
}
void FunctionNode::accept(const std::shared_ptr<Visitor>& v) {
  v->begin(shared_from_this());
  auto v_next = v->visit(shared_from_this());
  _returnType->accept(v_next);
  _statements->accept(v_next);
  v->end(shared_from_this());
}
