#ifndef PALLADIUM_RETURNSTATEMENTNODE_H
#define PALLADIUM_RETURNSTATEMENTNODE_H

#include <memory>
#include <string>
#include "AstNode.h"

class ReturnStatementNode : public AstNode, public std::enable_shared_from_this<ReturnStatementNode> {
public:
  ~ReturnStatementNode() = default;
  ReturnStatementNode(const AstPtr& expression);
  void accept(const std::shared_ptr<Visitor>& v) override;

private:
  AstPtr _expression;
};

#endif // RETURNSTATEMENTNODE_H
