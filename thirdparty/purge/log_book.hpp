//
// Created by Michael Burzan on 06.07.20.
//

#ifndef PURGE_LOG_BOOK_HPP
#define PURGE_LOG_BOOK_HPP
#include <map>
#include <string>
#include <sstream>
#include <iomanip>

namespace purge{

    enum class TEST_STATE{
        OK,
        FAILED,
        SEGMENTATION_FAULT
    };


    std::string to_string(purge::TEST_STATE t){
        static std::map<purge::TEST_STATE,std::string> map_test_state_to_string = {
                        {purge::TEST_STATE::OK,"\033[1;32m Ok\033[0m"},
                        {purge::TEST_STATE::FAILED,"\033[1;31m Failed\033[0m"},
                        {purge::TEST_STATE::SEGMENTATION_FAULT,"\033[1;35m Segmentation fault\033[0m"}
                };
        if( map_test_state_to_string.count(t) > 0 ){
            return map_test_state_to_string[t];
        }
        return "NOT VALID";
    }



    struct entry_item{
        TEST_STATE result_state;
        std::string check_description;
        std::string line;
        std::vector<std::pair<std::string,std::string>> test_values;

    };

    struct log_book_entry{
        std::string file_name;
        std::string line;
        std::string description;
        std::string test_name;
        std::vector<entry_item> test_items;
    };

    class log_book{
    public:

        static log_book * get_instance(){
            static log_book *instance = new log_book;
            return instance;
        };
        void add_entry(const log_book_entry & entry){
            entries.push_back(entry);
        }
        log_book_entry & get_entry(const std::string test_name){
            for(auto &x : entries){
                if(x.test_name == test_name){
                    return x;
                }
            }
            throw std::exception();
        }
        std::stringstream generate_output(){
            int first_len = std::string("Test").length();
            int second_len = std::string(" Check |").length();
            int third_len = std::string(" Result").length();
            for(auto & x : entries) {
                if(first_len < x.test_name.length()){

                    first_len =  x.test_name.length();
                }
                for (auto &y : x.test_items) {
                    if(second_len < y.check_description.length()){
                        second_len = y.check_description.length();
                    }
                    if(third_len < to_string(y.result_state).length() ){
                        third_len = to_string(y.result_state).length();
                    }
                }
            }
            third_len +=3;

            std::stringstream output;
            int tLength = std::string("Test").length();
            //output<<"Test"<<std::setw(tLength+first_len)<<"| Check "<<std::setw(second_len) <<" | Result\n";
            //output<<std::setfill('=')<<std::setw(first_len + second_len + third_len)<<"\n";
            //output<<std::setfill(' ');
            output<<std::setw(first_len+second_len+third_len+20)<<std::setfill('=')<<"\n";
            output<<std::setfill(' ');
            output<<"\033[1m"<<std::setw((first_len+second_len+third_len+20)/2)<<"PURGE RESULT\033[0m"<<"\n";
            output<<std::setfill('=');
            output<<std::setw(first_len+second_len+third_len+20)<<std::setfill('=')<<"\n";
            output<<std::setfill(' ');
            output<<"\n";
            for(auto & x : entries){
                output<<"\033[1m"<<x.test_name<<"\033[0m"<<"\n";
                output<<std::setfill('-');
                output<<std::setw(first_len+second_len+third_len+20)<<"\n";
                output<<std::setfill(' ');
                for(auto & y : x.test_items){
                    output<<std::setw(first_len+tLength-2)<<"| "<<y.check_description <<std::setw(second_len-y.check_description.length()+3)<<"|"<<to_string(y.result_state)<<"\n";
                }
                output<<"\n";
            }
            return output;
        }

    private:
        log_book(){ }
        std::vector<log_book_entry> entries;

    };


}


#endif //PURGE_LOG_BOOK_HPP
