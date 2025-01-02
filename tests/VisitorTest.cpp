#include "purge.hpp"
#include "Parser.h"
#include "Visitor.h"
#include "TranslationUnitNode.h"

class TestVisitor : public Visitor{
  public:
  TestVisitor(): count(0) {}
  
  auto begin(const std::shared_ptr<TranslationUnitNode> & node)->VisitResult{
    std::cout<<"XXX"<<std::endl;
    count++;
    return true;
  }
 
  auto visit(const std::shared_ptr<TranslationUnitNode> & node)->VisitResult{
    count++;
    return true;
  }

  auto end(const std::shared_ptr<TranslationUnitNode> & node)->VisitResult{
    count++;
    return true;
  }

  int count;
};


PURGE_MAIN

SIMPLE_TEST_CASE(SimpleCallTest) {
  auto node = std::make_shared<TranslationUnitNode>();
  auto testV = std::make_shared<TestVisitor>();
  node->accept(testV);
  REQUIRE(testV->count == 3);
}
