cpp_source = ../main.cpp

all:	montador.exe	ligador.exe

ligador.exe:	functions.o ../ligador.cpp
	g++ -std=c++17 -Wall ./*.o ../ligador.cpp -o ligador

montador.exe:	functions.o $(cpp_source)
	g++ -std=c++17 -Wall ./*.o $(cpp_source) -o montador

functions.o:	../functions.cpp
	g++ -std=c++17 -Wall -c ../functions.cpp