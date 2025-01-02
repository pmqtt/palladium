#include "Visitor.h"
#include "StatementsNode.h"

StatementsNode::StatementsNode() {
  // Constructor implementation
}

void StatementsNode::accept(const std::shared_ptr<Visitor>& v) {
  v->begin(shared_from_this());
  v->visit(shared_from_this());
  v->end(shared_from_this());
}
