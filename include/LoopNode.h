#ifndef PALLADIUM_LOOPNODE_H
#define PALLADIUM_LOOPNODE_H

#include <memory>
#include <string>
#include "AstNode.h"

class LoopNode : public AstNode, public std::enable_shared_from_this<LoopNode> {
public:
  ~LoopNode() = default;
  LoopNode();
  void accept(const std::shared_ptr<Visitor>& v) override;
};

#endif // LOOPNODE_H
