/******************************************************************************
 * main.cc
 *
 * Entry point for 8 queens problem.
 * This program generates 10,000 random starting positions and uses hill
 * climbing and simulated annealing to try to find solutions to the 8
 * queens problem.
 *
 * Jeff A
 * cs670
 * pa2
 *****************************************************************************/

#include <iostream>
#include "board.h"

using namespace std;

int main(int argc, char *argv[])
{
  Board b1;
  Board b2;
  Board b3;
  Board b4;
  Board b5;
  b1.print();
  b2.print();
  b3.print();
  b4.print();
  b5.print();
  return 0;
}
