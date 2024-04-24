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

class invalidJSONException : public std::exception{
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
	    bool operator==( JSONElement * );
		bool operator==(JSONElement &);
		bool operator==(long);
		bool operator==(int);
		bool operator==(double);
		bool operator==( char * );
		bool operator==(std::string);
		bool operator==(JSONArray);
		void * value();
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

class JSONObject{
	public:
		JSONObject();
		JSONObject(std::string);
		void put(std::string key, double num);
		void put(std::string key, std::string );
		void put(std::string key, JSONArray *);
		void put(std::string key, JSONObject *);
		JSONElement* operator[](std::string);
		JSONElement* operator[](char *);
		std::string toString();
	private:
		JSONObject( std::stringstream * sub );
		void parseString( std::stringstream * );
		unsigned int hash( std::string );
		static JSONObject* parseSubObject( std::stringstream * stream);
		static std::string parseJSONString( std::stringstream * );
		//decided to just use a flat value fo the data array instead of instantaitng a size
		struct JSONHOLDER* data[100];
};

#endif