#include "Visitor.h"
#include "StatementNode.h"

StatementNode::StatementNode(const AstPtr& statement, StatementType type)
    : _statement(statement), _statementType(type) {
  // Constructor implementation
}

void StatementNode::accept(const std::shared_ptr<Visitor>& v) {
  v->begin(shared_from_this());
  auto v_next = v->visit(shared_from_this());
  _statement->accept(v_next);
  v->end(shared_from_this());
}
