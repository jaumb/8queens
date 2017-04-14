###############################################################################
# Makefile for pa2
# 
# Jeff A
# cs670
# pa2
###############################################################################

CC=g++
OPT=-O3

all: main.cc board.o
	$(CC) $(OPT) -o main main.cc board.o

board.o: board.h board.cc
	$(CC) $(OPT) -c -o board.o board.cc

clean:
	rm -f *.o main core
