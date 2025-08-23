#include "logic.h"

CHESS_PIECE board[8][8]; // X, Y

static const s8 KNIGHT_MOVES[ 8 ][2] = {
    {+1, -2}, {+2, -1}, {+2, +1}, {+1, +2},
    {-1, +2}, {-2, +1}, {-2, -1}, {-1, -2}
};

bool in_bounds( s8 x, s8 y ) {
    if( x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE ) {
        return true;
    }
    return false;
}

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
    if( ( abs(x0 - x1) == 1 && abs( y0 - y1) == 2 ) || ( abs(x0 - x1) == 2 && abs( y0 - y1) == 1 ) ) {
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
    if( (abs(x0-x1) == abs(y0-y1) ) ||  // diag
            (abs( x0 - x1 ) > 0 && abs( y0 - y1 ) == 0 ) || // horizontal
            (abs( x0 - x1 ) == 0 && abs( y0 - y1 ) > 0 )  // vertical
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
    if( abs( x0 - x1 ) <= 2 && abs( y0 - y1 ) <=2 ){
        return true;
    }
    return false;
}


bool is_square_attacked( u8 x, u8 y, PLAYER player ) {
    if( player == NO_PLAYER ) {
        return false; // nothing to atck in this square.
    }
    // just need to check square that can possibly attack king
    PLAYER atkr = PLAYER_ONE == player ? PLAYER_TWO : PLAYER_ONE;

    // PAWN : immediate diagonal from opposite side for pawns )
    s8 atk_y = y - 1 ;
    if( player == PLAYER_TWO ) {
        // check if player two is attacking player one's square
        s8 atk_y = y + 1;         
    }
    if( in_bounds( (u8)(x-1), (u8)atk_y ) && board[ (u8)(x-1) ][(u8)atk_y].type == PAWN && board[(u8)(x-1)][(u8)atk_y].player == atkr ) {
        return true;
    }

    // Knight : 8 possible pieces
    for (u8 i=0;i<8;i++){
        s8 atk_x = x + KNIGHT_MOVES[i][0];
        s8 atk_y = y + KNIGHT_MOVES[i][1];
        if (in_bounds(atk_x,atk_y) && board[(u8)atk_x][(u8)atk_y].type == KNIGHT && board[(u8)atk_x][(u8)atk_y].player == atkr ) return true;
    }
    // unobstructed diagonal is attacker's Bishop or Queen

    // unobstructed horizontal is attacker's  Rook or Queen

    // unobstructed vertical is attacker's Rook or Queen
    return false;
}


bool is_king_in_check( PLAYER player ) {
    // find the king
    s8 x=-1;
    s8 y=-1;
    for( s8 i=0; i < BOARD_SIZE; i++ ) {
        for( s8 j=0; j < BOARD_SIZE; j++ ) {
            CHESS_PIECE piece = board[(u8)i][(u8)j];
            if( piece.type == KING && piece.player == player ) {
                x = i;
                y = j;
                break;
            }
        }
        if( x >= 0 ) {
            break;
        }
    }

    if( !in_bounds( x,y ) ) {
        // out of bounds? shouldn't ever happen but false.
        return false;
    }    
    // check if the player's king's square is getting attacked.
    return is_square_attacked( x, y, player );

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
            valid = try_king_move(x0, y0, x1, y1, src, dst );
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
    // checkmate if 
    // if king is in check 
    // and king has no valid moves

    return false;
}
