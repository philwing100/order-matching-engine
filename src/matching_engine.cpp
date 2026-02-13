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
bool insert_order(Order* o){
    if(o){
        return false;
    }

    lookup_book(o.getSymbol());

    return true;
}

bool lookup_book(string symbol){
    if(book_map.find(symbol) != book_map.end()){

        return true;
    }else{
        create_new_book(symbol);
        
        return false;
    }
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

