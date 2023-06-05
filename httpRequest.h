#ifndef __HTTP__REQUEST__
#define __HTTP__REQUEST__

#include "hash.h"
#include <string>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include<string>

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

#endif