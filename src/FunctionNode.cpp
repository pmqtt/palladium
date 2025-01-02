#include "Visitor.h"
#include "FunctionNode.h"

FunctionNode::FunctionNode() {
  // Constructor implementation
}

void FunctionNode::accept(const std::shared_ptr<Visitor>& v) {
  v->begin(shared_from_this());
  v->visit(shared_from_this());
  v->end(shared_from_this());
}
