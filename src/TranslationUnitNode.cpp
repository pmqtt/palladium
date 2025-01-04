#include "AstNode.h"
#include "Visitor.h"
#include "TranslationUnitNode.h"

TranslationUnitNode::TranslationUnitNode(const std::vector<AstPtr>& nodes) : _nodes(nodes) {
  // Constructor implementation
}

void TranslationUnitNode::accept(const std::shared_ptr<Visitor>& v) {
  v->begin(shared_from_this());
  auto v_next = v->visit(shared_from_this());
  for (auto& node : _nodes) {
    node->accept(v_next);
  }
  v->end(shared_from_this());
}
