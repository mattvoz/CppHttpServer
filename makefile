main.exe:	httpRequest.o	httpResponse.o	server.o	test.cpp
	g++	*.o	test.cpp	-o main.exe	-lws2_32
server.o:	server.cpp	server.h hash.h
	g++	server.cpp	-c
httpResponse.o:	httpResponse.cpp httpResponse.h hash.h
	g++ httpResponse.cpp -c
httpRequest.o:	httpRequest.cpp	httpRequest.h	hash.h
	g++ httpRequest.cpp -c
JSON.o:	JSON.c	JSON.h	hash.h
	g++ JSON.c
clean:
	rm *.o