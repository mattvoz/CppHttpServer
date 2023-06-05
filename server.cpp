#include "server.h"
#include <stdio.h>
#include <iostream>
#include <sstream>
#include<string>

int initWinsock() {
    WSADATA sockData;

    struct addrinfo * addrResult = NULL;

    return WSAStartup( MAKEWORD(2,2), &sockData );
}

struct addrinfo * getSocketAddressInfo( PCSTR port ) {
    int result;
    struct addrinfo * addrResult = NULL;

    struct addrinfo hints;

    memset( &hints, 0, sizeof(hints) );

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    result = getaddrinfo(NULL, port, &hints, &addrResult);

    if( result != 0) {
        printf("failed to get socket address info with error %d", result);
        exit;
    }

    return addrResult;
}

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
    }

    *response += "\n";
}

void httpResponse::sendResponse() {
    std::string response;

    response += "HTTP/1.1 ";

    appendStatus( &response );

    headers.add("Content-Type", "text/html\n");
    headers.add("Content-Length", "2");

    response += headers.toString();

    response += "hi";

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


httpServer::httpServer() {
    listenSocket = INVALID_SOCKET;

    if( initWinsock() != 0) {
        printf("failed to  init winsock");
        exit(1);
    }
}

void httpServer::prepSocket( std::string port ) {
    struct addrinfo * addrResult = getSocketAddressInfo( ( port.c_str() ) );

    listenSocket = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol);

    if( listenSocket == INVALID_SOCKET) {
        printf("error at socket(): %d\n", WSAGetLastError());
        freeaddrinfo(addrResult);
        WSACleanup();
        exit(1);
    }

    if( bind(listenSocket, addrResult->ai_addr, (int) addrResult->ai_addrlen) == SOCKET_ERROR ) {
        printf("failed to bind socket with error: %d\n", WSAGetLastError());
        freeaddrinfo(addrResult);
        closesocket(listenSocket);
        WSACleanup();
        exit(1);
    }

    freeaddrinfo(addrResult);

    if ( listen( listenSocket, SOMAXCONN ) == SOCKET_ERROR ) {
        printf( "listenSock failed with error: %ld\n", WSAGetLastError() );
        closesocket(listenSocket);
        WSACleanup();
        exit(1);
    }
}

void httpServer::serverListen( std::string port ) {

    this->prepSocket(port);

    while(true) {
        SOCKET clientSock = INVALID_SOCKET;

        sockaddr_in from;
        int fromlen = sizeof(from);

        clientSock = accept(listenSocket, (sockaddr *) &from, &fromlen);

        if( clientSock == INVALID_SOCKET) {
            printf("failed to accept connection to socket\n");
            closesocket(listenSocket);
            WSACleanup();
            exit(1);
        }

        struct threadData * data = new threadData();

        data->data = NULL;
        data->size = NULL;
        data->client = clientSock;
        data->server = this;

        CreateThread( 0, 0, (LPTHREAD_START_ROUTINE) &readRequest, data, 0, NULL);
    }
}

httpServer::~httpServer() {
    closesocket(listenSocket);
    WSACleanup();
}


//TODO change to non blocking socket
void readRequest( void * threadData ) {
        struct threadData * threadInfo = ( (struct threadData * )threadData );

        int buffSize = 1000;
        char * recBuf = (char *) malloc(sizeof(char) * buffSize);
        int bytesRecieved = 1000;

        /*
        u_long mode = 1;
        ioctlsocket(threadInfo->client, FIONBIO, &mode);
        */

        while( bytesRecieved >= buffSize) {
            buffSize += 2;
            recBuf = (char *) realloc(recBuf, sizeof(char) * buffSize);
            if( recBuf == NULL ) {
                closesocket( threadInfo->client );
                return;
            }
            bytesRecieved = recv( threadInfo->client, recBuf, buffSize, MSG_PEEK );
            printf("%s\n", recBuf);
            if( strstr(recBuf, "\r\n\r\n") != NULL ) {
                
            }
        }

        buffSize = bytesRecieved;
        if( realloc( recBuf, sizeof(char) * buffSize) == NULL) {
            printf("failed to update size of buffer canceling request\n");
            closesocket( threadInfo->client );
            return;
        }

        bytesRecieved = recv( threadInfo->client, recBuf, buffSize, 0);

        threadInfo->data = recBuf;
        threadInfo->size = bytesRecieved;

        parseRequest( threadInfo );
}

void parseRequest( void * threadData ) {
    struct threadData * threadInfo = ( (struct threadData * )threadData );
    int size = threadInfo->size;

    char type[5];
    char path[100];
    char version[20];
    sscanf( threadInfo->data, "%s %s %s", type, path, version );

    httpRequest * req = new httpRequest( threadInfo->data, threadInfo->size );


    std::string tmpstr( threadInfo->data, threadInfo->size );

    std::stringstream requestStream = std::stringstream( tmpstr );
    std::string currentLine;

    httpResponse * res = new httpResponse( threadInfo->client );

    res->setStatus(200);

    res->sendResponse();

    int error = closesocket( threadInfo->client );
    if(error == SOCKET_ERROR) {
        printf("failed to close the socket\n");
    }
    printf("closed socket\n");

}

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
