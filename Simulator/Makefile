simulator:simulator.o I_type.o J_type.o R_type.o Machinecycle.o Dump.o
	g++  simulator.o I_type.o J_type.o R_type.o Machinecycle.o Dump.o -o simulator
simulator.o:simulator.cpp Machinecycle.h
	g++ -c simulator.cpp -o simulator.o
Machinecycle.o: Machinecycle.cpp Machinecycle.h I_type.h J_type.h R_type.h Dump.h
	g++ -c Machinecycle.cpp -o Machinecycle.o
I_type.o:I_type.cpp I_type.h
	g++ -c I_type.cpp -o I_type.o
J_type.o:J_type.cpp J_type.h
	g++ -c J_type.cpp -o J_type.o
R_type.o:R_type.cpp R_type.h
	g++ -c R_type.cpp -o R_type.o
Dump.o: Dump.cpp Dump.h
	g++ -c Dump.cpp -o Dump.o

clean:
	rm *o simulator