#include "JSON.h"
#include <sstream>
#include <iostream>


invalidJSONException::invalidJSONException( const char * message ) {
		msg = std::string(message);
}

const char* invalidJSONException::what() const throw() {
    return msg.c_str();
}

JSONObject::JSONObject(){}

JSONObject::JSONObject( std::string string ) {
    std::stringstream stream = std::stringstream(string);
    this->parseJSONString( &stream );
}

void  JSONObject::parseString( std::stringstream * stream ) {
    std::cout << "string" << std::endl;
    JSONObject * newObject = new JSONObject();
    std::cout << "allocated" << std::endl;

    char currentChar;
    bool escaped;
    bool inString;

    bool searchingForKey=true;
    bool endOfKey = false;
    bool endOfValue = false;

    std::string key;

    char x;
    stream->get(x);
    std::cout << x << std::endl;

    if( x != '{') {
        std::cout << "invalid start char" << std::endl;
        throw invalidJSONException("OBJECT DID NOT START WITH {");
    }

    std::cout << stream->eof() << std::endl;
    fflush(stdout);

    while( !stream->eof() ) {
        std::cout << "parsing stream" << std::endl;
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
            std::cout << key << std::endl;
            searchingForKey = false;
            endOfKey = true;
        }else{
            if( x == '"' ) {
                std::string string = JSONObject::parseJSONString( stream );
            }else if( x == '{'){
                JSONElement * subObject = new JSONElement( this->parseSubObject(stream) );
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
}

//TODO
JSONObject* JSONObject::parseSubObject(std::stringstream * stream){
    return NULL;
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

unsigned int JSONObject::hash( std::string key ) {
    int hashVal=0;
    for(int i = 0 ; i < key.length(); i++ ) {
        hashVal += ( ( key[i] << 2 ) >> 4 ) * 17;
    }

    return hashVal;
}

JSONElement * JSONObject::operator[]( std::string key){
    struct JSONHOLDER * cur = this->data[hash(key)];

    while( cur != NULL ) {
        if( cur->data->key == key ){
            return cur->data;
        }
        cur = cur->next;
    }

    return (JSONElement *) NULL;
}

JSONElement * JSONObject::operator[]( char * key){
    struct JSONHOLDER * cur = this->data[hash(std::string(key))];

    while( cur != NULL ) {
        if( cur->data->key == key ){
            return cur->data;
        }
        cur = cur->next;
    }

    return (JSONElement *) NULL;
}

void JSONObject::put( std::string key, std::string val ) {
    unsigned int hashVal = hash(key);

    struct JSONHOLDER * cur = this->data[hashVal];
    if( this->data[hashVal] == NULL ) {
        struct JSONHOLDER * holder = new struct JSONHOLDER();
        holder->data = new JSONElement(val);
        holder->next = NULL;
        return;
    }

    while(cur->next != NULL) {
        cur = cur->next;
        if( cur->data->key == key ) {
            cur->data->data;
        }
    }

    cur->next = NULL;
}

void JSONObject::put( std::string key, JSONArray * arr ) {
    unsigned int hashVal = hash(key);

    struct JSONHOLDER * cur = this->data[hashVal];
    if( this->data[hashVal] == NULL ) {
        struct JSONHOLDER * holder = new struct JSONHOLDER();
        holder->data = new JSONElement(arr);
        holder->next = NULL;
        return;
    }

    while(cur->next != NULL) {
        cur = cur->next;
        if( cur->data->key == key ) {
            cur->data->data;
        }
    }

    cur->next = NULL;
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

void * JSONElement::value(){
    return data;
}

JSONElement::~JSONElement(){
    if( type == NULLTYP ) {
        return;
    }
    delete(data);
}