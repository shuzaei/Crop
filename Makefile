all: bin/crasm bin/crg bin/crint bin/crpp bin/crvm

install: bin/crasm bin/crg bin/crint bin/crpp bin/crvm bin/crc
	sudo cp bin/* /usr/local/bin

bin/crasm: CRASM/src/main.cc
	g++ -std=c++2a -o bin/crasm CRASM/src/main.cc

bin/crg: CRG/src/main.cc
	g++ -std=c++2a -o bin/crg CRG/src/main.cc

bin/crint: CRINT/out/main.o CRVM/out/grid.o
	g++ -std=c++2a -o bin/crint CRINT/out/main.o CRVM/out/grid.o

CRINT/out/main.o: CRINT/src/main.cc CRVM/src/grid.hh
	g++ -std=c++2a -c -o CRINT/out/main.o CRINT/src/main.cc

bin/crpp: CRPP/src/main.cc
	g++ -std=c++2a -o bin/crpp CRPP/src/main.cc

bin/crvm: CRVM/out/main.o CRVM/out/exec.o CRVM/out/grid.o
	g++ -std=c++2a -o bin/crvm CRVM/out/main.o CRVM/out/exec.o CRVM/out/grid.o

CRVM/out/grid.o: CRVM/src/grid.cc CRVM/src/grid.hh
	g++ -std=c++2a -c -o CRVM/out/grid.o CRVM/src/grid.cc
CRVM/out/exec.o: CRVM/src/exec.cc CRVM/src/grid.hh CRVM/src/exec.hh
	g++ -std=c++2a -c -o CRVM/out/exec.o CRVM/src/exec.cc 
CRVM/out/main.o: CRVM/src/main.cc CRVM/src/exec.hh
	g++ -std=c++2a -c -o CRVM/out/main.o CRVM/src/main.cc

clean:
	rm -rf */out/*