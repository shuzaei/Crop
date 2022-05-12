crvm: main.o exec.o grid.o
	g++ -std=c++17 -o crvm main.o exec.o grid.o

grid.o: grid.cc
	g++ -std=c++17 -c -o grid.o grid.cc
exec.o: exec.cc grid.hh
	g++ -std=c++17 -c -o exec.o exec.cc 
main.o: main.cc exec.hh
	g++ -std=c++17 -c -o main.o main.cc