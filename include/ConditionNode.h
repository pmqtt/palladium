#ifndef PALLADIUM_CONDITIONNODE_H
#define PALLADIUM_CONDITIONNODE_H

#include <memory>
#include <string>
#include "AstNode.h"

class ConditionNode : public AstNode, public std::enable_shared_from_this<ConditionNode> {
public:
  ~ConditionNode() = default;
  ConditionNode();
  void accept(const std::shared_ptr<Visitor>& v) override;
};

#endif // CONDITIONNODE_H
