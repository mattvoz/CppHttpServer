#ifndef __HTTP__RESPONSE__
#define __HTTP__RESPONSE__

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

enum responseType { unkown, JSON_RES, FileResponse, GIF, PNG, javaArchive, text_plain, image_jpeg, image_png, image_svg,};
class httpResponse {
	public:
		httpResponse( SOCKET recipient );
		~httpResponse();

		void setStatus( unsigned int);
		void setStatus(std::string);
		void textplain(std::string data);
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
		responseType type = unkown;
		std::string text;
		FILE ** files;
		JSONObject * jsonRes = NULL;

};

#endif