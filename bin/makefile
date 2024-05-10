cpp_source = ../functions.cpp

all:	main.exe
	./main.exe

test:	runtest.exe
	./runtest.exe

runtest.exe:	runtest.o functions.o
	g++ -std=c++17 -Wall ./*.o -o runtest.exe

runtest.o:	../runtest.cpp
	g++ -std=c++17 -Wall -c ../runtest.cpp

main.exe:	main.o functions.o
	g++ -std=c++17 -Wall ./*.o -o main.exe

main.o:	../main.cpp
	g++ -std=c++17 -Wall -c ../main.cpp

functions.o:	../functions.cpp
	g++ -std=c++17 -Wall -c ../functions.cpp