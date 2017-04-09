/******************************************************************************
 * board.h
 *
 * Implementation of the chess board class for the 8 queens problem.
 * The default constuctor generates a random layout of a board with 8
 * queens.
 * 
 * Jeff Amari
 * cs670
 * pa2
 *****************************************************************************/

#ifndef BOARD_H
#define BOARD_H

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
    void print(); // print the board (for debugging)

  private:
    // methods
    void initBoard();
    int row(int);
    int col(int);
    int pos(int, int);
    void swap(unsigned char&, unsigned char&);
    void shuffle(); // shuffle the board

    // members
    unsigned char* board;

};

#endif
