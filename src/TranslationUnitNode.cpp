#include "Visitor.h"
#include "TranslationUnitNode.h"
#include <iostream>

TranslationUnitNode::TranslationUnitNode() {
  // Constructor implementation
}

void TranslationUnitNode::accept(const std::shared_ptr<Visitor>& v) {
  v->begin(shared_from_this());
  v->visit(shared_from_this());
  v->end(shared_from_this());
}
