#ifndef __JSON__PARSER__
#define __JSON__PARSER__

#include <sstream>
#include <iostream>
#include <string>
#include <exception>
#include <any>
#include "hash.h"

enum JSONDataType{ STRING, NUMBER, OBJECT, ARRAY, NULLTYP};

class JSONArray;
class JSONObject;

class invalidJSONException: public std::exception{
	public:
		explicit invalidJSONException( const char * message );
		virtual const char* what() const throw();

	protected:
		std::string msg;
};

class JSONElement{
	public:
		JSONElement();
		JSONElement( double num );
		JSONElement( std::string );
		JSONElement( JSONArray *  );
		JSONElement( JSONObject * );
		~JSONElement();
	private:
		friend JSONObject;
		std::string key;
		void * data;
		JSONDataType type;
};

struct JSONHOLDER{
	JSONElement * data;
	JSONHOLDER * next;
};

class JSONArray{
	public:
		JSONElement * operator[](int);
		static JSONArray* parseArray(  std::string );
	private:
		JSONElement * arr;
};

/**
 * Uses a hashtable of std::any to store values so casts are required with std::cast_any
*/
class JSONObject{
	public:
		JSONObject();
		JSONObject( char * , int objectSize );

		void put(std::string key, double num);
		void put(std::string key, std::string );
		void put(std::string key, JSONArray *);
		void put(std::string key, JSONObject *);
		JSONElement* operator[](std::string);
		std::string toString();
		static JSONObject* parseObject( std::string JSONString );
	private:
		JSONObject( std::stringstream * sub );
		unsigned int hash( std::string );
		static JSONObject* parseObject( std::stringstream * JSONString );
		static JSONObject* parseSubObject( std::stringstream * stream);
		static std::string parseJSONString( std::stringstream * );
		struct JSONHOLDER ** data;
		const JSONObject * nullObj = new JSONObject();

};

#endif