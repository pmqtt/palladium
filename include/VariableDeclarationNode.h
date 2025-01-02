#ifndef PALLADIUM_VARIABLEDECLARATIONNODE_H
#define PALLADIUM_VARIABLEDECLARATIONNODE_H

#include <memory>
#include <string>
#include "AstNode.h"

class VariableDeclarationNode : public AstNode {
public:
  ~VariableDeclarationNode() = default;
  VariableDeclarationNode();
  void accept(const std::shared_ptr<Visitor>& v) override;
};

#endif // VARIABLEDECLARATIONNODE_H
