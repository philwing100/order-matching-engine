//Order.cpp
#include "order.h" //"" Checks the local files
#include <stdlib.h> // <> Checks the system include paths
#include <string>

using namespace std;
    

//True bool is bid
//False bool is ask
Order::Order(int id, string symbol, bool side, double price, double quantity)
        : id_(id), symbol_(symbol), side_(side), price_(price), quantity_(quantity){}

void Order::print_order(){
        cout<<"Order id " << id_ <<"\n";
        cout<<"Symbol " << symbol_ <<"\n";
        cout<<"Side " << side_ <<"\n";
        cout<<"Price " << price_ <<"\n";
        cout<<"Quantity " << quantity_ <<"\n";
}

int Order::getId(){
        return id_;
}

string Order::getSymbol(){
        return symbol_;
}

bool Order::getSide(){
        return side_;
}

double Order::getPrice(){
        return price_;
}

double Order::getQuantity(){
        return quantity_;
}




