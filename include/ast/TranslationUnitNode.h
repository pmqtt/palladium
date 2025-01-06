#ifndef PALLADIUM_TRANSLATIONUNITNODE_H
#define PALLADIUM_TRANSLATIONUNITNODE_H

#include <memory>
#include "AstNode.h"
#include <vector>

class TranslationUnitNode : public AstNode, public std::enable_shared_from_this<TranslationUnitNode> {
public:
  ~TranslationUnitNode() = default;
  TranslationUnitNode(const std::vector<AstPtr>& nodes);
  void accept(const std::shared_ptr<Visitor>& v) override;

private:
  std::vector<AstPtr> _nodes;
};

#endif // TRANSLATIONUNITNODE_H
