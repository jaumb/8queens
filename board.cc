/******************************************************************************
 * board.cc
 *
 * Board class methods' implementation.
 *
 * Jeff Amari
 * cs670
 * pa2
 *****************************************************************************/

#include <cstdlib>
#include <cstdio>
#include "board.h"


//============================================================================
//                          Public Methods                                  //
//============================================================================

/**
 * Default constructor
 * Generates a random starting board with 8 queens
 */
Board::Board()
{
  _cost = -1;
  initBoard();
}

/**
 * Copy constructor
 */
Board::Board(Board const& rhs)
{
  _board = new unsigned char[N_CELLS];
  for (char i = 0; i < N_CELLS; ++i)
    _board[i] = rhs._board[i];
  this->_cost = rhs._cost;
}

/**
 * Destructor
 */
Board::~Board()
{
  delete[] _board;
}

/**
 * Assignment operator
 */
Board& Board::operator=(Board const& rhs)
{
  if (this != &rhs)
  {
    for (char i = 0; i < N_CELLS; ++i)
      _board[i] = rhs._board[i];
    this->_cost = rhs._cost;
  }
  return *this;
}

/**
 * Get the cost of this board state
 * Cost is evaluated by the number of pairs of queens endangering
 * each other, directly or indirectly
 */
int Board::cost()
{
  if (_cost == -1)
    _cost = evaluate(_board);
  return _cost;
}

/**
 * Get a list of this state's lowest cost neighbor states
 */
std::list<Board> Board::bestNeighbors()
{
  std::list<Board> list;
  // TODO
  return list;
}

/**
 * Print the board to stdout (used for debugging)
 */
void Board::print()
{
  printf("\nboard address: %p\n", _board);
  for (int i = 0; i < N_ROWS; ++i)
  {
    for (int j = 0; j < N_COLS; ++j)
    {
      printf("%-2d", _board[pos(i,j)]);
    }
    printf("\n");
  }
  printf("cost: %d\n\n", cost());
}



//============================================================================
//                          Private Methods                                 //
//============================================================================

/*
 * Initialize the board
 */
void Board::initBoard()
{
  int i;

  _board = new unsigned char[N_CELLS];
  for (i = 0; i < N_QUEENS; ++i)
    _board[i] = 1;

  for (; i < N_CELLS; ++i)
    _board[i] = 0;

  shuffle();
}

/*
 * Get the row on a 2D board given a position in a 1D array
 */
int Board::row(const int& pos)
{
  return pos / N_ROWS;
}

/*
 * Get the col on a 2D board given a position in a 1D array
 */
int Board::col(const int& pos)
{
  return pos % N_ROWS;
}

/*
 * Get the position in the 1D array given row and column in 2D board
 */
int Board::pos(const int& row, const int& col)
{
  return row * N_COLS + col;
}

/*
 * Swap element a with element b
 */
void Board::swap(unsigned char& a, unsigned char& b)
{
  unsigned char tmp = a;
  a = b;
  b = tmp;
}

/*
 * Shuffle the board (Fisher-Yates Shuffle algorithm)
 */
void Board::shuffle()
{
  srand(reinterpret_cast<size_t>(_board));
  for (int i = N_CELLS - 1; i > 0; --i)
    swap(_board[i], _board[rand() % (i + 1)]);
}

/*
 * Calculate the cost of this board state by counting the number of pairs
 * of queens endangering each other, directly or indirectly
 */
int Board::evaluate(const unsigned char* b)
{
  int currCost = 0;
  int i, r, c;
  unsigned char diag1Counts[2*N_QUEENS - 1];
  unsigned char diag2Counts[2*N_QUEENS - 1];
  unsigned char rowCounts[N_QUEENS];
  unsigned char colCounts[N_QUEENS];

  // zero out row and cols counts
  for (i = 0; i < N_QUEENS; ++i)
  {
    rowCounts[i] = 0;
    colCounts[i] = 0;
  }

  // zero out diagonal counts
  for (i = 0; i < 2*N_QUEENS - 1; ++i)
  {
    diag1Counts[i] = 0;
    diag2Counts[i] = 0;
  }

  // record counts along rows, cols, and both diagonals
  for (i = 0; i < N_CELLS; ++i)
  {
    if (b[i])
    {
      r = row(i);
      c = col(i);
      diag1Counts[r + c] += 1;
      diag2Counts[c - r + (N_QUEENS - 1)] += 1;
      rowCounts[r] += 1;
      colCounts[c] += 1;
    }
  }

  // get total for rows and cols
  for (i = 0; i < N_QUEENS; ++i)
  {
    currCost += (rowCounts[i] * (rowCounts[i] - 1)) / 2;
    currCost += (colCounts[i] * (colCounts[i] - 1)) / 2;
  }

  // get total for both diagonals
  for (i = 0; i < 2*N_QUEENS - 1; ++i)
  {
    currCost += (diag1Counts[i] * (diag1Counts[i] - 1)) / 2;
    currCost += (diag2Counts[i] * (diag2Counts[i] - 1)) / 2;
  }

  return currCost;
}
