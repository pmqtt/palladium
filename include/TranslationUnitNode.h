#ifndef PALLADIUM_TRANSLATIONUNITNODE_H
#define PALLADIUM_TRANSLATIONUNITNODE_H

#include <memory>
#include <string>
#include "AstNode.h"

class TranslationUnitNode : public AstNode, public std::enable_shared_from_this<TranslationUnitNode> {
public:
  ~TranslationUnitNode() = default;
  TranslationUnitNode();
  void accept(const std::shared_ptr<Visitor>& v) override;
};

#endif // TRANSLATIONUNITNODE_H
