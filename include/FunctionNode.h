#ifndef PALLADIUM_FUNCTIONNODE_H
#define PALLADIUM_FUNCTIONNODE_H

#include <memory>
#include <string>
#include "AstNode.h"

class FunctionNode : public AstNode, public std::enable_shared_from_this<FunctionNode> {
public:
  ~FunctionNode() = default;
  FunctionNode(const std::string& fname, const AstPtr& returnType, const AstPtr& statements);
  void accept(const std::shared_ptr<Visitor>& v) override;

public:
  auto function_name() const -> const std::string& {
    return _fname;
  }

private:
  std::string _fname;
  AstPtr _returnType;
  AstPtr _statements;
};

#endif // FUNCTIONNODE_H
