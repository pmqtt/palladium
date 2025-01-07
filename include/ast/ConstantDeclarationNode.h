#ifndef PALLADIUM_CONSTANTDECLARATIONNODE_H
#define PALLADIUM_CONSTANTDECLARATIONNODE_H

#include <memory>
#include <string>
#include "AstNode.h"

class ConstantDeclarationNode : public AstNode, public std::enable_shared_from_this<ConstantDeclarationNode> {
public:
  ~ConstantDeclarationNode() = default;
  ConstantDeclarationNode(const std::string& var_name, const AstPtr& type, const AstPtr& expression);
  void accept(const std::shared_ptr<Visitor>& v) override;

private:
  std::string _var_name;
  AstPtr _type;
  AstPtr _expression;
};

#endif // CONSTANTDECLARATIONNODE_H
