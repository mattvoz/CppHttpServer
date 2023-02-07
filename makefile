main.exe:	server.o	test.cpp
	g++	server.o	test.cpp	-o main.exe	-lws2_32
server.o:	server.cpp	server.h
	g++	server.cpp	-c
clean:
	rm *.o