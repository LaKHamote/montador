cpp_source = ../functions.cpp

all:	main.exe
	./main.exe

main.exe:	main.o functions.o
	g++ -std=c++17 -Wall ./*.o -o main.exe

main.o:	../main.cpp
	g++ -std=c++17 -Wall -c ../main.cpp

functions.o:	../functions.cpp
	g++ -std=c++17 -Wall -c ../functions.cpp