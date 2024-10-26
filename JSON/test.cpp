#include "JSON.h"

int main() {
    try{
        JSONObject object = JSONObject("{\"hi\" : \"hey\", \"testtwo\": null, \"num\": -123, \"arr\":[1, 3, \"hi\"]}");
        JSONElement * x = object["num"];
        std::cout <<  *(double *)x->value() << std::endl;
    }catch(std::exception e ) {
        std::cout << "Caught exception" << e.what() ;
    }
}