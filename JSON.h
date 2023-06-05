#ifndef __JSON__PARSER__
#define __JSON__PARSER__

#include <string>
#include <any>
#include "hash.h"

/**
 * Uses a hashtable of std::any to store values so casts are required with std::cast_any
*/
class JSONObject{
	public:
		JSONObject();
		JSONObject(std::string object);
		JSONObject( char * , int objectSize );

		std::string operator[](std::string);

		std::string toString();

		hashContainer<void *> data;
	private:

};

#endif