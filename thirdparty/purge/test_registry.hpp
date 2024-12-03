//
// Created by Michael Burzan on 03.07.20.
//

#ifndef PTEST_TEST_REGISTRY_HPP
#define PTEST_TEST_REGISTRY_HPP
#include <vector>
#include <map>
#include <functional>
#include <tuple>
#include <string>
namespace purge{

    class test_registry{
    public:

        test_registry(std::function<void()> testAsFunction,const std::string & test_name,int line_no){
            test_storage[test_name] = std::pair<int,std::function<void()>>{line_no,testAsFunction};
        }

        static std::map<std::string,std::pair<int,std::function<void()>>> test_storage;
    };
}
#define INIT_TEST_REGISTRY \
        std::map<std::string,std::pair<int,std::function<void()>>> purge::test_registry::test_storage;\

#endif //PTEST_TEST_REGISTRY_HPP
