out/crvm: out/main.o out/exec.o out/grid.o
	g++ -std=c++17 -o out/crvm out/main.o out/exec.o out/grid.o

out/grid.o: src/grid.cc src/grid.hh
	g++ -std=c++17 -c -o out/grid.o src/grid.cc
out/exec.o: src/exec.cc src/grid.hh src/exec.hh
	g++ -std=c++17 -c -o out/exec.o src/exec.cc 
out/main.o: src/main.cc src/exec.hh
	g++ -std=c++17 -c -o out/main.o src/main.cc

clean:
	rm -rf out/*