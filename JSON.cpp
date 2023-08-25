#include "JSON.h"
#include <sstream>
#include <iostream>

JSONObject::JSONObject() {
    data = hashContainer<void *>();
};

JSONObject::JSONObject( std::string object ) {
    data = hashContainer<void *>();
}

void JSONObject::parseString( std::string object ) {
    char currentChar;
    bool escaped;
    bool inString;

    std::stringstream stream(object);

    std::string returnString = std::string();

    bool searchingForKey=true;

    while( !stream.eof() ) {
        char x;
        stream.get(x);
        if( )

    }
}