#ifndef __HTTP__REQUEST__
#define __HTTP__REQUEST__

#include "hash.h"
#include <string>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include<string>

void parseQueryParams( std::stringstream & pathString, hashTable<std::string> * queryParams );

class httpRequest {
	public:
		httpRequest( char * req, int reqSize);
		~httpRequest();

		hashTable<std::string> headers = hashTable<std::string>(50);
		hashTable<std::string> queryParams = hashTable<std::string>(20);
	private:
		void parseHeaders();
		std::string requestType;
		std::string httpVersion;
		std::string path;
		std::string rawRequest;
		std::string rawHeaders;
};

#endif