#ifndef PALLADIUM_STATEMENTSNODE_H
#define PALLADIUM_STATEMENTSNODE_H

#include <memory>
#include <string>
#include "AstNode.h"

class StatementsNode : public AstNode {
public:
  ~StatementsNode() = default;
  StatementsNode();
  void accept(const std::shared_ptr<Visitor>& v) override;
};

#endif // STATEMENTSNODE_H
