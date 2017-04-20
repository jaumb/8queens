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

#define N_STARTS 10000UL // 10,000 random starts for both algorithms

#define TEMP_CUTOFF 0.0 // temp precision for simulated annealing
#define INITIAL_TEMP 0.01 // initial temperature for simulated annealing
#define CONSTANT_K 0.11361 // constant used to calculate acceptance probability

using namespace std;

// forward declarations
Board leastSuccessor(Board&);
Board randSuccessor(Board&);
double estimateK(void);

int main(int argc, char *argv[])
{
  size_t i; // counter
  size_t n_steps; // step count for simulated annealing
  size_t n_success = 0; // count # of optimal states reached
  size_t n_stuck = 0; // count # of times we got stuck at suboptimal state
  size_t n_moves; // count # of moves for each random start state
  double success_moves_sum = 0; // sum # moves for each successful attempt
  double stuck_moves_sum = 0; // sum # moves for each stuck attempt
  double T; // temperature
  double deltaE; // difference between next state and current state
  double k = CONSTANT_K; // constant used to calculate acceptance probability
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
  printf("n_success=%lu, n_stuck=%lu, N_STARTS=%lu\n",n_success,n_stuck,N_STARTS);
#endif
  printf("%.2f%%,%.2f,%.2f\n",100*static_cast<double>(n_success) / N_STARTS,
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
      printf("Trial #%lu----------------------------------\n",i);
#endif
    while (T > TEMP_CUTOFF && currState.cost() > 0)
    {
      successor = randSuccessor(currState); // get a random successor state
      deltaE = successor.cost() - currState.cost();
#ifdef DEBUG
      if (deltaE > 0)
        printf("dE: %.2f, T: %.4f, e^(k*dE/T): %.4f\n", deltaE, T, \
                                                          exp(k*-deltaE/T));
#endif
      if (deltaE <= 0 || exp(k * -deltaE / T) >
                                  static_cast<double>(rand())/RAND_MAX)
      {
        currState = successor;
        ++n_moves;
      }
      T = INITIAL_TEMP * pow(0.95, ++n_steps); // calculate temperature
      //T -= 0.05; // calculate temperature
    }
#ifdef DEBUG
      printf("n_steps: %lu, T: %f", n_steps, T);
      printf("End Trial #%lu------------------------------\n\n",i);
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
  printf("n_success=%lu, n_stuck=%lu, N_STARTS=%lu\n",n_success,n_stuck,N_STARTS);
#endif
  printf("%.2f%%,%.2f,%.2f\n",100*static_cast<double>(n_success) / N_STARTS,
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
  size_t minVal = numeric_limits<size_t>::max();
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

/**
 * Used to estimate k for different initial temps.
 * k is a constant used in calculating acceptance probability.
 */
double estimateK()
{
  size_t Q = 1000;
  double errorSum = 0;
  double errorSum2 = 0;
  Board currState;
  Board successor;

  for (size_t i = 0; i < Q; ++i)
  {
    successor = randSuccessor(currState);
    errorSum += successor.cost();
    errorSum2 += pow(successor.cost(), 2);
  }
  double varE = 1.0/(Q-1) * errorSum - 1.0/(Q*(Q-1)) * errorSum2;
  printf("varE: %f\n", varE);
  return -(INITIAL_TEMP * log(.8) / varE);
}

