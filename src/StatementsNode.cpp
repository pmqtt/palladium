#include "Visitor.h"
#include "StatementsNode.h"
#include <memory>

StatementsNode::StatementsNode(const std::vector<AstPtr>& statements) : _statements(statements) {
  // Constructor implementation
}

void StatementsNode::accept(const std::shared_ptr<Visitor>& v) {
  v->begin(shared_from_this());
  auto v_next = v->visit(shared_from_this());
  for (auto& statement : _statements) {
    statement->accept(v_next);
  }
  v->end(shared_from_this());
}
