#ifndef PALLADIUM_STATEMENTNODE_H
#define PALLADIUM_STATEMENTNODE_H

#include <memory>
#include <string>
#include "AstNode.h"

class StatementNode : public AstNode {
public:
  ~StatementNode() = default;
  StatementNode();
  void accept(const std::shared_ptr<Visitor>& v) override;
};

#endif // STATEMENTNODE_H
