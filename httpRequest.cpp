#include "httpRequest.h"

void parseQueryParams( std::stringstream & pathString, hashTable<std::string> * queryParams ) {
    if( pathString.eof() ) {
        return;
    }
    std::string curValue;
    while( ! pathString.eof() ) {
        std::string tmpKey;
        std::string tmpVal;
        //get line for query Param;
        std::getline( pathString, tmpKey, '=' );
        //Store the key into key
        std::getline( pathString, tmpVal, '&');

        queryParams->add( tmpKey, tmpVal );
    }

    return;
}

httpRequest::httpRequest( char * requestHeaders, int headersSize ) {
    rawRequest = std::string(requestHeaders, headersSize);

    std::stringstream stream(rawRequest);

    stream >> requestType;

    std::string tmpPathAndQueries;

    stream >> tmpPathAndQueries;

    stream >> httpVersion;

    std::stringstream queryStream = std::stringstream(tmpPathAndQueries);

    std::getline( queryStream, this->path, '?');

    parseQueryParams( queryStream, &queryParams);

    stream >> httpVersion;

    std::string currentLine;

    //Clear the first line :)
    std::getline( stream, currentLine );

    while( !stream.eof() ) {
        //Check headers have ended;
        if( currentLine == "\r" ){
            break;
        }
        std::string key;
        std::string value;
        std::stringstream tmpStream = std::stringstream(currentLine);
        std::getline( tmpStream, key, ':');
        std::getline( tmpStream, value);
        headers.add( key, value );

        std::getline( stream, currentLine );
    }

    // ignore all headers over 1000
    while( currentLine != "\r" ) {
        std::getline( stream, currentLine );
    }

    //TODO body parser class

}

void httpRequest::parseHeaders() {

}

httpRequest::~httpRequest() {
}