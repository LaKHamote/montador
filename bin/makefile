cpp_source = ../main.cpp

all:	main.exe
	./main.exe

test:	runtest.exe
	./runtest.exe

runtest.exe:	functions.o ../runtest.cpp
	g++ -std=c++17 -Wall ./*.o ../runtest.cpp -o runtest.exe

main.exe:	functions.o $(cpp_source)
	g++ -std=c++17 -Wall ./*.o $(cpp_source) -o main.exe

functions.o:	../functions.cpp
	g++ -std=c++17 -Wall -c ../functions.cpp