#include "Visitor.h"
#include "StatementNode.h"

StatementNode::StatementNode() {
  // Constructor implementation
}

void StatementNode::accept(const std::shared_ptr<Visitor>& v) {
  v->begin(shared_from_this());
  v->visit(shared_from_this());
  v->end(shared_from_this());
}
