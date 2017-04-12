/******************************************************************************
 * board.cc
 *
 * Board class methods' implementation.
 *
 * Jeff A
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
  _neighbors = NULLPTR;
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
  this->_neighbors = rhs._neighbors;
}

/**
 * Destructor
 */
Board::~Board()
{
  delete[] _board;
  if (_neighbors)
    delete _neighbors;
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
    if (this->_neighbors)
      delete this->_neighbors;
    this->_neighbors = rhs._neighbors;
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
 * Get a list of this state's neighbor states
 */
const std::list<Board>* Board::neighbors()
{
  if (_neighbors == NULLPTR)
  {
    _neighbors = new std::list<Board>();
    int i, j, p, prevp, r, c; // index i, index j, position p, row r, column c

    // generate neighbor for every legal move for every queen
    for (i = 0; i < N_CELLS; ++i)
    {
      if (_board[i])
      {
        // record row, column, and position
        r = row(i);
        c = col(i);
        p = i;

        // move up as far as possible
        while (r > 0 && _board[pos(r - 1, c)] != 1)
        {
          prevp = p;
          p = pos(--r, c);
          swap(_board[prevp], _board[p]);
          _neighbors->push_back(Board(_board));
        }
        swap(_board[p], _board[i]);
        r = row(i);
        c = col(i);
        p = i;

        // move down as far as possible
        while (r < N_ROWS - 1 && _board[pos(r + 1, c)] != 1)
        {
          prevp = p;
          p = pos(++r, c);
          swap(_board[prevp], _board[p]);
          _neighbors->push_back(Board(_board));
        }
        swap(_board[p], _board[i]);
        r = row(i);
        c = col(i);
        p = i;

        // move left as far as possible
        while (c > 0 && _board[pos(r, c - 1)] != 1)
        {
          prevp = p;
          p = pos(r, --c);
          swap(_board[prevp], _board[p]);
          _neighbors->push_back(Board(_board));
        }
        swap(_board[p], _board[i]);
        r = row(i);
        c = col(i);
        p = i;

        // move right as far as possible
        while (c < N_COLS - 1 && _board[pos(r, c + 1)] != 1)
        {
          prevp = p;
          p = pos(r, ++c);
          swap(_board[prevp], _board[p]);
          _neighbors->push_back(Board(_board));
        }
        swap(_board[p], _board[i]);
        r = row(i);
        c = col(i);
        p = i;

        // move up and left as far as possible
        while (r > 0 && c > 0 && _board[pos(r - 1, c - 1)] != 1)
        {
          prevp = p;
          p = pos(--r, --c);
          swap(_board[prevp], _board[p]);
          _neighbors->push_back(Board(_board));
        }
        swap(_board[p], _board[i]);
        r = row(i);
        c = col(i);
        p = i;

        // move up and right as far as possible
        while (r > 0 && c < N_COLS - 1 && _board[pos(r - 1, c + 1)] != 1)
        {
          prevp = p;
          p = pos(--r, ++c);
          swap(_board[prevp], _board[p]);
          _neighbors->push_back(Board(_board));
        }
        swap(_board[p], _board[i]);
        r = row(i);
        c = col(i);
        p = i;

        // move down and left as far as possible
        while (r < N_ROWS - 1 && c > 0 && _board[pos(r + 1, c - 1)] != 1)
        {
          prevp = p;
          p = pos(++r, --c);
          swap(_board[prevp], _board[p]);
          _neighbors->push_back(Board(_board));
        }
        swap(_board[p], _board[i]);
        r = row(i);
        c = col(i);
        p = i;

        // move down and right as far as possible
        while (r < N_ROWS-1 && c < N_COLS-1 && _board[pos(r + 1, c + 1)] != 1)
        {
          prevp = p;
          p = pos(++r, ++c);
          swap(_board[prevp], _board[p]);
          _neighbors->push_back(Board(_board));
        }
        swap(_board[p], _board[i]);
      }
    }
  }
  return _neighbors;
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
 * Create a new board with an array
 * Restricted to private use only
 */
Board::Board(const unsigned char* board)
{
  _board = new unsigned char[N_CELLS];
  for (int i = 0; i < N_CELLS; ++i)
    _board[i] = board[i];
  _cost = -1;
  _neighbors = NULLPTR;
}

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
 * This is the h function for calculating the h value of board state.
 * Calculate the cost of this board state by counting the number of pairs
 * of queens endangering each other, directly or indirectly
 */
int Board::evaluate(const unsigned char* b)
{
  int currCost = 0;
  int i, r, c; // index, row, and column
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

  // get total # of pairs for rows and cols
  for (i = 0; i < N_QUEENS; ++i)
  {
    currCost += (rowCounts[i] * (rowCounts[i] - 1)) / 2;
    currCost += (colCounts[i] * (colCounts[i] - 1)) / 2;
  }

  // get total # of pairs for both diagonals
  for (i = 0; i < 2*N_QUEENS - 1; ++i)
  {
    currCost += (diag1Counts[i] * (diag1Counts[i] - 1)) / 2;
    currCost += (diag2Counts[i] * (diag2Counts[i] - 1)) / 2;
  }

  return currCost;
}
