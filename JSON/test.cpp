#include "JSON.h"

int main() {
    try{
        JSONObject::parseObject("{\"hi\" : \"hey\", \"num\": -123}");
    }catch(std::exception e ) {
        std::cout << e.what() ;
    }
}