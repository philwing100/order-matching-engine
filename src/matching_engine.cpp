//matching_engine.cpp
#include "matching_engine.h"

void MatchingEngine::start(){
    running_ = true;
        thread_ = std::thread(&MatchingEngine::run, this);
}

void MatchingEngine::stop(){
        running_ = false;
    if (thread_.joinable())
        thread_.join();
}

insert_book(){
    if(){}
    
}

bool insert_order(Order* o){
    if(o){
        return false;
    }


    return true;
}

void MatchingEngine::run(int opt) {
    while(true){
        if(insert_order(&stream_.pop())){
            break;
        }
    }

    running_ = false;
}

//will check what stock a given order is for, if there is not an order book for that stock then it will make one


//this 

//

