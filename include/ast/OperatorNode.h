#ifndef PALLADIUM_OPERATORNODE_H
#define PALLADIUM_OPERATORNODE_H

#include <memory>
#include "AstNode.h"
#include "Visitor.h"

enum class OperatorKind { OP_LS, OP_ADD, OP_EQ, OP_SET };

class OperatorNode : public AstNode, public std::enable_shared_from_this<OperatorNode> {
public:
  ~OperatorNode() = default;
  OperatorNode(OperatorKind kind);
  void accept(const std::shared_ptr<Visitor>& v) override;

private:
  OperatorKind _kind;
};

#endif // OPERATORNODE_H
