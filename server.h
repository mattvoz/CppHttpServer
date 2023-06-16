#ifndef __WINSOCK__SERVER__
#define __WINSOCK__SERVER__

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <any>
#include "hash.h"
#include "JSON.h"

enum responseType { JSON, FileResponse, GIF, PNG, javaArchive, };
class httpServer;

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

class httpRequest {
	public:
		httpRequest( char * req, int reqSize);
		~httpRequest();

		hashTable<std::string> headers = hashTable<std::string>(50);
		hashTable<std::string> queryParams = hashTable<std::string>(20);
	private:
		std::string requestType;
		std::string httpVersion;
		std::string path;
		std::string rawRequest;
};

class httpResponse {
	public:
		httpResponse( SOCKET recipient );
		~httpResponse();

		void setStatus( unsigned int);
		void setStatus(std::string);
		void JSON( std::string object );
		void JSON( JSONObject object );
		void file( std::string filePath );
		void file( FILE * file );
		void sendFile();
		void sendResponse();
		void appendStatus( std::string * response );

		int getStatus();

		hashTable<std::string> headers = hashTable<std::string>(50);
	private:
		SOCKET target;
		std::string statusCode;
		std::any body;
		responseType type;
		FILE ** files;

};

struct threadData {
	char * data;
	long size;
	SOCKET client;
	httpServer * server;
};

class httpServer {
	public:
		httpServer();
		void serverListen( std::string port);
		void use( void * (* middleware ) ( void * req, void * res) );
		void GET( std::string route, void * (* func) (void * req, void * res) );
		void POST( std::string route, void * (* func) (void * req, void * res) );
		void PUT( std::string route, void * (* func) (void * req, void * res ) );
		void RDELETE( std::string route, void * (* func) (void * req, void * res ) );
		void applyMiddleWare();
		void doRouting();
		~httpServer();
	private:
		std::string port;
		std::vector< void (*)( httpRequest * req, httpResponse * res)> middleWare;
		void findRoute();
		SOCKET listenSocket;
		void prepSocket( std::string port );

};

void readRequest( void * threadData );
void parseRequest( void * threadData );

#endif