#ifndef PALLADIUM_CONSTANTDECLARATIONNODE_H
#define PALLADIUM_CONSTANTDECLARATIONNODE_H

#include <memory>
#include <string>
#include "AstNode.h"

class ConstantDeclarationNode : public AstNode {
public:
  ~ConstantDeclarationNode() = default;
  ConstantDeclarationNode();
  void accept(const std::shared_ptr<Visitor>& v) override;
};

#endif // CONSTANTDECLARATIONNODE_H
