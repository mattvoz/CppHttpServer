#include "server.h"
#include <iostream>
#include <sstream>
#include "PathTree.h"

void hiRoute( httpRequest * req, httpResponse * res) {
    res->textplain( "this is a test from the server :)" );
    res->setStatus(200);
}

void test( httpRequest * req, httpResponse * res) {
    res->textplain( "this is a test from the server :)" );
    res->headers.add("Content-Type", "test-type");
    res->setStatus(200);
}

void testMiddleWare(httpRequest * req, httpResponse * res){
    printf("THIS IS MIDDLEWARE ALERT MIDDLEWARE");
}

void JSONtest( httpRequest * req, httpResponse * res) {
    res->JSON("{'TEST': 1234}");
}

int main(int argc, char ** argv) {
    httpServer serv = httpServer();

    serv.GET("/", testMiddleWare, test, NULL);

    serv.GET("/hi", hiRoute, NULL);
    serv.GET("/test", test);
    serv.GET("/jsonTest", JSONtest);

    serv.serverListen("0.0.0.0", "27015");
}