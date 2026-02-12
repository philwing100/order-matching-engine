//Order.cpp
#include "order.h" //"" Checks the local files
#include <stdlib.h> // <> Checks the system include paths
#include <string>

using namespace std;
    
Order::Order(int id, string symbol, bool side, PriceLevel price, double quantity, string type)
        : id_(id), symbol_(symbol), side_(side), price_(price), quantity_(quantity), type_(type){}

void Order::print_order(){
        cout<<"Order id " << id_<<"\n";
        cout<<"Side " << side_<<"\n";
        cout<<"Price " << price_<<"\n";
        cout<<"Quantity " << quantity_<<"\n";
        cout<<"Type " << type_<<"\n";
}






