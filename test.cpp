#include "server.h"

int main(int argc, char ** argv) {
    httpServer serv = httpServer();

    hashTable<std::string> x = hashTable<std::string>( 50 );

    std::string testString = "ahhhhh\n";

    x.add("hello", &testString);
    testString = "xd";

    x.add("hello", &testString);

    printf("%s", x["hello"]->c_str());

    serv.serverListen("27015");
}