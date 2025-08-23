#include "logic.h"

CHESS_PIECE board[8][8]; // X, Y

bool try_pawn_move( u8 x0,u8 y0, u8 x1,u8 y1, CHESS_PIECE src, CHESS_PIECE dst ) {
    if( src.player == PLAYER_ONE ) {
        // white pawn can move up 
        if( x0 == x1 ) {
            // can only move up
            if ( y0 - y1  == 1  && dst.player == NO_PLAYER ) {
                return true;
            }
            if( y0 == 6 ) { // starting white row, can move one or two
                if ( y0 - y1  == 2  && dst.player == NO_PLAYER ) {
                    // also check 
                    if( board[ x0][ y0 -1].player == NO_PLAYER) {
                        return true;
                    }
                }
            }
        } else {
            // captures are diagonal.

        }

    } else {
        // black pawn can move down.
    }
    return false;
}

bool try_knight_move( u8 x0,u8 y0, u8 x1,u8 y1, CHESS_PIECE src, CHESS_PIECE dst ) {
    // valid knight moves work in all dirs for both players
    if( abs(x0 - x1) == 1 && abs( y0 - y1) == 2 || abs(x0 - x1) == 2 && abs( y0 - y1) == 1 ) {
        if ( dst.player != src.player || dst.player == NO_PLAYER) {
            return true;
        }
    }
    return false;
}


bool is_valid_move( u8 x0,u8 y0, u8 x1,u8 y1)
{
    // if occupied by my own piece return false
    CHESS_PIECE src = board[x0][y0];
    CHESS_PIECE dst = board[x1][y1];
    if( dst.player == src.player ) {
        return false;
    }
    // what piece am I moving
    bool valid = false;
    switch( src.type ) {
        case PAWN: 
            valid = try_pawn_move(x0, y0, x1, y1, src, dst );
            break;
        case KNIGHT: 
            valid = try_knight_move(x0, y0, x1, y1, src, dst );
            break;
        case BISHOP: return true;
        case ROOK: return true;
        case QUEEN: return true;
        case KING: return true;
        default: return false;
    }

    if( valid == true ) {
        // check if King is exposed.
    } 
     
    // if we're here, nothing worked
    return valid;
}

bool check_win( ) 
{
    return false;
}
