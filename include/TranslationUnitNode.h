#ifndef PALLADIUM_TRANSLATIONUNITNODE_H
#define PALLADIUM_TRANSLATIONUNITNODE_H

#include <memory>
#include "AstNode.h"
#include "FunctionNode.h"

class FunctionNode;

class TranslationUnitNode : public AstNode, public std::enable_shared_from_this<TranslationUnitNode> {
public:
  ~TranslationUnitNode() = default;
  TranslationUnitNode(const AstPtr& fnode);
  void accept(const std::shared_ptr<Visitor>& v) override;

private:
  AstPtr _fnode;
};

#endif // TRANSLATIONUNITNODE_H
