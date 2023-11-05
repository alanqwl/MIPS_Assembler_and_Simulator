tester:tester.o labelTable.o phase1.o phase2.o
	g++	tester.o labelTable.o phase1.o phase2.o -o tester
tester.o:tester.cpp phase1.h phase2.h labelTable.h
	g++ -c tester.cpp -o tester.o
phase2.o:phase2.h phase2.cpp
	g++ -c phase2.cpp -o phase2.o
labelTable.o:labelTable.cpp labelTable.h phase1.h
	g++ -c labelTable.cpp -o labelTable.o
phase1.o:phase1.h phase1.cpp
	g++ -c phase1.cpp -o phase1.o

clean:
	rm *o tester