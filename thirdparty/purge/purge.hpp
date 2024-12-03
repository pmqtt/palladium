//
// Created by Michael Burzan on 02.07.20.
//

#ifndef PURGE_PURGE_HPP
#define PURGE_PURGE_HPP

#include "purge_common.hpp"
#include "runner.hpp"
#include "log_book.hpp"
#include "purge_tuple_util.hpp"

class ptester{
public:
    void test(const std::string & t,int line_no,bool res,const std::string &function_name){
        if(!res){
            auto & entry = purge::log_book::get_instance()->get_entry(function_name);
            purge::entry_item item;
            item.check_description = t;
            item.result_state = purge::TEST_STATE::FAILED;
            item.line = std::to_string(line_no);
            entry.test_items.push_back(item);
        }else{
            auto & entry = purge::log_book::get_instance()->get_entry(function_name);
            purge::entry_item item;
            item.check_description = t;
            item.result_state = purge::TEST_STATE::OK;
            item.line = std::to_string(line_no);
            entry.test_items.push_back(item);
        }
    }

    ptester(){}
private:

};

#define SIMPLE_TEST_CASE(TEST_NAME) \
    static void TEST_NAME(); \
    namespace { purge::test_registry PURGE_UNIQUE_NAME(TEST_NAME)(TEST_NAME,std::string(#TEST_NAME),__LINE__); } \
    static void TEST_NAME()


#define FUZZER_CONTAINER_CASE(TEST_NAME,CONTAINER,RULE,VALUE) \
    static void TEST_NAME(const CONTAINER & VALUE); \
    namespace { \
    purge::test_registry PURGE_UNIQUE_NAME(TEST_NAME)( \
        [](){\
           purge::fuzz_runner<CONTAINER>(RULE,TEST_NAME); \
        },std::string(#TEST_NAME),__LINE__); \
    } \
    static void TEST_NAME(const CONTAINER & VALUE)

//METHOD_TEST_CASE(TEST_CLASS_METHOD,NumberClass,is_number,(false,'a'), ( true,'1'),(false,'b'));
#define METHODE_TEST_CASE(TEST_NAME,CLASS,CONSTRUCT_PARAMETER,METHODE,TUPLE,...) \
    namespace { \
        purge::test_registry PURGE_UNIQUE_NAME(TEST_NAME)( \
        [](){\
            auto TUP = std::make_tuple TUPLE; \
            std::vector<decltype(TUP)> vec={ TUP, CALL_MACRO_X_FOR_EACH(MAKE_TUPE,__VA_ARGS__)  }; \
                for(auto & x : vec){ \
                   auto res = std::get<0>(x); \
                   bool correct_test; \
                   auto parameter = purge::get_parameter_from_tuple<decltype(TUP)>(x); \
                   std::apply([&correct_test,&res](auto &&... args) { \
                        auto callable = std::mem_fn(&CLASS::METHODE);\
                        CLASS clazz CONSTRUCT_PARAMETER;\
                        correct_test = (res == callable(clazz,args...));\
                    }, parameter); \
                   ptester().test(std::string(#CLASS)+"::"+std::string(#METHODE),__LINE__,correct_test,std::string(#TEST_NAME)); \
                }\
            \
        },std::string(#TEST_NAME),__LINE__); \
    }



#define FUNCTION_TEST_CASE(TEST_NAME,TEST_FUNC,TUPLE,...)\
    namespace { \
    purge::test_registry PURGE_UNIQUE_NAME(TEST_NAME)( \
        [](){\
            auto TUP = std::make_tuple TUPLE; \
            std::vector<decltype(TUP)> vec={ TUP, CALL_MACRO_X_FOR_EACH(MAKE_TUPE,__VA_ARGS__)  }; \
                for(auto & x : vec){ \
                   auto res = std::get<0>(x); \
                   bool correct_test; \
                   auto parameter = purge::get_parameter_from_tuple<decltype(TUP)>(x); \
                   std::apply([&correct_test,&res](auto &&... args) { correct_test = (res == TEST_FUNC(args...)); }, parameter); \
                   ptester().test(#TEST_FUNC,__LINE__,correct_test,#TEST_NAME); \
                }\
            \
        },std::string(#TEST_NAME),__LINE__); \
    }


#define REQUIRE(X) \
    ptester().test(#X,__LINE__,X,__FUNCTION__)




#define PURGE_MAIN \
    INIT_TEST_REGISTRY \
    int main(int argc,char** argv){ \
        purge::runner(argc,argv).run(); \
        std::cout<<purge::log_book::get_instance()->generate_output().str(); \
    }


#endif //PURGE_PURGE_HPP
