#include "JSON.h"
#include <sstream>
#include <iostream>


invalidJSONException::invalidJSONException( const char * message ) {
		msg = std::string(message);
}

const char* invalidJSONException::what() const throw() {
    return msg.c_str();
}


JSONObject::JSONObject() {
    data = hashContainer<JSONChild>();
};

JSONObject JSONObject::parseObject( std::string object ) {

    JSONObject newObject = JSONObject();

    char currentChar;
    bool escaped;
    bool inString;

    std::stringstream stream(object);

    std::string returnString = std::string();

    bool searchingForKey=true;
    bool endOfKey = false;
    bool endOfValue = false;

    std::string key;

    while( !stream.eof() ) {
        char x;
        stream.get(x);

        std::cout << x << "\n";

        if( x == ' ' || x == '\n' ) {
            stream.get(x);
            continue;
        
        }

        if( x != '{') {
            throw invalidJSONException("OBJECT DID NOT START WITH {");
            return NULL;
        }else{
            break;
        }
    }

    while( !stream.eof() ) {
        char x;
        stream.get(x);

        if( x == ' ' || x == '\n' ) {
            stream.get(x);
            continue;
        
        }

        if( endOfKey && x != ',' ) {
            throw new invalidJSONException("DID NOT FIND , AFTER KEY AT ");
        }


        if(searchingForKey) {
            if( x != '"' ) {
                throw invalidJSONException("KEY DID NOT START WITH \"");
            }
            key = JSONObject::parseJSONString( &stream );
            searchingForKey = false;
            endOfKey = true;
        }else{

            if( x == '"' ) {
                std::string string = JSONObject::parseJSONString( &stream );
            }else if( x == '{'){
                JSONObject subObject = parseSubObject( &stream );

            }else if( x == '[' ) {
                
            }else if( (x > 0x29 && x < 0x40) || x == '-') {
                std::string num;
                stream >> num;
                char * end;
                double val = strtod( num.c_str(), &end );
            }else {
                std::string string = std::string("UNEXPECTED VALUE STARTER GOT ");
                string += x;
                throw invalidJSONException(string.c_str());
            }
        }
    }

    return newObject;
}

JSONObject JSONObject::parseSubObject(std::stringstream * stream ) {

}

std::string JSONObject::parseJSONString( std::stringstream * stream ) {
    std::string returnString = std::string();
    bool escaped = false;
    char currentChar;
    stream->get(currentChar);
    while( !stream->eof() ){

        std::cout << currentChar << "\n";

        if( !escaped && currentChar == '"'){
            stream->get();
            break;
        }

        if( !escaped && currentChar == '\\') {
            escaped = true;
            stream->get(currentChar);
            continue;
        }

        if( escaped ) {
            switch(currentChar) {
                case 'n':
                    returnString+="\n";
                    break;
                case '\\':
                    returnString += '\\';
                    break;
                default:
                    throw invalidJSONException("INVALID ESCAPED CHARACTER IN STRING");
                    break;
            }
        }
        escaped = false;
        returnString+=currentChar;
        stream->get(currentChar);
    }
    return returnString;
}