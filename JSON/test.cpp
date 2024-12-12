#include "JSON.h"
#include <string.h>

int main() {
    try{
        const char * string = "{\"hi\" : \"hey\", \"testtwo\": null, \"num\": -123, \"test\": [\"hi\", 1234]}";
        printf("length %d", strlen(string));
        JSONObject object = JSONObject(string);
        JSONElement * x = object["hi"];
        std::cout << "got object " << x << std::endl;
        std::cout << "does x = hi? " << (x == "hi") << std::endl;
    }catch(invalidJSONException e ) {
        std::cout << "Caught exception" << e.what();
    }
}