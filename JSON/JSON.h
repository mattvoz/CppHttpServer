#ifndef __JSON__PARSER__
#define __JSON__PARSER__

#include <sstream>
#include <iostream>
#include <string>
#include <exception>
#include <any>
#include "hash.h"
#include <vector>

#ifndef JSONHASHSIZE
	#define JSONHASHSIZE 100
#endif

enum JSONDataType{ STRING, NUMBER, OBJECT, ARRAY, BOOLEAN, NULLTYP};

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
		JSONElement( bool );
	    bool operator==( JSONElement * );
		bool operator==(JSONElement &);
		bool operator==(long);
		bool operator==(int);
		bool operator==(double);
		bool operator==(bool);
		bool operator==( char * );
		bool operator==(std::string);
		bool operator==(JSONArray);
		void * value();
		~JSONElement();
	private:
		friend JSONObject;
		void * data;
		JSONDataType type;
};

struct JSONHOLDER{
	JSONElement * data;
	JSONHOLDER * next;
	std::string key;
};

class JSONArray{
	friend JSONObject;
	public:
		JSONArray();
		JSONArray(int);
		JSONElement * operator[](int);
		static JSONArray* parseJSONArray(  std::string );
		void put(int i, std::string str);
		void put(int i, double number);
		void put(int i, bool boolean);
		void put(int i, JSONObject * obj);
		void put(int i, JSONArray * arr);
		void deleteElement( int index );
		int length();
	private:
	    static JSONArray * parseJSONArray( std::stringstream * stream);
		std::vector<JSONElement *> arr;
};

class JSONObject{
	friend JSONArray;
	public:
		JSONObject();
		JSONObject(std::string);
		void put(std::string key, double num);
		void put (std::string key, bool boolean);
		void put(std::string key, std::string );
		void put(std::string key, JSONArray *);
		void put(std::string key, JSONObject *);
		void put( std::string key );
		JSONElement* operator[](std::string);
		JSONElement* operator[](char *);
		std::string toString();
	private:
		void parseString( std::stringstream * );
		unsigned int hash( std::string );
		static JSONObject* parseSubObject( std::stringstream * stream);
		/*decided to just use a flat value for the data array instead of instantaitng a size
		leaving size just in case change mind */
		struct JSONHOLDER* data[JSONHASHSIZE] = {};
		unsigned int size = 100;
};

static std::string parseJSONString( std::stringstream * );

#endif