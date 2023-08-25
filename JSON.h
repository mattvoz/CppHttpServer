#ifndef __JSON__PARSER__
#define __JSON__PARSER__

#include <sstream>
#include <iostream>
#include <string>
#include <any>
#include "hash.h"

enum JSONDataTypes{ STRING, NUMBER, OBJECT, ARRAY};
struct JSONMember{
	JSONDataTypes type;
	std::string key;

};


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

	private:
		void parseString( std::string JSONString );
		hashContainer<struct JSONMember *> data;

};

#endif