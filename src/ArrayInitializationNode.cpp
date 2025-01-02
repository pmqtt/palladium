#include "ArrayInitializationNode.h"
#include "Visitor.h"

ArrayInitializationNode::ArrayInitializationNode() {
  // Constructor implementation
}

void ArrayInitializationNode::accept(const std::shared_ptr<Visitor>& v) {
  v->begin(shared_from_this());
  v->visit(shared_from_this());
  v->end(shared_from_this());
}
