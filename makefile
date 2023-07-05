main.exe:	httpRequest.o	httpResponse.o	server.o	test.cpp	PathTree.o
	g++	*.o	test.cpp	-o main.exe	-lws2_32
server.o:	server.cpp	server.h hash.h
	g++	server.cpp	-c
httpResponse.o:	httpResponse.cpp httpResponse.h hash.h
	g++ httpResponse.cpp -c
httpRequest.o:	httpRequest.cpp	httpRequest.h	hash.h
	g++ httpRequest.cpp -c
PathTree.o:	PathTree.cpp	PathTree.h	hash.h
	g++ PathTree.cpp hash.h -c
JSON.o:	JSON.cpp	JSON.h	hash.h
	g++ JSON.cpp
clean:
	rm *.o