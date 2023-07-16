#include "server.h"
#include <stdio.h>
#include <iostream>
#include <sstream>
#include<string>

int initWinsock() {
    WSADATA sockData;
    return WSAStartup( MAKEWORD(2,2), &sockData );
}

/**
 * Get socket prepped for connection
*/
struct addrinfo * getSocketAddressInfo( std::string address, PCSTR port ) {
    int result;
    struct addrinfo * addrResult = NULL;

    struct addrinfo localaddr;

    memset( &localaddr, 0, sizeof(localaddr) );

    localaddr.ai_family = AF_INET;
    localaddr.ai_socktype = SOCK_STREAM;
    localaddr.ai_protocol = IPPROTO_TCP;
    localaddr.ai_flags = AI_PASSIVE;

    result = getaddrinfo(address.c_str(), port, &localaddr, &addrResult);

    if( result != 0) {
        printf("failed to get socket address info with error %d", result);
        fflush(stdout);
        exit;
    }

    return addrResult;
}

void httpServer::get( std::string route, void (* func) (httpRequest * req, httpResponse * res) ){
    struct routeInfo * info = new routeInfo();
    if( info == NULL ) {
        exit(1);
    }
    info->func = func;
    paths.addPath(route, info, GET);
}

httpServer::httpServer() {
    fflush(stdout);
    listenSocket = INVALID_SOCKET;

    if( initWinsock() != 0) {
        printf("failed to  init winsock\n");
        fflush(stdout);
        exit(1);
    }

}

void httpServer::staticServe( std::string path ) {
    struct routeInfo info = routeInfo();
}

void httpServer::prepSocket( std::string address, std::string port ) {
    struct addrinfo * addrResult = getSocketAddressInfo( address, ( port.c_str() ) );

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

void httpServer::serverListen( std::string address, std::string port ) {

    this->prepSocket(address, port);

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

        data->headers = NULL;
        data->size = 0;
        data->client = clientSock;
        data->server = this;

        CreateThread( 0, 0, (LPTHREAD_START_ROUTINE) &readRequest, data, 0, NULL);
    }
}

void httpServer::applyMiddleWare(){

}

httpServer::~httpServer() {
    closesocket(listenSocket);
    WSACleanup();
}


void httpServer::doRequest( std::string requestType, std::string route, httpRequest * req, httpResponse * res ) {\
    printf("processing request\n");
    struct treeNode * node = paths.getPath(route);
    if( node == NULL ) {
        printf("no route found\n");
        res->setStatus(404);
        return;
    }

    if( requestType == "GET" ) {
        
        struct routeInfo * routeDetails = node->pathFunctions[GET];

        if( routeDetails == NULL ) {
            printf("no route found\n");
            res->setStatus(404);
            return;
        }
        for(int i = 0 ; i < 10; i++ ) {
            if( routeDetails->middleWare[i] == NULL ){
                break;
            }
            routeDetails->middleWare[i]( req, res );
        }

        routeDetails->func( req, res );
    }

}


//TODO change to non blocking socket
void readRequest( void * threadData ) {
        struct threadData * threadInfo = ( (struct threadData * )threadData );

        long buffSize = 1000;
        char * recBuf = (char *) malloc(sizeof(char) * buffSize);
        long bytesRecieved = 1000;

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
            recv( threadInfo->client, recBuf, buffSize, MSG_PEEK );
            char * address;
            if( ( address = strstr(recBuf, "\r\n\r\n") ) != NULL || ( address = strstr(recBuf, "\n\n") ) ) {
                buffSize = (long) (address - recBuf);
                break;
            }
        }

        buffSize = bytesRecieved;
        if( realloc( recBuf, sizeof(char) * buffSize) == NULL) {
            printf("failed to update size of headers buffer canceling request\n");
            closesocket( threadInfo->client );
            return;
        }

        bytesRecieved = recv( threadInfo->client, recBuf, buffSize, 0);
        if( bytesRecieved < 0 ) {
            printf("failed to read from socket closing socket");
            closesocket( threadInfo->client );
            return;
        }

        threadInfo->headers = recBuf;
        threadInfo->size = bytesRecieved;

        parseRequest( threadInfo );
}

void parseRequest( void * threadData ) {
    struct threadData * threadInfo = ( (struct threadData * )threadData );
    int size = threadInfo->size;

    char type[5];
    char path[100];
    char version[20];
    sscanf( threadInfo->headers, "%s %s %s", type, path, version );

    printf("%s %s %s\n", path, type, version);

    httpRequest * req = new httpRequest( threadInfo->headers, threadInfo->size );
    httpResponse * res = new httpResponse( threadInfo->client );

    threadInfo->server->doRequest(type, path, req, res);

    res->sendResponse();

    int error = closesocket( threadInfo->client );
    if(error == SOCKET_ERROR) {
        printf("failed to close the socket\n");
    }

}
