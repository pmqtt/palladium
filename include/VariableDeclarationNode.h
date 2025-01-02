#ifndef PALLADIUM_VARIABLEDECLARATIONNODE_H
#define PALLADIUM_VARIABLEDECLARATIONNODE_H

#include <memory>
#include <string>
#include "AstNode.h"

class VariableDeclarationNode : public AstNode, public std::enable_shared_from_this<VariableDeclarationNode> {
public:
  ~VariableDeclarationNode() = default;
  VariableDeclarationNode(const std::string& var_name, const AstPtr& expression);
  void accept(const std::shared_ptr<Visitor>& v) override;

private:
  std::string _var_name;
  AstPtr _expression;
};

#endif // VARIABLEDECLARATIONNODE_H
