#include "httpResponse.h"


httpResponse::httpResponse( SOCKET recipient ) {
    this->target = recipient;
}

void httpResponse::appendStatus( std::string * response ) {
    int status = atoi( this->statusCode.c_str() );
    if( 100 <= status && status < 200 ) {
        switch(status) {
            case 101:
                *response += this->statusCode + " Switching Protocols";
                break;
            case 102:
                *response += this->statusCode + " Processing";
            default:
                *response += this->statusCode + "Continue";
        }
    }
    if( 200 <= status && status < 300) {
        switch( status ){
            case 201:
                *response += this->statusCode + " CREATED";
            case 202:
                *response += this->statusCode + " Accepted";
            case 203:
                *response += this->statusCode + " Non-Authoritative Information";
                break;
            case 204:
                *response += this->statusCode + " No Content";
            case 205:
                *response += this->statusCode + " Reset Content";
            case 206:
                *response += this->statusCode + " Partial Content";
            case 207:
                *response += this->statusCode + " Multi-Status";
            case 208:
                *response += this->statusCode + "Already Reported";
            default:
                *response += this->statusCode + " OK";
        };
    } else if( 300 <= status && status < 400) {
        switch( status ) {
            case 300:
                *response += this->statusCode += " Multiple Choices";
                break;
            default:
                *response += this->statusCode += " Redirect";
        }
    } else if( 400 <= status && status < 500) {
        *response += this->statusCode + " BAD REQUEST";
    } else if( 500 <= status ) {
        *response += this->statusCode + " INTERNAL SERVER ERROR";
    }else {
        *response += "200 OK";
    }

    *response += "\r\n";
}

void httpResponse::textplain( std::string data ) {
    type = text_plain;
    text = data;
}

void httpResponse::JSON( std::string data ) {
    type = JSON_RES;
    text = data;
}

void httpResponse::JSON( JSONObject data ) {
    type = JSON_RES;
    text = data.toString();
}

void httpResponse::sendResponse() {
    std::string response;

    response += "HTTP/1.1 ";

    appendStatus( &response );

    std::string body;
    char lenBuf[9];
    switch( type ) {
        case text_plain:
            snprintf(lenBuf, 9, "%d", text.length());
            headers.add("Content-Length", lenBuf);
            headers.add("Content-Type", "text/plain");
            body = text;
        break;
        case JSON_RES:
            if(jsonRes != NULL){
                //body = jsonRes->toString();
            } else {
                body = text;
            }
            snprintf(lenBuf, 9, "%d", body.length());
            headers.add("Content-Type", "text/JSON");
            headers.add("Content-Length", lenBuf);
        break;
        default:
            headers.add("Content-Length", "0");
        break;
    }
    response += headers.toString() + body;
    printf("response: %s", response.c_str());
    send( this->target, response.c_str(), response.size(), 0);
};

void httpResponse::setStatus( unsigned int status ) {
    if(  600 < status ) {
        printf("status isn't a proper status setting to 200 by default\n");
        this->statusCode = "200";
        return;
    }

    this->statusCode = std::to_string(status);

}

void httpResponse::setStatus( std::string status ){

    unsigned int statusInt = atoi( status.c_str() );

    if(  600 < statusInt ) {
        printf("status isn't a proper status setting to 200 by default\n");
        this->statusCode = "200";
        return;
    }

    this->statusCode = status;

}

httpResponse::~httpResponse() {
    
}