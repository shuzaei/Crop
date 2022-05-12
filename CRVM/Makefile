crvm: main.o exec.o grid.o
	g++ -std=c++17 -o out/crvm out/main.o out/exec.o out/grid.o

grid.o: grid.cc
	g++ -std=c++17 -c -o out/grid.o grid.cc
exec.o: exec.cc grid.hh
	g++ -std=c++17 -c -o out/exec.o exec.cc 
main.o: main.cc exec.hh
	g++ -std=c++17 -c -o out/main.o main.cc

clean:
	rm -rf out/*