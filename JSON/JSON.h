#ifndef __JSON__PARSER__
#define __JSON__PARSER__

#include <sstream>
#include <iostream>
#include <string>
#include <exception>
#include <any>
#include "hash.h"

enum JSONDataTypes{ STRING, NUMBER, OBJECT, ARRAY, EMPTY};

class invalidJSONException: public std::exception{
	public:
		explicit invalidJSONException( const char * message );
		virtual const char* what() const throw();

	protected:
		std::string msg;
};

class JSONChild{
	public:
	private:
		double num;
		std::string string;
		JSONChild * next;
};


/**
 * Uses a hashtable of std::any to store values so casts are required with std::cast_any
*/
class JSONObject{
	public:
		JSONObject();
		JSONObject( char * , int objectSize );

		std::string operator[](std::string);
		std::string toString();
		static JSONObject parseObject( std::string JSONString );
	private:
		JSONObject( std::stringstream * sub );
		static JSONObject parseSubObject( std::stringstream * stream);
		static std::string parseJSONString( std::stringstream * );
		hashContainer<JSONChild> data;

};

#endif