#include "Visitor.h"
#include "TypeNode.h"

TypeNode::TypeNode(const std::string& identfier, TypeKind kind) : _identifier(identfier), _kind(kind) {
  // Constructor implementation
}

void TypeNode::accept(const std::shared_ptr<Visitor>& v) {
  v->begin(shared_from_this());
  v->visit(shared_from_this());
  v->end(shared_from_this());
}
