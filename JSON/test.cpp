#include "JSON.h"
#include <string.h>

int main() {
    try{
        const char * string = "{\"hi\" : \"hey\", \"testtwo\": null, \"num\": -123, \"test\": [\"hi\", 1234, \"testing\"]}";
        printf("length %d", strlen(string));
        JSONObject object = JSONObject(string);
        object.put(std::string("hi"), (double)1234);
        JSONElement * x = object["hi"];
        std::cout << "got object " << x << std::endl;
        std::cout << "does x = 1234? . " << (*x == 1234) << std::endl;
    }catch(invalidJSONException e ) {
        std::cout << "Caught exception" << e.what();
    }
}