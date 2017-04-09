###############################################################################
# Makefile for pa2
# 
# Jeff Amari
# cs670
# pa2
###############################################################################

CC=g++

all: main.cc board.o
	$(CC) -o main main.cc board.o

board.o: board.h board.cc
	$(CC) -c -o board.o board.cc

clean:
	rm -f *.o main core
