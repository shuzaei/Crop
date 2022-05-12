out/crvm: out/main.o out/exec.o out/grid.o
	g++ -std=c++17 -o out/crvm out/main.o out/exec.o out/grid.o

out/grid.o: grid.cc grid.hh
	g++ -std=c++17 -c -o out/grid.o grid.cc
out/exec.o: exec.cc grid.hh exec.hh
	g++ -std=c++17 -c -o out/exec.o exec.cc 
out/main.o: main.cc exec.hh
	g++ -std=c++17 -c -o out/main.o main.cc

clean:
	rm -rf out/*