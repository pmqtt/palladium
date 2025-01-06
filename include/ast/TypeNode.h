#ifndef PALLADIUM_TYPENODE_H
#define PALLADIUM_TYPENODE_H

#include <memory>
#include <string>
#include "AstNode.h"

enum class TypeKind { BUILD_IN_I32, USER_IDENTIFIER };

class TypeNode : public AstNode, public std::enable_shared_from_this<TypeNode> {
public:
  ~TypeNode() = default;
  TypeNode(const std::string& identfier, TypeKind kind);
  void accept(const std::shared_ptr<Visitor>& v) override;

private:
  std::string _identifier;
  TypeKind _kind;
};

#endif // TYPENODE_H
