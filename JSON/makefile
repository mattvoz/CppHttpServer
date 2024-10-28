lib:	JSON.o	JSONObject.o	JSON.h	
	ar rs	JSON.a	JSON.o	JSON.h

test:	JSON.o	JSONObject.o	JSONArray.o	test.o
	g++ -g JSON.o 	JSONObject.o	JSONArray.o	test.o -o test.exe

test.o:	test.cpp	JSON.h
	g++ -g test.cpp -c

JSON.o:	JSON.cpp	JSON.h
	g++ -g JSON.cpp -c

JSONArray.o:	JSONArray.cpp	JSON.h
	g++ -g JSONArray.cpp	-c

JSONObject.o: JSON.h JSONObject.cpp
	g++ -g JSONObject.cpp -c

clean:
	rm -f *.o