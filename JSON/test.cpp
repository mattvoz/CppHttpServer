#include "JSON.h"
#include <string.h>

int main() {
    try{
        const char * string = "{\"hi\" : \"hey\", \"testtwo\": null, \"num\": -123, \"test\": [\"hi\", \"hello\", \"hi there\", {\"test\": 123}]}";
        printf("length %d", strlen(string));
        JSONObject object = JSONObject(string);
        JSONElement * x = object["num"];
        std::cout <<  *(double *)x->value() << std::endl;
    }catch(invalidJSONException e ) {
        std::cout << "Caught exception" << e.what();
    }
}