all: exampleC exampleCPP

exampleCPP: exampleCPP.cpp
	g++ exampleCPP.cpp -pthread -O3 -std=c++2a -g -o exampleCPP

exampleC: exampleC.c
	gcc exampleC.c -pthread -O3 -g -o exampleC



.PHONY : clean
clean:
	rm exampleC exampleCPP
