#ifndef PALLADIUM_EXPRESSIONNODE_H
#define PALLADIUM_EXPRESSIONNODE_H

#include <memory>
#include <string>
#include "AstNode.h"

enum class ExpressionKind { CONST_DOUBLE, CONST_INT, CONST_TEXT, ARRAY_INIT, BIN_OP };

class ExpressionNode : public AstNode, public std::enable_shared_from_this<ExpressionNode> {
public:
  ~ExpressionNode() = default;
  ExpressionNode(const AstPtr& exp, ExpressionKind kind);
  ExpressionNode(const std::string& constante, ExpressionKind kind) : _constante(constante), _kind(kind) {
  }
  void accept(const std::shared_ptr<Visitor>& v) override;

  auto kind() const -> ExpressionKind {
    return _kind;
  }

  auto constante() const -> const std::string& {
    return _constante;
  }

private:
  std::string _constante;
  AstPtr _exp;
  ExpressionKind _kind;
};

#endif // EXPRESSIONNODE_H
