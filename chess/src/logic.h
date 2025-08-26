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

// structure for loggin two checkers
typedef struct {
    int count;
    int x[2];
    int y[2];
    bool is_biroqu[2]; // is a bishop, rook, or queen
} CHECKERS;


void clear_board();
bool is_valid_move( s8 x0,s8 y0, s8 x1,s8 y1);
bool is_my_king_in_check( CHESS_PIECE b[BOARD_SIZE][BOARD_SIZE], PLAYER player );

void set_piece( s8 x, s8 y, PIECE_TYPE t, PLAYER p );

CHECKERS find_checkers( PLAYER palyer, s8 king_x, s8 kings_y ); // KINGS X!

bool is_checkmate( PLAYER player );
bool is_stalemate( PLAYER player );

bool is_block_square(s8 kings_x, s8 kings_y, s8 checker_x, s8 checker_y,  s8 blocker_x, s8 blocker_y);
bool has_any_valid_move( PLAYER player );
bool any_valid_king_move( PLAYER player);
bool find_king(  CHESS_PIECE b[BOARD_SIZE][BOARD_SIZE], PLAYER player, s8 * kings_x, s8  *kings_y );


bool is_rank_empty_between(  s8 y, s8 x0, s8 x1 );
bool is_square_attacked( CHESS_PIECE b[BOARD_SIZE][BOARD_SIZE], u8 x, u8 y, PLAYER player );
bool is_rank_attacked_between( CHESS_PIECE b[BOARD_SIZE][BOARD_SIZE], s8 y, s8 x0, s8 x1, PLAYER player );
bool can_castle_kingside( PLAYER player );
bool can_castle_queenside( PLAYER player );


#endif

