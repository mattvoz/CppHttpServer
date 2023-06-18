#include "server.h"
#include <iostream>
#include <sstream>

void hiRoute( httpRequest * req, httpResponse * res) {
    res->textplain( "this is a test from the server :)" );
    res->setStatus(200);
}

int main(int argc, char ** argv) {
    httpServer serv = httpServer();

    serv.GET(std::string("/test"), hiRoute);

    serv.serverListen("27015");
}