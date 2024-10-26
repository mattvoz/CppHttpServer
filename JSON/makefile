lib:	JSON.o
	ar rs	JSON.a	JSON.o	JSON.h
test:	JSON.o	test.o
	g++ -g JSON.o test.o -o test.exe
test.o:	test.cpp	JSON.o
	g++ -g test.cpp -c
JSON.o:	JSON.cpp	JSON.h
	g++ -g JSON.cpp -c
clean:
	rm -f *.o