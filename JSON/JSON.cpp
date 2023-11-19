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
    data = new JSONElement*[100];
};

JSONObject *  JSONObject::parseObject( std::stringstream * stream ) {

    JSONObject * newObject = new JSONObject();

    char currentChar;
    bool escaped;
    bool inString;

    bool searchingForKey=true;
    bool endOfKey = false;
    bool endOfValue = false;

    std::string key;

    while( !stream->eof() ) {
        char x;
        stream->get(x);

        std::cout << x << "\n";

        if( x == ' ' || x == '\n' ) {
            stream->get(x);
            continue;
        
        }

        if( x != '{') {
            throw invalidJSONException("OBJECT DID NOT START WITH {");
            return NULL;
        }else{
            break;
        }
    }

    while( !stream->eof() ) {
        char x;
        stream->get(x);

        std::cout << "CURRENT CHAR IS " << x << '\n';

        if( x == ' ' || x == '\n' ) {
            continue;
        }

        std::cout << "END OF KEY IS " << endOfKey << '\n';
        if( endOfKey && x != ':' ) {
            throw new invalidJSONException("DID NOT FIND : AFTER KEY AT ");
        }else if( endOfKey ) {
            endOfKey = false;
            continue;
        }

        std::cout << "END OF VALUE IS " << endOfValue << '\n';

        if( endOfValue && x == '}' ) {
            break;
        }else if( endOfValue && x == ',' ) {
            endOfValue = false;
            searchingForKey = true;
            continue;
        }else if( endOfValue ) {
            std::cout << ", NOT FOUND";
            throw new invalidJSONException("DID NOT FIND , AFTER VALUE ");
        }


        if(searchingForKey) {
            if( x != '"' ) {
                throw invalidJSONException("KEY DID NOT START WITH \"");
            }
            key = JSONObject::parseJSONString( stream );
            searchingForKey = false;
            endOfKey = true;
        }else{
            if( x == '"' ) {
                std::string string = JSONObject::parseJSONString( stream );
            }else if( x == '{'){
                JSONElement * subObject = new JSONElement( JSONObject::parseObject(stream) );
            }else if( x == '[' ) {
                
            }else if( (x > 0x29 && x < 0x40) || x == '-') {
                stream->putback(x);
                double num;
                *stream >> num;
                std::cout << "num string is " << num << "\n";
            }else {
                std::string string = std::string("UNEXPECTED VALUE STARTER GOT ");
                string += x;
                throw invalidJSONException(string.c_str());
            }

            endOfValue = true;
            searchingForKey = true;
        }
    }

    return newObject;
}

JSONObject * JSONObject::parseObject( std::string object ) {
    std::stringstream stream = std::stringstream(object);
    return JSONObject::parseObject(&stream);
}

std::string JSONObject::parseJSONString( std::stringstream * stream ) {
    std::string returnString = std::string();
    bool escaped = false;
    char currentChar;
    stream->get(currentChar);
    while( !stream->eof() ){

        std::cout << currentChar << "\n";

        if( !escaped && currentChar == '"'){
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
    std::cout << "RETURNING STRING " << returnString << "\n";
    return returnString;
}


JSONElement::JSONElement(){
    type = NULLTYP;
    data = NULL;
}
JSONElement::JSONElement( double num ){
    type = NUMBER;
    data = new double(num);
}
JSONElement::JSONElement( std::string string ){
    type = STRING;
    data = new std::string(string);
}

JSONElement::JSONElement( JSONArray * array ){
    type=ARRAY;
    data = array;
}
JSONElement::JSONElement( JSONObject * object ){
    type=OBJECT;
    data = object;
}

JSONElement::~JSONElement(){
    if( type == NULLTYP ) {
        return;
    }
    delete(data);
}