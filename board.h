/******************************************************************************
 * board.h
 *
 * Implementation of the chess board class for the 8 queens problem.
 * The default constuctor generates a random layout of a board with 8
 * queens.
 * 
 * Jeff A
 * cs670
 * pa2
 *****************************************************************************/

#ifndef BOARD_H
#define BOARD_H

#include <list>

#define NULLPTR 0
#define N_ROWS 8
#define N_COLS 8
#define N_CELLS (N_ROWS*N_COLS)
#define N_QUEENS 8

class Board
{

  public:
    Board(); // default constructor
    Board(Board const&); // copy constuctor
    ~Board(); // destructor
    Board& operator=(Board const&); // assignment operator
    int cost(); // cost of this state
    const std::list<Board>* neighbors(); // get list of all neighbor states
    void print(); // print the board (for debugging)

  private:
    // methods
    Board(const unsigned char* board); // initialize with an array
    void initBoard();
    int row(const int&);
    int col(const int&);
    int pos(const int&, const int&);
    void swap(unsigned char&, unsigned char&);
    void shuffle(); // shuffle the board
    int evaluate(const unsigned char*); // get the cost of this board state

    // members
    unsigned char* _board;
    std::list<Board>* _neighbors;
    int _cost;

};

#endif
