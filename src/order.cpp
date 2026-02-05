#include "order.h" //"" Checks the local files
#include <stdlib.h> // <> Checks the system include paths
#include <string>

using namespace std;

class order{
    public:
    int order_id;
    double price;
    double quantity;
    double timestamp;
    string type;
}
