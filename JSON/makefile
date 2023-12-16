lib:	JSON.o
	ar rs	JSON.a	JSON.o	JSON.h
test.exe:	JSON.o	test.o
	g++ JSON.o test.o -o test.exe
test.o:	test.cpp	JSON.o
	g++ test.cpp	JSON.o -c
JSON.o:	JSON.cpp	JSON.h
	g++ JSON.cpp -c