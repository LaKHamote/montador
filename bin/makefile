cpp_source = ../main.cpp

all:	compilador.exe
	./compilador.exe

test:	runtest.exe
	./runtest.exe

runtest.exe:	structure.o ../runtest.cpp
	g++ -std=c++17 -Wall ./*.o ../runtest.cpp -o runtest.exe

main.exe:	structure.o $(cpp_source)
	g++ -std=c++17 -Wall ./*.o $(cpp_source) -o compilador.exe

utils.o:	../utils.cpp
	g++ -std=c++17 -Wall -c ../utils.cpp

structure.o:	../structure.cpp
	g++ -std=c++17 -Wall -c ../structure.cpp

pre_proc.o:	../pre_proc.cpp
	g++ -std=c++17 -Wall -c ../pre_proc.cpp

assembler.o:	../assembler.cpp
	g++ -std=c++17 -Wall -c ../assembler.cpp

ligador.o:	../ligador.cpp
	g++ -std=c++17 -Wall -c ../ligador.cpp