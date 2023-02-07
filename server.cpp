#include "server.h"
#include <stdio.h>
#include <iostream>
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

httpRequest::httpRequest( char * req, int reqSize ) {
    rawRequest = std::string(req, reqSize);

    std::cout << rawRequest;

    std::stringstream stream(rawRequest);

    stream >> requestType;
    stream >> path;
    stream >> httpVersion;

    std::cout << requestType << "\n" << path << "\n" << httpVersion << "\n";

    std::string currentLine;

    //Clear the first line :)
    std::getline( stream, currentLine );

    while( std::getline( stream, currentLine ) ) {
        if( currentLine == "\r" ){
            printf("end of headers\n");
            break;
        }
        std::string key;
        std::string * value = new std::string();
        std::stringstream tmpStream = std::stringstream(currentLine);
        std::getline( tmpStream, key, ':');
        std::getline( tmpStream, *value);
        headers.add( key, value );
    }

}

httpRequest::~httpRequest() {
    delete &headers;
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
        int buffSize = 200;
        char * recBuf = (char *) malloc(sizeof(char) * buffSize);

        if( recBuf == NULL) {
            printf("failed to allocate buffer for connection skipping");
            continue;
        }

        printf("awaiting connection \n");
        SOCKET clientSock = INVALID_SOCKET;

        sockaddr_in from;
        int fromlen = sizeof(from);

        clientSock = accept(listenSocket, (sockaddr *) &from, &fromlen);

        if(clientSock == INVALID_SOCKET) {
            printf("failed to accept connection to socket\n");
            closesocket(listenSocket);
            WSACleanup();
            exit(1);
        }

        int bytesRecieved = 200;

        while( bytesRecieved >= buffSize) {
            buffSize += 10;
            recBuf = (char *) realloc(recBuf, sizeof(char) * buffSize);
            bytesRecieved = recv(clientSock, recBuf, buffSize, MSG_PEEK);
        }

        buffSize = bytesRecieved;
        if( realloc( recBuf, sizeof(char) * buffSize) == NULL) {
            printf("failed to update size of buffer canceling request");
            closesocket( clientSock );
        }

        bytesRecieved = recv(clientSock, recBuf, buffSize, 0);

        struct threadData * data = new threadData();

        data->data = recBuf;
        data->size = buffSize;
        data->client = clientSock;
        data->server = this;

        CreateThread( 0, 0, (LPTHREAD_START_ROUTINE) &parseRequest, data, 0, NULL);
    }
}

httpServer::~httpServer() {

}

void httpServer::parseRequest( void * threadData ) {
    struct threadData * threadInfo = ((struct threadData * )threadData);
    int size = threadInfo->size;

    char type[5];
    char path[100];
    char version[20];
    sscanf(threadInfo->data, "%s %s %s", type, path, version);

    printf("making request object\n");
    httpRequest * req = new httpRequest( threadInfo->data, threadInfo->size);
    printf("finished request object\n");

    if( strcmp(type, "GET") != 0 && strcmp(type, "POST") != 0 && strcmp(type,"PUT") != 0 && strcmp(type, "DELETE") != 0 ) {
        printf("unrecognized http type closing socket\n");
        closesocket(threadInfo->client);
        return;
    }

    std::string tmpstr(threadInfo->data, threadInfo->size);

    std::stringstream requestStream = std::stringstream(tmpstr);
    std::string currentLine;

    //Clear the GET / 
    std::getline( requestStream, currentLine );

    while( std::getline( requestStream, currentLine ) ) {
        if( currentLine == "\r" ){
            printf("end of headers\n");
            break;
        }
        std::string key,value;
        std::stringstream tmpStream = std::stringstream(currentLine);
        std::getline( tmpStream, key, ':');
        std::getline( tmpStream, value);
    }
    closesocket( threadInfo->client );
    printf("closed socket");
}