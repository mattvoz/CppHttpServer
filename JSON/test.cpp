#include "JSON.h"

int main() {
    try{
        JSONObject object = JSONObject("{\"hi\" : \"hey\", \"num\": -123}");
        JSONElement * x = object["hi"];
        printf("Flushed");
        fflush(stdout);
        std::cout <<  x->value() << std::endl;
    }catch(std::exception e ) {
        std::cout << "Caught exception" << e.what() ;
    }
}