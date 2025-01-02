#include "purge.hpp"
#include "Parser.h"
#include "Visitor.h"
#include "TranslationUnitNode.h"

class TestVisitor : public Visitor{
  public:
  TestVisitor(): count(0) {}
  
  auto begin(const std::shared_ptr<TranslationUnitNode> & node)->VisitResult override{
    UNUSED(node);
    count++;
    return true;
  }
 
  auto visit(const std::shared_ptr<TranslationUnitNode> & node)->std::shared_ptr<Visitor> override{
    UNUSED(node);
    count++;
    return shared_from_this();
  }

  auto end(const std::shared_ptr<TranslationUnitNode> & node)->VisitResult override {
    UNUSED(node);
    count++;
    return true;
  }
  using Visitor::begin;
  using Visitor::end;
  using Visitor::visit;

  int count;
};


PURGE_MAIN

SIMPLE_TEST_CASE(SimpleCallTest) {
  // only compile test
}
