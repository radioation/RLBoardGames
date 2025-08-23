#include "logic.h"

CHESS_PIECE board[8][8]; // X, Y

bool try_pawn_move( s8 x0,s8 y0, s8 x1,s8 y1, CHESS_PIECE src, CHESS_PIECE dst ) {
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
                    if( board[ (u8)x0][ (u8)y0 -1].player == NO_PLAYER) {
                        return true;
                    }
                }
            }
        } else if ( abs( x0 - x1 ) == 1 && (y0-y1) == 1 && dst.player == PLAYER_TWO ) {
            // captures are diagonal.
            return true; 
        }

    } else {
        // black pawn can move down.
        if( x0 == x1 ) {
            // can only move down
            if ( y1 - y0  == 1  && dst.player == NO_PLAYER ) {
                return true;
            }
            if( y0 == 1 ) { // starting white row, can move one or two
                if ( y1 - y0  == 2  && dst.player == NO_PLAYER ) {
                    // also check 
                    if( board[ (u8)x0][ (u8)y0 +1].player == NO_PLAYER) {
                        return true;
                    }
                }
            }
        } else if ( abs( x0 - x1 ) == 1 && (y0-y1) == 1 && dst.player == PLAYER_ONE ) {
            // captures are diagonal.
            return true; 
        }
    }
    return false;
}



bool try_knight_move( s8 x0,s8 y0, s8 x1,s8 y1, CHESS_PIECE src, CHESS_PIECE dst ) {
    // valid knight moves work in all dirs for both players
    if( abs(x0 - x1) == 1 && abs( y0 - y1) == 2 || abs(x0 - x1) == 2 && abs( y0 - y1) == 1 ) {
        if ( dst.player != src.player || dst.player == NO_PLAYER) {
            return true;
        }
    }
    return false;
}


bool check_diagonal( s8 x0,s8 y0, s8 x1,s8 y1 ) {
    s8 dx = x1 - x0;
    s8 dy = y1 - y0;
    if( abs(dx) == 1 ) {
        return true; // fine.
    }
    dx = dx < 0 ? -1 : 1 ;
    dy = dy < 0 ? -1 : 1 ;
    s8 cx = x0 + dx;
    s8 cy = y0 + dy;
    while( cx  != x1 ) {
        if( board[(u8)cx][(u8)cy].player != NO_PLAYER ) {
            KLog("DIAG BAD");
            return false;
        }
        cx += dx;
        cy += dy;
    }
    return true;
}


bool try_bishop_move( s8 x0,s8 y0, s8 x1,s8 y1, CHESS_PIECE src, CHESS_PIECE dst ) {
    // diagnoal means move will always besame delta in x and y
    if( abs(x0-x1) != abs(y0-y1) ) {
        return false;
    }
    // and nothing must be in the way

    return check_diagonal( x0, y0, x1, y1 );
}

bool check_horizontal( s8 x0,s8 y0, s8 x1,s8 y1 ) {
    s8 dx = x1 - x0;
    if( abs(dx) == 1 ) {
        KLog("HORIZ fine" );
        return true; // fine.
    }
    dx = dx < 0 ? -1 : 1 ;
    s8 cx = x0 + dx;
    while( cx != x1 ) {
        KLog_S2(" cx ", cx, " y0 ", y0 );
        if( board[(u8)cx][(u8)y0].player != NO_PLAYER ) {
        KLog("HORIZ BAD" );
            return false;
        }
        cx += dx;
    }
        KLog("HORIZ OK" );
    return true;
}

bool check_vertical( s8 x0,s8 y0, s8 x1,s8 y1 ) {
    s8 dy = y1 - y0;
    if( abs(dy) == 1 ) {
        return true; // fine.
    }
    dy = dy < 0 ? -1 : 1 ;
    s8 cy = y0 + dy;
    while( cy != y1 ) {
        if( board[(u8)x0][(u8)cy].player != NO_PLAYER ) {
            return false;
        }
        cy += dy;
    }
    return true;
}




bool try_rook_move( s8 x0,s8 y0, s8 x1,s8 y1, CHESS_PIECE src, CHESS_PIECE dst ) {
    // must be horizontal or vertical (so diff-x or diff-y MUST be 0 )
    if( abs( x0 - x1 ) > 0 && abs( y0 - y1 ) > 0 ){
        // non-zero in both dirs, can't possibly be avlid
        return false;
    }
    // and nothing must be in the way
    if( x0 != x1 && !check_horizontal( x0, y0, x1, y1 ) ) {
        return false;
    }
    if( y0 != y1 && !check_vertical( x0, y0, x1, y1 ) ) {
        return false;
    }
    return true;
}

bool try_queen_move( s8 x0,s8 y0, s8 x1,s8 y1, CHESS_PIECE src, CHESS_PIECE dst ) {
    if( abs(x0-x1) == abs(y0-y1)  ||  // diag
            abs( x0 - x1 ) > 0 && abs( y0 - y1 ) == 0  || // horizontal
            abs( x0 - x1 ) == 0 && abs( y0 - y1 ) > 0   // vertical
      ){
        // and nothing must be in the way
        // and nothing must be in the way
        if( x0 != x1 && !check_horizontal( x0, y0, x1, y1 ) ) {
            return false;
        }
        if( y0 != y1 && !check_vertical( x0, y0, x1, y1 ) ) {
            return false;
        }
        if( !check_diagonal( x0, y0, x1, y1 ) ) {
            return false;
        }
        return true; 
    }
    return false;
}

bool try_king_move( s8 x0,s8 y0, s8 x1,s8 y1, CHESS_PIECE src, CHESS_PIECE dst ) {
    return false;
}


bool is_valid_move( s8 x0,s8 y0, s8 x1,s8 y1)
{
    if( x0 < 0 || x1 < 0 || y0 < 0 || y1 < 0 ) {
        return false;
    }
    if( x0 > 7 || x1 > 7 || y0 > 7 || y1 > 7 ) {
        return false;
    }
    // if occupied by my own piece return false
    CHESS_PIECE src = board[(u8)x0][(u8)y0];
    CHESS_PIECE dst = board[(u8)x1][(u8)y1];
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
        case BISHOP: 
            valid = try_bishop_move(x0, y0, x1, y1, src, dst );
            break;
        case ROOK:
            valid = try_rook_move(x0, y0, x1, y1, src, dst );
            break;
        case QUEEN:
            valid = try_queen_move(x0, y0, x1, y1, src, dst );
            break;
        case KING: 
            valid = try_queen_move(x0, y0, x1, y1, src, dst );
            break;
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
