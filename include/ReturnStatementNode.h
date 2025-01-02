#ifndef PALLADIUM_RETURNSTATEMENTNODE_H
#define PALLADIUM_RETURNSTATEMENTNODE_H

#include <memory>
#include <string>
#include "AstNode.h"

class ReturnStatementNode : public AstNode {
public:
  ~ReturnStatementNode() = default;
  ReturnStatementNode();
  void accept(const std::shared_ptr<Visitor>& v) override;
};

#endif // RETURNSTATEMENTNODE_H
