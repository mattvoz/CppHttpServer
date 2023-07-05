#include "server.h"
#include <iostream>
#include <sstream>
#include "PathTree.h"

void hiRoute( httpRequest * req, httpResponse * res) {
    res->textplain( "this is a test from the server :)" );
    res->setStatus(200);
}

int main(int argc, char ** argv) {
    httpServer serv = httpServer();

    serv.get("/", hiRoute);

    serv.get("/hi", hiRoute);

    serv.serverListen("27015");
}