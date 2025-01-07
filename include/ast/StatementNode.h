#ifndef PALLADIUM_STATEMENTNODE_H
#define PALLADIUM_STATEMENTNODE_H

#include <memory>
#include "AstNode.h"

enum class StatementType { VAR_DEC, CONST_DEC, LOOP, RETURN_STATEMENT, EXPRESSION };

class StatementNode : public AstNode, public std::enable_shared_from_this<StatementNode> {
public:
  ~StatementNode() = default;
  StatementNode(const AstPtr& statement, StatementType type);
  void accept(const std::shared_ptr<Visitor>& v) override;
  auto statement_type() const -> StatementType {
    return _statementType;
  }

private:
  AstPtr _statement;
  StatementType _statementType;
};

#endif // STATEMENTNODE_H
