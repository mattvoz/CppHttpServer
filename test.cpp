#include "server.h"
#include <iostream>
#include <sstream>

int main(int argc, char ** argv) {
    httpServer serv = httpServer();

    serv.serverListen("27015");
}