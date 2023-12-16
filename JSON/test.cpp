#include "JSON.h"

int main() {
    try{
        JSONObject* object = JSONObject::parseObject("{\"hi\" : \"hey\", \"num\": -123}");
        JSONElement x = (object)["hi"];
    }catch(std::exception e ) {
        std::cout << "Caught exception" << e.what() ;
    }
}