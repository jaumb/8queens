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
#include <iterator>
#include <limits>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <vector>
#include "board.h"

#define N_STARTS 10000 // 10,000 random starts

using namespace std;

Board leastSuccessor(Board&);

int main(int argc, char *argv[])
{
  int n_success = 0; // count # of optimal states reached
  int n_stuck = 0; // count # of times we got stuck at suboptimal state
  int n_moves; // count # of moves for each random start state
  double success_moves_sum = 0; // sum # moves for each successful attempt
  double stuck_moves_sum = 0; // sum # moves for each stuck attempt

  // perform Hill Climbing search
  for (int i = 0; i < N_STARTS; ++i) // for each random starting state
  {
    Board currState; // generate a random start state
    Board leastSuccessorState = leastSuccessor(currState);
    n_moves = 0; // reset # moves for each start

    // continue to move to successor states while the least cost
    // successor state is better than the current state
    while (leastSuccessorState.cost() < currState.cost())
    {
      currState = leastSuccessorState;
      leastSuccessorState = leastSuccessor(currState);
      ++n_moves;
    }

    // increment success counter if current state is a goal state
    // otherwise increment stuck counter
    if (currState.cost() == 0)
    {
      ++n_success;
      success_moves_sum += n_moves;
    }
    else
    { 
      ++n_stuck;
      stuck_moves_sum += n_moves;
    }
  }
  // print Hill Climbing results
  printf("n_success=%d, n_stuck=%d, N_STARTS=%d\n",n_success,n_stuck,N_STARTS);
  printf("%.2f%%, %.2f, %.2f\n",100 * static_cast<double>(n_success) / N_STARTS,
                      (n_success == 0) ? 0.0 : success_moves_sum / n_success,
                      (n_stuck == 0) ? 0.0 : stuck_moves_sum / n_stuck);

  // reset counters for Simulated Annealing
  n_success = 0;
  n_stuck = 0;
  success_moves_sum = 0;
  stuck_moves_sum = 0;

  // perform Simulated Annealing search
  //for (int i = 0; i < N_STARTS; ++i) // for each random starting state
  //{
  //  Board currState; // generate a random start state
  //  Board leastSuccessorState = leastSuccessor(currState);
  //  n_moves = 0;
  //}
  //// print Simulated Annealing results
  //printf("%.2f%%, %.2f, %.2f\n",100*static_cast<double>(n_success) / N_STARTS,
  //                    (n_success == 0) ? 0.0 : success_moves_sum / n_success,
  //                    (n_stuck == 0) ? 0.0 : stuck_moves_sum / n_stuck);

  return 0;
}


/**
 * Helper function to find a least successor of the current board state.
 */
Board leastSuccessor(Board& currState)
{
  const list<Board>* l = currState.successors();
  int minVal = numeric_limits<int>::max();
  vector<Board> minSuccessors;

  // find successors with lowest value
  for (list<Board>::const_iterator it = l->begin(); it != l->end(); ++it)
  {
    if ((*it).cost() < minVal)
    {
      minVal = (*it).cost();
      minSuccessors.clear();
      minSuccessors.push_back(*it);
    }
    else if ((*it).cost() == minVal)
    {
      minSuccessors.push_back(*it);
    }
  }
  // return a random least successor state
  srand(time(NULL));
  return minSuccessors[rand() % minSuccessors.size()];
}
