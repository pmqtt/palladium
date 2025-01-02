#include "Vistor.h"
#include "ReturnStatementNode.h"

ReturnStatementNode::ReturnStatementNode() {
  // Constructor implementation
}

void ReturnStatementNode::accept(const std::shared_ptr<Visitor>& v) {
  v->begin(shared_from_this());
  v->visit(shared_from_this());
  v->end(shared_from_this());
}
