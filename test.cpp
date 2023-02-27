#include "server.h"
#include <iostream>
#include <sstream>

int main(int argc, char ** argv) {
    httpServer serv = httpServer();

    hashTable<std::string> x = hashTable<std::string>( 50 );

    std::string testString = "ahhhhh\n";

    x.add("hi", testString);

    std::any testAny = std::string("hello");

    std::cout << std::any_cast<std::string>( testAny ) << "\n";

    std::stringstream testStream("hello my name is tim");

    std::getline( testStream, testString );

    std::cout << testString << '\n';

    std::getline( testStream, testString );

    printf("%s\n", testString.c_str() );



    printf("%s\n", x["hi"]->c_str());

    serv.serverListen("27015");
}