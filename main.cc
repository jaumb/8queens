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
#include <cmath>
#include "board.h"

#define N_STARTS 10000 // 10,000 random starts for both algorithms

#define TEMP_PRECISION 1.00 // temp precision for simulated annealing
#define INITIAL_TEMP 5.0 // initial temperature for simulated annealing

using namespace std;

// forward declarations
Board leastSuccessor(Board&);
Board randSuccessor(Board&);

int main(int argc, char *argv[])
{
  int i; // counter
  int n_steps; // step count for simulated annealing
  int n_success = 0; // count # of optimal states reached
  int n_stuck = 0; // count # of times we got stuck at suboptimal state
  int n_moves; // count # of moves for each random start state
  double success_moves_sum = 0; // sum # moves for each successful attempt
  double stuck_moves_sum = 0; // sum # moves for each stuck attempt
  double T; // temperature
  double deltaE; // difference between next state and current state
  Board successor; // successor state

  // seed the random number generator
  srand(time(NULL));

#ifdef HILL_CLIMBING
  // perform Hill Climbing search
  for (i = 0; i < N_STARTS; ++i) // for each random starting state
  {
    Board currState; // generate a random start state
    successor = leastSuccessor(currState);
    n_moves = 0; // reset # moves for each start

    // continue to move to successor states while the least cost
    // successor state is better than the current state
    while (successor.cost() < currState.cost())
    {
      currState = successor;
      successor = leastSuccessor(currState);
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
#ifdef DEBUG
  printf("n_success=%d, n_stuck=%d, N_STARTS=%d\n",n_success,n_stuck,N_STARTS);
#endif
  printf("%.2f%%, %.2f, %.2f\n",100*static_cast<double>(n_success) / N_STARTS,
                      (n_success == 0) ? 0.0 : success_moves_sum / n_success,
                      (n_stuck == 0) ? 0.0 : stuck_moves_sum / n_stuck);
#endif // end hill climbing

  // reset counters for Simulated Annealing
  n_success = 0;
  n_stuck = 0;
  success_moves_sum = 0;
  stuck_moves_sum = 0;

#ifdef SIMULATED_ANNEALING
  // perform Simulated Annealing search
  for (i = 0; i < N_STARTS; ++i) // for each random starting state
  {
    Board currState; // generate a random start state
    n_moves = 0;
    n_steps = 0;
    T = INITIAL_TEMP; // initial temperature

#ifdef DEBUG
      printf("Trial #%d----------------------------------\n",i);
#endif
    while (T > TEMP_PRECISION && currState.cost() > 0)
    {
      successor = randSuccessor(currState); // get a random successor state
      deltaE = currState.cost() - successor.cost();
#ifdef DEBUG
      if (deltaE < 0)
        printf("dE: %.2f, T: %.4f, e^(dE/T): %.4f\n", deltaE, T, \
                                                          exp(deltaE/T));
#endif
      if (deltaE > 0 || exp(deltaE / T) >
                                  static_cast<double>(rand())/RAND_MAX)
      {
        currState = successor;
        ++n_moves;
      }
      T = INITIAL_TEMP * pow(0.995, ++n_steps); // calculate temperature
    }
#ifdef DEBUG
      printf("n_steps: %d, T: %f", n_steps, T);
      printf("End Trial #%d------------------------------\n\n",i);
#endif

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
  // print Simulated Annealing results
#ifdef DEBUG
  printf("n_success=%d, n_stuck=%d, N_STARTS=%d\n",n_success,n_stuck,N_STARTS);
#endif
  printf("%.2f%%, %.2f, %.2f\n",100*static_cast<double>(n_success) / N_STARTS,
                      (n_success == 0) ? 0.0 : success_moves_sum / n_success,
                      (n_stuck == 0) ? 0.0 : stuck_moves_sum / n_stuck);
#endif // end simulated annealing

  return 0;
}


/**
 * Helper function to find a least successor of the current board state.
 */
Board leastSuccessor(Board& currState)
{
  const vector<Board>* vec = currState.successors();
  int minVal = numeric_limits<int>::max();
  vector<Board> minSuccessors;

  // find successors with lowest value
  for (vector<Board>::const_iterator it = vec->begin(); it != vec->end(); ++it)
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
  return minSuccessors.at(rand() % minSuccessors.size());
}

/**
 * Helper function to select a random successor of the current board state.
 */
Board randSuccessor(Board& currState)
{
  const vector<Board>* vec = currState.successors();
  return vec->at(rand() % vec->size());
}
