#include "AstNode.h"
#include "FunctionNode.h"
#include "Visitor.h"
#include "TranslationUnitNode.h"

TranslationUnitNode::TranslationUnitNode(const AstPtr& fnode) : _fnode(fnode) {
  // Constructor implementation
}

void TranslationUnitNode::accept(const std::shared_ptr<Visitor>& v) {
  v->begin(shared_from_this());
  auto v_next = v->visit(shared_from_this());
  _fnode->accept(v_next);
  v->end(shared_from_this());
}
