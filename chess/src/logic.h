#ifndef _LOGIC_H_
#define _LOGIC_H_

#include "genesis.h"

#define BOARD_SIZE 8


// Enum to represent piece types
typedef enum {
    EMPTY = 0,
    KING = 3, 
    QUEEN = 6, 
    ROOK = 9, 
    BISHOP = 12, 
    KNIGHT = 15, 
    PAWN = 18 
} PIECE_TYPE;

typedef enum {
    NO_PLAYER = 0,
    PLAYER_ONE = 1,
    PLAYER_TWO = 2
} PLAYER;

// Structure to represent a chess piece
typedef struct {
    PIECE_TYPE type;   // Type of the piece
    PLAYER player;     // which player  
} CHESS_PIECE;

CHESS_PIECE board[8][8]; // X, Y


//bool move_piece( int x0,int y0, int x1,int y1);
bool check_win( );


#endif

