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
  initBoard();
}

/**
 * Copy constructor
 */
Board::Board(Board const& rhs)
{
  board = new unsigned char[N_CELLS];
  for (char i = 0; i < N_CELLS; ++i)
    board[i] = rhs.board[i];
}

/**
 * Destructor
 */
Board::~Board()
{
  delete[] board;
}

/**
 * Assignment operator
 */
Board& Board::operator=(Board const& rhs)
{
  if (this != &rhs)
  {
    for (char i = 0; i < N_CELLS; ++i)
      board[i] = rhs.board[i];
  }
  return *this;
}

/**
 * Print the board to stdout (used for debugging)
 */
void Board::print()
{
  printf("\nboard address: %p\n", board);
  for (int i = 0; i < N_ROWS; ++i)
  {
    for (int j = 0; j < N_COLS; ++j)
    {
      printf("%-3d", board[pos(i,j)]);
    }
    printf("\n");
  }
  printf("\n");
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

  board = new unsigned char[N_CELLS];
  for (i = 0; i < N_QUEENS; ++i)
    board[i] = 1;

  for (; i < N_CELLS; ++i)
    board[i] = 0;

  shuffle();
}

/*
 * Get the row on a 2D board given a position in a 1D array
 */
int Board::row(int pos)
{
  return pos / N_ROWS;
}

/*
 * Get the col on a 2D board given a position in a 1D array
 */
int Board::col(int pos)
{
  return pos % N_ROWS;
}

/*
 * Get the position in the 1D array given row and column in 2D board
 */
int Board::pos(int row, int col)
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
  srand(reinterpret_cast<size_t>(board));
  for (int i = N_CELLS - 1; i > 0; --i)
    swap(board[i], board[rand() % (i + 1)]);
}
