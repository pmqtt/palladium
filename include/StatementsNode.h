#ifndef PALLADIUM_STATEMENTSNODE_H
#define PALLADIUM_STATEMENTSNODE_H

#include <memory>
#include <vector>
#include "AstNode.h"

class StatementsNode : public AstNode, public std::enable_shared_from_this<StatementsNode> {
public:
  ~StatementsNode() = default;
  StatementsNode(const std::vector<AstPtr>& statements);
  void accept(const std::shared_ptr<Visitor>& v) override;

private:
  std::vector<AstPtr> _statements;
};

#endif // STATEMENTSNODE_H
