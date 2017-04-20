###############################################################################
# Makefile for pa2
# 
# Jeff A
# cs670
# pa2
###############################################################################

CC=g++-6
OPT=-O3

DBG=DEBUG
HC=HILL_CLIMBING
SA=SIMULATED_ANNEALING

all: main.cc board.o
	$(CC) $(OPT) -D$(HC) -D$(SA) -o main main.cc board.o

dbg: main.cc board.o
	$(CC) $(OPT) -D$(HC) -D$(SA) -D$(DBG) -o main main.cc board.o

hc: main.cc board.o
	$(CC) $(OPT) -D$(HC) -o main main.cc board.o

hcdbg: main.cc board.o
	$(CC) $(OPT) -D$(DBG) -D$(HC) -o main main.cc board.o

sa: main.cc board.o
	$(CC) $(OPT) -D$(SA) -o main main.cc board.o

sadbg: main.cc board.o
	$(CC) $(OPT) -D$(DBG) -D$(SA) -o main main.cc board.o

board.o: board.h board.cc
	$(CC) $(OPT) -c -o board.o board.cc

clean:
	rm -f *.o main core
