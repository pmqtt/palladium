#ifndef PALLADIUM_VARIABLEDECLARATIONNODE_H
#define PALLADIUM_VARIABLEDECLARATIONNODE_H

#include <memory>
#include <string>
#include "AstNode.h"

class VariableDeclarationNode : public AstNode, public std::enable_shared_from_this<VariableDeclarationNode> {
public:
  ~VariableDeclarationNode() = default;
  VariableDeclarationNode(const std::string& var_name, const AstPtr& type, const AstPtr& expression);
  void accept(const std::shared_ptr<Visitor>& v) override;
  auto var_name() const -> const std::string& {
    return _var_name;
  }
  auto expression() const -> AstPtr {
    return _expression;
  }

private:
  std::string _var_name;
  AstPtr _type;
  AstPtr _expression;
};

#endif // VARIABLEDECLARATIONNODE_H
