#ifndef PALLADIUM_CONDITIONNODE_H
#define PALLADIUM_CONDITIONNODE_H

#include <memory>
#include "AstNode.h"

class ConditionNode : public AstNode, public std::enable_shared_from_this<ConditionNode> {
public:
  ~ConditionNode() = default;
  ConditionNode(const AstPtr& expression);
  void accept(const std::shared_ptr<Visitor>& v) override;

private:
  AstPtr _expression;
};

#endif // CONDITIONNODE_H
