#include "logic.h"

CHESS_PIECE board[BOARD_SIZE][BOARD_SIZE]; // X, Y

static const s8 KNIGHT_MOVES[8][2] = { {1, -2}, {2, -1}, {2, 1}, {1, 2}, {-1, 2}, {-2, 1}, {-2, -1}, {-1, -2} };

static const s8 DIAGONAL_MOVES[4][2] = { { -1, -1 }, { 1, -1 }, { 1, 1 }, { -1, 1 } };

void clear_board() {
    for( u8 x=0; x < BOARD_SIZE; x++ ) {
        for( u8 y=0; y < BOARD_SIZE; y++ ){
            board[x][y].type = EMPTY;
            board[x][y].player = NO_PLAYER;
        }
    }
}



bool in_bounds( s8 x, s8 y ) {
    if( x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE ) {
        return true;
    }
    return false;
}

bool try_pawn_move( s8 x0,s8 y0, s8 x1,s8 y1, CHESS_PIECE src, CHESS_PIECE dst ) {
    if( src.player == PLAYER_ONE ) {
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
        } else if ( abs( x0 - x1 ) == 1 && (y1-y0) == 1 && dst.player == PLAYER_ONE ) {
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
    if( abs(dx) != abs(dy) || abs(dx) == 0 || abs(dy) == 0 ) {
        return false; 
    }
    dx = dx < 0 ? -1 : 1 ;
    dy = dy < 0 ? -1 : 1 ;
    s8 cx = x0 + dx;
    s8 cy = y0 + dy;
    while( cx  != x1 ) {
        if( board[(u8)cx][(u8)cy].player != NO_PLAYER ) {
            //KLog("DIAG BAD");
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
        //KLog("HORIZ fine" );
        return true; // fine.
    }
    dx = dx < 0 ? -1 : 1 ;
    s8 cx = x0 + dx;
    while( cx != x1 ) {
        //KLog_S2(" cx ", cx, " y0 ", y0 );
        if( board[(u8)cx][(u8)y0].player != NO_PLAYER ) {
            //KLog("HORIZ BAD" );
            return false;
        }
        cx += dx;
    }
    //KLog("HORIZ OK" );
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
        if( x0 != x1 && y0 == y1 ) {
            return  check_horizontal( x0, y0, x1, y1 );
        } else if( y0 != y1 &&  x0 == x1 ) {
            return check_vertical( x0, y0, x1, y1 );
        } 
        // diagonal is all that's left
        return check_diagonal( x0, y0, x1, y1 );
    }
    return false;
}

bool try_king_move( s8 x0,s8 y0, s8 x1,s8 y1, CHESS_PIECE src, CHESS_PIECE dst ) {
    if( abs( x0 - x1 ) <= 1 && abs( y0 - y1 ) <=1 ){
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
        atk_y = y + 1;         
    }
    if( in_bounds( (u8)(x-1), (u8)atk_y ) 
        && board[ (u8)(x-1) ][(u8)atk_y].type == PAWN 
        && board[(u8)(x-1)][(u8)atk_y].player == atkr ) {
        return true;
    }
    if( in_bounds( (u8)(x+1), (u8)atk_y ) 
        && board[ (u8)(x+1) ][(u8)atk_y].type == PAWN 
        && board[(u8)(x+1)][(u8)atk_y].player == atkr ) {
        return true;
    }

    // Knight : 8 possible pieces
    for (u8 i=0;i<8;i++){
        s8 atk_x = x + KNIGHT_MOVES[i][0];
        s8 atk_y = y + KNIGHT_MOVES[i][1];
        if (in_bounds(atk_x,atk_y) && board[(u8)atk_x][(u8)atk_y].type == KNIGHT && board[(u8)atk_x][(u8)atk_y].player == atkr ) {
            return true;
         }     
    }


    // unobstructed diagonal is attacker's Bishop or Queen
    for( u8 i=0; i < 4; i++ ) {
        s8 dx = DIAGONAL_MOVES[i][0];
        s8 dy = DIAGONAL_MOVES[i][1];
        s8 atk_x = x + dx;
        s8 atk_y = y + dy;
        while( in_bounds( atk_x, atk_y ) ) {
            CHESS_PIECE piece = board[(u8)atk_x][(u8)atk_y];
            if( piece.type != EMPTY && piece.player == player ) {
                break;
            }
            if( ( piece.type == BISHOP || piece.type == QUEEN ) && piece.player == atkr) 
            {
                return true;
            } 
            atk_x = atk_x + dx;
            atk_y = atk_y + dy;
        }

    }
    // unobstructed horizontal is attacker's  Rook or Queen
    for( s8 dx = -1; dx < 2; dx+=2 ) {
        s8 atk_x = x + dx;
        while( in_bounds( atk_x, y ) ) {
            CHESS_PIECE piece = board[(u8)atk_x][(u8)y];
            if( piece.type != EMPTY && piece.player == player ) {
                break;
            }
            if( ( piece.type == ROOK || piece.type == QUEEN ) && piece.player == atkr) 
            {
                return true;
            } 
            atk_x = atk_x + dx;
        }
    }


    // unobstructed vertical is attacker's Rook or Queen
    for( s8 dy = -1; dy < 2; dy+=2 ) {
        s8 atk_y = y + dy;
        while( in_bounds( x, atk_y ) ) {
            CHESS_PIECE piece = board[(u8)x][(u8)atk_y];
            if( piece.type != EMPTY && piece.player == player ) {
                break;
            }
            if( ( piece.type == ROOK || piece.type == QUEEN ) && piece.player == atkr) 
            {
                return true;
            } 
            atk_y = atk_y + dy;
        }
    }

    return false;
}

bool find_king( CHESS_PIECE b[BOARD_SIZE][BOARD_SIZE], PLAYER player, s8 *x, s8 *y ) {
    // find the king
    *x = -1;
    *y = -1;
    for( s8 i=0; i < BOARD_SIZE; i++ ) {
        for( s8 j=0; j < BOARD_SIZE; j++ ) {
            CHESS_PIECE piece = b[(u8)i][(u8)j];
            if( piece.type == KING && piece.player == player ) {
                *x = i;
                *y = j;
                break;
            }
        }
        if( *x >= 0 ) {
            return true;
        }
    }
    return false; 
}


bool is_my_king_in_check( CHESS_PIECE b[BOARD_SIZE][BOARD_SIZE], PLAYER player ) {
    s8 x=-1;
    s8 y=-1;
    if( !find_king( b, player, &x, &y ) ) {
        return false; // not found, cant' be in check.
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

   // if( valid == true ) {
   //     // check if King is exposed.
   //     valid = is_my_king_in_check( src.player );
   // } 

    // if we're here, nothing worked
    return valid;
}

void set_piece( s8 x, s8 y, PIECE_TYPE t, PLAYER p ) 
{
    board[x][y].type = t;
    board[x][y].player = p;
}

CHECKERS find_checkers( PLAYER player, s8 x, s8 y )
{
    CHECKERS check_out = {0}; 

    PLAYER atkr = ( player == PLAYER_ONE ) ? PLAYER_TWO : PLAYER_ONE;

    // PAWN : immediate diagonal from opposite side for pawns )
    s8 atk_y = y - 1 ;
    if( player == PLAYER_TWO ) {
        // check if player two is attacking player one's square
        atk_y = y + 1;         
    }
    if( in_bounds( (u8)(x-1), (u8)atk_y ) 
        && board[ (u8)(x-1) ][(u8)atk_y].type == PAWN 
        && board[(u8)(x-1)][(u8)atk_y].player == atkr ) {
        check_out.x[check_out.count] = x-1;
        check_out.y[check_out.count] = atk_y;
        check_out.is_biroqu[check_out.count] = false;
        check_out.count++;
        if( check_out.count >=2 ) return check_out;
    
    }
    if( in_bounds( (u8)(x+1), (u8)atk_y ) 
        && board[ (u8)(x+1) ][(u8)atk_y].type == PAWN 
        && board[(u8)(x+1)][(u8)atk_y].player == atkr ) {
        check_out.x[check_out.count] = x+1;
        check_out.y[check_out.count] = atk_y;
        check_out.is_biroqu[check_out.count] = false;
        check_out.count++;
        if( check_out.count >=2 ) return check_out;
    }

    // Knight : 8 possible pieces
    for (u8 i=0;i<8;i++){
        s8 atk_x = x + KNIGHT_MOVES[i][0];
        s8 atk_y = y + KNIGHT_MOVES[i][1];
        if (in_bounds(atk_x,atk_y) && board[(u8)atk_x][(u8)atk_y].type == KNIGHT && board[(u8)atk_x][(u8)atk_y].player == atkr ) {
            check_out.x[check_out.count] = atk_x;
            check_out.y[check_out.count] = atk_y;
            check_out.is_biroqu[check_out.count] = false;
            check_out.count++;
            if( check_out.count >=2 ) return check_out;
        }     
    }
    // unobstructed diagonal is attacker's Bishop or Queen
    for( u8 i=0; i < 4; i++ ) {
        s8 dx = DIAGONAL_MOVES[i][0];
        s8 dy = DIAGONAL_MOVES[i][1];
        s8 atk_x = x + dx;
        s8 atk_y = y + dy;
        while( in_bounds( atk_x, atk_y ) ) {
            CHESS_PIECE piece = board[(u8)atk_x][(u8)atk_y];
            if( piece.type != EMPTY && piece.player == player ) {
                break;
            }
            if( ( piece.type == BISHOP || piece.type == QUEEN ) && piece.player == atkr) 
            {
                check_out.x[check_out.count] = atk_x;
                check_out.y[check_out.count] = atk_y;
                check_out.is_biroqu[check_out.count] = true;
                check_out.count++;
                if( check_out.count >=2 ) return check_out;
            } 
            atk_x = atk_x + dx;
            atk_y = atk_y + dy;
        }

    }
    // unobstructed horizontal is attacker's  Rook or Queen
    for( s8 dx = -1; dx < 2; dx+=2 ) {
        s8 atk_x = x + dx;
        while( in_bounds( atk_x, y ) ) {
            CHESS_PIECE piece = board[(u8)atk_x][(u8)y];
            if( piece.type != EMPTY && piece.player == player ) {
                break;
            }
            if( ( piece.type == ROOK || piece.type == QUEEN ) && piece.player == atkr) 
            {
                check_out.x[check_out.count] = atk_x;
                check_out.y[check_out.count] = y;
                check_out.is_biroqu[check_out.count] = true;
                check_out.count++;
                if( check_out.count >=2 ) return check_out;
            } 
            atk_x = atk_x + dx;
        }
    }


    // unobstructed vertical is attacker's Rook or Queen
    for( s8 dy = -1; dy < 2; dy+=2 ) {
        s8 atk_y = y + dy;
        while( in_bounds( x, atk_y ) ) {
            CHESS_PIECE piece = board[(u8)x][(u8)atk_y];
            if( piece.type != EMPTY && piece.player == player ) {
                break;
            }
            if( ( piece.type == ROOK || piece.type == QUEEN ) && piece.player == atkr) 
            {
                check_out.x[check_out.count] = x;
                check_out.y[check_out.count] = atk_y;
                check_out.is_biroqu[check_out.count] = true;
                check_out.count++;
                if( check_out.count >=2 ) return check_out;
            } 
            atk_y = atk_y + dy;
        }
    }

    return check_out;
}


bool is_block_square(s8 kings_x, s8 kings_y, s8 checker_x, s8 checker_y,  s8 blocker_x, s8 blocker_y) {

    // general direction of Checker.
    s8 dx = (checker_x > kings_x) ? 1 : (checker_x < kings_x) ? -1 : 0;
    s8 dy = (checker_y > kings_y) ? 1 : (checker_y < kings_y) ? -1 : 0;

    // same square (shouldn't happen)
    if (dx==0 && dy==0) {
        return false; 
    }

    // runtowrads the checker and see if we can block it
    s8 x = kings_x + dx;
    s8 y = kings_y + dy;
    while (!(x == checker_x && y == checker_y)) {
        if (x == blocker_x && y == blocker_y)  {
            return true;
        } 
        x += dx; y += dy;
    }
    return false;
}



bool any_valid_king_move( PLAYER player) {
    s8  x, y;
    if (!find_king( board, player, &x, &y)) return false;
    static const s8  KING_MOVE[8][2] = {
        {1,0},{-1,0},{0,1},{0,-1},{1,1},{1,-1},{-1,1},{-1,-1}
    };
    for (s8 i=0;i<8;i++){
        s8  nx = x + KING_MOVE[i][0], ny = y + KING_MOVE[i][1];
        if (!in_bounds(nx,ny)) continue;

        // avoid moving onto own piece (and keep castling out).
        if (!is_valid_move( x,y, nx,ny)) {
            continue;
        }

        CHESS_PIECE tmp[BOARD_SIZE][BOARD_SIZE]; // X, Y
        memcpy(board, tmp, sizeof(board) );
        tmp[nx][ny] = tmp[x][y];
        tmp[x][y].type = EMPTY;
        tmp[x][y].player = NO_PLAYER;

        if (!is_my_king_in_check(tmp, player)) {
            return true;
        }
    }
    return false;
}

bool has_any_valid_move(PLAYER player) {
    s8  kx;
    s8 ky;
    if (!find_king(board, player, &kx, &ky)) return false;

    bool in_check = is_my_king_in_check(board, player);
    if (!in_check) {
        // NOT in check → any legal move is fine (short-circuit on first).
        for (u8 x0=0;x0<8;x0++){
            for (u8 y0 = 0; y0 < 8 ;y0++){
                CHESS_PIECE p = board[x0][y0];
                if (p.player != player || p.type == EMPTY) continue;
                for (u8 y1=0;y1<8;y1++){
                    for (u8 x1=0;x1<8;x1++){
                        if (x0==x1 && y0==y1)  {
                            continue;
                        }
                        if (!is_valid_move(x0,y0,x1,y1)) {
                            continue;
                        } 

                        CHESS_PIECE tmp[BOARD_SIZE][BOARD_SIZE]; // X, Y
                        memcpy(board, tmp, sizeof(board) );
                        tmp[x1][y1] = tmp[x0][y0];
                        tmp[x0][y0].type = EMPTY;
                        tmp[x0][y0].player = NO_PLAYER;
                        if (!is_my_king_in_check(tmp, player)) return true;
                    }
                }
            }
        }
        return false;
    }

    // two checkers, there's no way to block/capture both in one move
    // so only check for king moves
    CHECKERS chk = find_checkers( player, kx, ky);
    if (chk.count >= 2) {

        return any_valid_king_move( player);
    }

    // Only one checker  king moves OR capture checker OR block checker (if is_biroqu).
    if (any_valid_king_move( player)) return true;

    // Try non-king pieces with constraints
    int cx = chk.x[0], cy = chk.y[0];
    bool is_biroqu = chk.is_biroqu[0];

    for (int y0=0;y0<8;y0++){
        for (int x0=0;x0<8;x0++){
            CHESS_PIECE p = board[y0][x0];
            if (p.player != player || p.type == EMPTY || p.type == KING) continue;

            for (int y1=0;y1<8;y1++){
                for (int x1=0;x1<8;x1++){
                    if (x0==x1 && y0==y1) continue;
                    // Must either capture the checker, or (if is_biroqu) block the line.
                    bool candidate = false;
                    if (x1 == cx && y1 == cy) {
                        candidate = true; // capturing the checker

                    } else if (is_biroqu && is_block_square(kx,ky,cx,cy,x1,y1)) {
                        candidate = true; // blocking square
                    } else {
                        candidate = false;
                    }
                    if (!candidate) {
                        continue;
                    } 

                    if (!is_valid_move(x0,y0,x1,y1)) {
                        continue;
                    } 

                    CHESS_PIECE tmp[BOARD_SIZE][BOARD_SIZE]; // X, Y
                    memcpy(board, tmp, sizeof(board) );
                    tmp[x1][y1] = tmp[x0][y0];
                    tmp[x0][y0].type = EMPTY;
                    tmp[x0][y0].player = NO_PLAYER;
                    if (!is_my_king_in_check(tmp, player)) return true;
                }
            }
        }
    }
    return false;
}



bool is_checkmate( PLAYER player ) 
{
    // checkmate if 
    // if king is in check 
    if( is_my_king_in_check(board, player ) == true ) {
        // and king has no valid moves
        // and no other piece can cancel check
        return !has_any_valid_move( player );
    }
    return false;
}


bool is_stalemate( PLAYER player ) 
{
    // stalemate if 
    // if king is NOT in check 
    if( is_my_king_in_check(board, player ) == false ) {
        // no legal moves left.
        return !has_any_valid_move( player );
    }

    return false;
}

bool have_any_valid_moves( PLAYER player ) {
    return false;
}



