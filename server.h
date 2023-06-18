#ifndef __WINSOCK__SERVER__
#define __WINSOCK__SERVER__

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <functional>
#include <any>
#include "hash.h"
#include "JSON.h"
#include "httpRequest.h"
#include "httpResponse.h"

class httpServer;

struct routeInfo{
	void (* func) (httpRequest * req, httpResponse * res);
	void (* middleWare[10]) (void * req, void * res);
};

struct addrinfo * getSocketAddressInfo( PCSTR port );
int initWinsock();

class Router {
	public: 
		Router();
		void GET();
		void POST();
		void PUT();
		void RDELETE();
		friend httpServer;
	private:
};

struct threadData {
	char * headers;
	long size;
	SOCKET client;
	httpServer * server;
};

class httpServer {
	public:
		httpServer();
		void serverListen( std::string port);
		void use( void * (* middleware ) ( void * req, void * res) );
		void GET( std::string route, void (* func) (httpRequest * req, httpResponse * res));
		void POST( std::string route, void (* func) (httpRequest * req, httpResponse * res) );
		void POST( std::string route, void (* func) (httpRequest * req, httpResponse * res), ... );
		void PUT( std::string route, void (* func) (httpRequest * req, httpResponse * res ) );
		void PUT( std::string route, void (* func) (httpRequest * req, httpResponse * res ), ... );
		void RDELETE( std::string route, void (* func) (httpRequest * req, httpResponse * res ) );
		void RDELETE( std::string route, void (* func) (httpRequest * req, httpResponse * res ), ... );
		void applyMiddleWare();
		void doRequest( std::string requestType, std::string route, httpRequest * req, httpResponse * res );
		~httpServer();
	private:
		std::string port;
		std::vector< void (*)( httpRequest * req, httpResponse * res)> middleWare;
		hashTable<struct routeInfo> getRoutes = hashTable<struct routeInfo>();
		/*
		hashTable<struct routeInfo> postRoutes;
		hashTable<struct routeInfo> putRoutes;
		hashTable<struct routeInfo> deleteRoutes;
		*/
		SOCKET listenSocket;
		void prepSocket( std::string port );

};

void readRequest( void * threadData );
void parseRequest( void * threadData );

#endif