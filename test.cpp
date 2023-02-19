#include "server.h"

int main(int argc, char ** argv) {
    httpServer serv = httpServer();

    hashTable<std::string> x = hashTable<std::string>( 50 );

    std::string testString = "ahhhhh\n";

    x.add("hi", testString);

    printf("%s\n", x["hi"]->c_str());

    serv.serverListen("27015");
}