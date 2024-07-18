cpp_source = ../main.cpp

all:	montador.exe
	./montador

test:	runtest.exe
	./runtest.exe

runtest.exe:	functions.o ../runtest.cpp
	g++ -std=c++17 -Wall ./*.o ../runtest.cpp -o runtest.exe

montador.exe:	functions.o $(cpp_source)
	g++ -std=c++17 -Wall ./*.o $(cpp_source) -o montador

functions.o:	../functions.cpp
	g++ -std=c++17 -Wall -c ../functions.cpp