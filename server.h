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

enum responseType { JSON, FileResponse, GIF, PNG, javaArchive, };

static int stringHashFunction( std::string key, int tablesize ) {
	int total = 0;
	int shift = key.length() % 2 == 0 ? 1 : 2;
	for(int i = 0; i < key.length(); i++) {
		total += key[i] << shift;
	}

	return total % tablesize;
}

template <typename T>
struct hashContainer {
	std::string key;
	T * value;
	hashContainer * next;
};

template <typename T>
class hashTable {
	public:
		hashTable() {
			values = new hashContainer<T>*[100];
			for(int i = 0; i < size; i++) {
				values[i] = NULL;
			}
			this->size = 100;
		};

		hashTable(int tableSize) {
			this->size = tableSize;
			values = new hashContainer<T>*[tableSize];
			for(int i = 0; i < tableSize; i++) {
				values[i] = NULL;
			}
		};

		void add( std::string key, T value) {
			int pos = stringHashFunction(key, size);
			hashContainer<T> * newVal = new struct hashContainer<T>();
			newVal->key = key.c_str();
			newVal->value = new T();
			*(newVal->value) = value;
			newVal->next = NULL;

			hashContainer<T> * tmp = values[pos];

			if( tmp == NULL ) {
				values[pos] = newVal;
				return;
			}

			if( tmp->key == key ) {
				delete newVal;
				*(tmp->value) = value;
				return;
			}

			while( tmp->next != NULL ) {
				tmp = tmp->next;
				if( tmp->key == key ) {
					delete newVal;
					*(tmp->value) = value;
					return;
				}
			}

			tmp->next = newVal;
			return;
		};

		T * operator[](std::string key) {
			int hashVal = stringHashFunction(key, size);
			hashContainer<T> * cur = values[hashVal];

			while(cur->next != NULL) {
				if(cur->key == key) {
					return cur->value;
				}
				cur = cur->next;
			}

			if(cur->key == key) {
				return cur->value;
			}
			
			return NULL;
		};

		std::string toString() {
			static std::string response = std::string();
			for( int i = 0; i < size; i++ ) {
				hashContainer<T> * current = values[i];
				while( current != NULL ) {
					response += current->key + ":" + * current->value + "\n";
					current = current->next;
				}
			}
			return response;
		}

		~hashTable() {
			for(int i = 0; i < size; i++) {
				struct hashContainer<T> * cur = values[i];
				while( cur->next != NULL ) {
					struct hashContainer<T> * tmp = cur;
					cur = cur->next;
					delete tmp->value;
					delete tmp;
				}
			}
		};
	private:
		int size;
		hashContainer<T> ** values;
		
};

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

/**
 * Uses a hashtable of std::any to store values so casts are required with std::cast_any
*/
class JSONObject{
	public:
		JSONObject();
		JSONObject(std::string object);
		JSONObject( char * , int objectSize);

		std::string toString();

		hashContainer<std::any> data;
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
		void JSON();
		void sendFile();
		void sendResponse();
		void appendStatus( std::string * response );

		int getStatus();

		hashTable<std::string> headers = hashTable<std::string>(50);
	private:
		SOCKET target;
		std::string statusCode;
		std::string body;
		responseType type;
		FILE ** file;

};

struct threadData {
	char * data;
	int size;
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
		std::vector< void * (*)( void *, void * )> middleWare;
		void findRoute();
		SOCKET listenSocket;
		void prepSocket( std::string port );

};

void readRequest( void * threadData );
void parseRequest( void * threadData );
void parseQueryParams( std::stringstream & pathString, hashTable<std::string>* queryParams );

#endif