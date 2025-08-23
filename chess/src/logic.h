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



bool is_valid_move( s8 x0,s8 y0, s8 x1,s8 y1);
bool is_king_in_check( PLAYER player );
bool check_win( );


#endif

