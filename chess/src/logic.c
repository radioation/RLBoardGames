#include "logic.h"

////////////////////////////////////////////////////////////
// state variables
CHESS_PIECE board[BOARD_SIZE][BOARD_SIZE]; // X, Y

bool PLAYER_ONE_CAN_CASTLE_KING_SIDE = true;
bool PLAYER_ONE_CAN_CASTLE_QUEEN_SIDE = true;
bool PLAYER_TWO_CAN_CASTLE_KING_SIDE = true;
bool PLAYER_TWO_CAN_CASTLE_QUEEN_SIDE = true;




////////////////////////////////////////////////////////////
// move deltas
static const s8 KNIGHT_MOVES[8][2] = { {1, -2}, {2, -1}, {2, 1}, {1, 2}, {-1, 2}, {-2, 1}, {-2, -1}, {-1, -2} };

static const s8 DIAGONAL_MOVES[4][2] = { { -1, -1 }, { 1, -1 }, { 1, 1 }, { -1, 1 } };

// King (non-castling) 
static const s8 KING_MOVES [8][2] = { {1,0},{-1,0},{0,1},{0,-1},{1,1},{1,-1},{-1,1},{-1,-1} };


////////////////////////////////////////////////////////////
// utility
void clear_board() {
    for( u8 x=0; x < BOARD_SIZE; x++ ) {
        for( u8 y=0; y < BOARD_SIZE; y++ ){
            board[x][y].type = EMPTY;
            board[x][y].player = NO_PLAYER;
        }
    }
}

void reset_state() {
    clear_board();
    PLAYER_ONE_CAN_CASTLE_KING_SIDE = true;
    PLAYER_ONE_CAN_CASTLE_QUEEN_SIDE = true;
    PLAYER_TWO_CAN_CASTLE_KING_SIDE = true;
    PLAYER_TWO_CAN_CASTLE_QUEEN_SIDE = true;
}

#ifdef CLITEST
void draw_board( CHESS_PIECE b[BOARD_SIZE][BOARD_SIZE] ) {
    //printf("\n+-+-+-+-+\n");
    //printf("  0 1 2 3 4 5 6 7\n");
    for (int y=0;y<8;y++){
        //printf("%d ", y );
        for (int x=0;x<8;x++){
            const CHESS_PIECE *p = &b[x][y];
            char c='.';
            switch(p->type){
                case KING: 
                    c='K';
                    break; 
                case QUEEN:
                    c='Q';
                    break;
                case ROOK:
                    c='R'; 
                    break;
                case BISHOP:
                    c='B'; 
                    break;
                case KNIGHT:
                    c='N'; 
                    break;
                case PAWN:
                    c='P'; 
                    break;
                default: 
                    c='.'; 
                    break;
            }
            if (p->player==PLAYER_TWO) c = (char)(c + 32); 

            //printf("%c ", c);
        }
        //printf("\n");
    }
}
#endif

////////////////////////////////////////////////////////////
// Logic

bool in_bounds( s8 x, s8 y ) {
    if( x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE ) {
        return true;
    }
    return false;
}

/*
bool is_rank_empty_between(  s8 y, s8 x0, s8 x1 ) {
    s8 start_x = x0;
    s8 end_x = x1;
    if( x1 < x0 ) {
        start_x = x1;
        end_x = x0;
    }
    for( s8 x = start_x; x <= end_x; x++ ) {
        if( board[x][y].type == EMPTY ) {
            return false;
        } 
    }
    return true;
}
*/

bool is_rank_attacked_between( CHESS_PIECE b[BOARD_SIZE][BOARD_SIZE], s8 y, s8 x0, s8 x1, PLAYER player ) {
	//printf("irab  y %d x0 %d x1 %d\n",y, x0, x1 );
    s8 start_x = x0;
    s8 end_x = x1;
    if( x1 < x0 ) {
        start_x = x1;
        end_x = x0;
    }
    for( s8 x = start_x; x <= end_x; x++ ) {
        if( is_square_attacked( b, x, y, player )) {
	//printf("IS attacked?\n"); 
            return true;
        } 
    }
	//printf("NOT attacked?\n"); 
    return false;

}

bool can_castle_kingside( PLAYER player ) {
    if( is_my_king_in_check( board, player )) {
        return false;
    }
    if (player == PLAYER_ONE ) {   
        if( ! PLAYER_ONE_CAN_CASTLE_KING_SIDE ) {
	//printf("ccks 1\n");
            return false;
        }
        if( board[5][7].type != EMPTY || board[6][7].type != EMPTY ) {
	//printf("ccks 2\n");
            return false;
        } 
	//printf("ccks 3\n");
        return !is_rank_attacked_between( board, 7, 5, 6, player );
    } else {
        if( ! PLAYER_TWO_CAN_CASTLE_KING_SIDE ) {
            return false;
        }
        if( board[5][0].type != EMPTY || board[6][0].type != EMPTY ) {
            return false;
        } 
        return !is_rank_attacked_between( board, 0, 5, 6, player );
    }
    return false;
}
bool can_castle_queenside( PLAYER player ) {
    if( is_my_king_in_check( board, player )) {
        return false;
    }
    if (player == PLAYER_ONE ) {   
        if( !PLAYER_ONE_CAN_CASTLE_QUEEN_SIDE  ){
            return false;
        }
        if( board[1][7].type != EMPTY || board[2][7].type != EMPTY || board[3][7].type != EMPTY ) {
            return false;
        } 
        return !is_rank_attacked_between( board, 7, 1, 3, player );
    } else {
        if( !PLAYER_TWO_CAN_CASTLE_QUEEN_SIDE  ){
            return false;
        }
        if( board[1][0].type != EMPTY || board[2][0].type != EMPTY || board[3][0].type != EMPTY ) {
            return false;
        } 
        return !is_rank_attacked_between( board, 0, 1, 3, player );
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
    // check if moves look like castles?
    if( src.player == PLAYER_ONE ) {
        if( y0 == 7 && y1 == 7 &&  x0 == 4 && x1 == 6 ) {   
            return can_castle_kingside( src.player );
        } else if( y0 == 7 && y1 == 7 &&  x0 == 4 && x1 == 2 ) {   
            return can_castle_queenside( src.player ); 
        }
    } else if ( src.player == PLAYER_TWO ) {
        if( y0 == 0 && y1 == 0 &&  x0 == 4 && x1 == 6 ) {   
            return can_castle_kingside( src.player );
        } else if( y0 == 0 && y1 == 0 &&  x0 == 4 && x1 == 2 ) {   
            return can_castle_queenside( src.player ); 
        }
    }

    if( abs( x0 - x1 ) <= 1 && abs( y0 - y1 ) <=1 ){
        
        return true;
    }
    return false;
}


bool is_square_attacked( CHESS_PIECE b[BOARD_SIZE][BOARD_SIZE], u8 x, u8 y, PLAYER player ) {
    //printf(">>>>>> ISA 1\n");
    if( player == NO_PLAYER ) {
        return false; // nothing to atck in this square.
    }
    // just need to check square that can possibly attack king
    PLAYER atkr = PLAYER_ONE == player ? PLAYER_TWO : PLAYER_ONE;




    // PAWN : immediate diagonal from opposite side for pawns )
    //printf(">>>>>> ISA 2\n");
    s8 atk_y = y - 1 ;
    if( player == PLAYER_TWO ) {
        // check if player two is attacking player one's square
        atk_y = y + 1;         
    }
    if( in_bounds( (u8)(x-1), (u8)atk_y ) 
        && b[ (u8)(x-1) ][(u8)atk_y].type == PAWN 
        && b[(u8)(x-1)][(u8)atk_y].player == atkr ) {
        return true;
    }
    if( in_bounds( (u8)(x+1), (u8)atk_y ) 
        && b[ (u8)(x+1) ][(u8)atk_y].type == PAWN 
        && b[(u8)(x+1)][(u8)atk_y].player == atkr ) {
        return true;
    }

    // Knight : 8 possible pieces
    //printf(">>>>>> ISA 3\n");
    for (u8 i=0;i<8;i++){
        s8 atk_x = x + KNIGHT_MOVES[i][0];
        s8 atk_y = y + KNIGHT_MOVES[i][1];
        if (in_bounds(atk_x,atk_y) && b[(u8)atk_x][(u8)atk_y].type == KNIGHT && b[(u8)atk_x][(u8)atk_y].player == atkr ) {
            return true;
         }     
    }


    // unobstructed diagonal is attacker's Bishop or Queen
    //printf(">>>>>> ISA 4\n");
    for( u8 i=0; i < 4; i++ ) {
        s8 dx = DIAGONAL_MOVES[i][0];
        s8 dy = DIAGONAL_MOVES[i][1];
        s8 atk_x = x + dx;
        s8 atk_y = y + dy;
        while( in_bounds( atk_x, atk_y ) ) {
            CHESS_PIECE piece = b[(u8)atk_x][(u8)atk_y];
            if( ( piece.type != EMPTY && piece.type != KING ) && piece.player == player ) {
                break;
            }
            if( ( piece.type != EMPTY && piece.type != BISHOP && piece.type != QUEEN  ) && piece.player == atkr ) {
                // their non bishop and non queen piece will be in the way
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
    //printf(">>>>>> ISA 5\n");
    for( s8 dx = -1; dx < 2; dx+=2 ) {
        s8 atk_x = x + dx;
    //printf(">>>>>> ISA 5 : atk_x %d \n", atk_x);
        while( in_bounds( atk_x, y ) ) {
            //printf(">>>>>> ISA 5 : atk_x %d y %d \n", atk_x, y );
            CHESS_PIECE piece = b[(u8)atk_x][(u8)y];
            if( ( piece.type != EMPTY && piece.type != KING ) && piece.player == player ) {
                //printf(">>>>>> ISA 5 : blocked by type %d player %d \n", piece.type, piece.player );
                break;
            }
            if( ( piece.type != EMPTY && piece.type != ROOK && piece.type != QUEEN  ) && piece.player == atkr ) {
                // their non rook and non queen piece will be in the way
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
    //printf(">>>>>> ISA 6\n");
    for( s8 dy = -1; dy < 2; dy+=2 ) {
        s8 atk_y = y + dy;
    //printf(">>>>>> ISA 6 %d %d \n", x, atk_y);
        while( in_bounds( x, atk_y ) ) {
            CHESS_PIECE piece = b[(u8)x][(u8)atk_y];
            if( ( piece.type != EMPTY && piece.type != KING ) && piece.player == player ) {
                // my pieces can block
                break;
            }
            if( ( piece.type != EMPTY && piece.type != ROOK && piece.type != QUEEN  ) && piece.player == atkr ) {
                // their non rook and non queen piece will be in the way
                break;
            }
            
            if( ( piece.type == ROOK || piece.type == QUEEN ) && piece.player == atkr) 
            {
                return true;
            } 
            atk_y = atk_y + dy;
    //printf(">>>>>> ISA 6 %d %d \n", x, atk_y);
        }
    }

    //printf(">>>>>> ISA 7\n");
    // Kings (adjacent)
    for (u8 i=0;i<8;i++){
        s8 atk_x = x + KING_MOVES[i][0];
        s8 atk_y = y + KING_MOVES[i][1];
        if (in_bounds(atk_x,atk_y) && b[(u8)atk_x][(u8)atk_y].type == KING && b[(u8)atk_x][(u8)atk_y].player == atkr) return true;
    }
    //printf(">>>>>> ISA 8\n");
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
    //printf(">>>> IMKIC: ENTERED player: %d\n", player);
    
    s8 x=-1;
    s8 y=-1;
    //printf(">>>> IMKIC: find king\n");
    if( !find_king( b, player, &x, &y ) ) {
    //printf(">>>> IMKIC: NO king?\n");
        return false; // not found, cant' be in check.
    }
    //printf(">>>> IMKIC: found king\n");
    if( !in_bounds( (u8)x,(u8)y ) ) {
        // out of bounds? shouldn't ever happen but false.
        return false;
    }    
    //printf(">>>> IMKIC:  king in bounds\n");
    // check if the player's king's square is getting attacked.
    return is_square_attacked(b, x, y, player );

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
        // check if King is exposed with new setup
        CHESS_PIECE tmp[BOARD_SIZE][BOARD_SIZE]; // X, Y
        memcpy(tmp, board, sizeof(board) );
        tmp[(u8)x1][(u8)y1] = tmp[(u8)x0][(u8)y0];
        tmp[(u8)x0][(u8)y0].type = EMPTY;
        tmp[(u8)x0][(u8)y0].player = NO_PLAYER;

        //printf(" check king \n");
        valid = !is_my_king_in_check( tmp, src.player );
    } 

    // if we're here, nothing worked
    return valid;
}

void set_piece( s8 x, s8 y, PIECE_TYPE t, PLAYER p ) 
{
    board[(u8)x][(u8)y].type = t;
    board[(u8)x][(u8)y].player = p;
}

bool do_move ( s8 x0, s8 y0,  s8 x1, s8 y1 ) 
{
    // check in bounds
    if( !in_bounds( x0, y0 ) || !in_bounds( x1, y1 )) {
        return false;
    }
    CHESS_PIECE moving_piece = board[(u8)x0][(u8)y0];
    PLAYER player = moving_piece.player;

    PIECE_TYPE captured_type = board[x1][y1].type; 

    // check  for rook capture    
    if( captured_type == ROOK ) {
        // if a rook is captured on it's original square turn off the boolean.
        if( y1 == 7 ) {
            if( x1 == 7  && PLAYER_ONE_CAN_CASTLE_KING_SIDE ) {
                PLAYER_ONE_CAN_CASTLE_KING_SIDE= false;  
            } else if ( x1 == 0  && PLAYER_ONE_CAN_CASTLE_QUEEN_SIDE ) {
                PLAYER_ONE_CAN_CASTLE_QUEEN_SIDE = false;
            }
        } else if ( y1 == 0 ) {
            if( x1 == 7  && PLAYER_TWO_CAN_CASTLE_KING_SIDE ) {
                PLAYER_TWO_CAN_CASTLE_KING_SIDE= false;  
            } else if ( x1 == 0  && PLAYER_TWO_CAN_CASTLE_QUEEN_SIDE ) {
                PLAYER_TWO_CAN_CASTLE_QUEEN_SIDE = false;
            }
        }

    } 

    // do the move
    board[(u8)x1][(u8)y1] = moving_piece;
    board[(u8)x0][(u8)y0].type = EMPTY;
    board[(u8)x0][(u8)y0].player = NO_PLAYER;

    // check for king castle
    bool is_castling = ( moving_piece.type == KING) && 
        (( y0 == 0 || y0 == 7 ) &&  (y0 == y1 )) &&
        (( x0 == 4 ) && ( x1 == 6 || x1 == 2 ) );


    if ( is_castling ) {
        if( player == PLAYER_ONE ) {
            if( x1 == 6 ) { // king side
                board[7][7].type = EMPTY;
                board[7][7].player = NO_PLAYER;
                board[5][7].type = ROOK;
                board[5][7].player = PLAYER_ONE;
            } else{ // queen siide.
                board[0][7].type = EMPTY;
                board[0][7].player = NO_PLAYER;
                board[3][7].type = ROOK;
                board[3][7].player = PLAYER_ONE;
            }    
        } else if (player == PLAYER_TWO ) {
            if( x1 == 6 ) { // king side
                board[7][0].type = EMPTY;
                board[7][0].player = NO_PLAYER;
                board[5][0].type = ROOK;
                board[5][0].player = PLAYER_TWO;
            } else{ // queen siide.
                board[0][0].type = EMPTY;
                board[0][0].player = NO_PLAYER;
                board[3][0].type = ROOK;
                board[3][0].player = PLAYER_TWO;
            }    
        }
    }

    // if moving pieces is a king disable casting
    if( moving_piece.type == KING ) {
        if( moving_piece.player == PLAYER_ONE ) {
            PLAYER_ONE_CAN_CASTLE_KING_SIDE = false;
            PLAYER_ONE_CAN_CASTLE_QUEEN_SIDE = false;
        }  else {
            PLAYER_TWO_CAN_CASTLE_KING_SIDE = false;
            PLAYER_TWO_CAN_CASTLE_QUEEN_SIDE = false;
        }
    } else if ( moving_piece.type == ROOK ) {
        if( y0 == 7 ) {
            if( x0 == 7  && PLAYER_ONE_CAN_CASTLE_KING_SIDE ) {
                PLAYER_ONE_CAN_CASTLE_KING_SIDE= false;  
            } else if ( x0 == 0  && PLAYER_ONE_CAN_CASTLE_QUEEN_SIDE ) {
                PLAYER_ONE_CAN_CASTLE_QUEEN_SIDE = false;
            }
        } else if ( y0 == 0 ) {
            if( x0 == 7  && PLAYER_TWO_CAN_CASTLE_KING_SIDE ) {
                PLAYER_TWO_CAN_CASTLE_KING_SIDE= false;  
            } else if ( x0 == 0  && PLAYER_TWO_CAN_CASTLE_QUEEN_SIDE ) {
                PLAYER_TWO_CAN_CASTLE_QUEEN_SIDE = false;
            }
        }

    }


    return true;
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
    //printf(">> AVKM: FOUND KING\n");
    for (u8 i=0;i<8;i++){
        s8 nx = x + KING_MOVE[i][0];
        s8 ny = y + KING_MOVE[i][1];
        //printf(">> AVKM: i: %d new x: %d new y %d \n", i, nx, ny );
        if (!in_bounds(nx,ny)) {
            //printf(">> AVKM: out of bounds, skip\n");
            continue;
        }
        // avoid moving onto own piece (and keep castling out).
        if (!is_valid_move( x,y, nx,ny)) {
            //printf(">> AVKM: invalid move, skip\n");
            continue;
        }

        CHESS_PIECE tmp[BOARD_SIZE][BOARD_SIZE]; // X, Y
        memcpy(tmp, board, sizeof(board) );
        // move to new space
        tmp[(u8)nx][(u8)ny] = tmp[(u8)x][(u8)y];
        // clear old space
        tmp[(u8)x][(u8)y].type = EMPTY;
        tmp[(u8)x][(u8)y].player = NO_PLAYER;

        //printf(">> AVKM: call is_my_king_in_check()\n");
#ifdef CLITEST
        draw_board(board);
        draw_board(tmp);
#endif
        if (!is_my_king_in_check(tmp, player)) {
            //printf(">> AVKM: NOT IN CHECK i: %d new x: %d new y %d \n", i, nx, ny );
            return true;
        }
    }
    return false;
}

bool has_any_valid_move(PLAYER player) {
    s8 kx;
    s8 ky;
    if (!find_king(board, player, &kx, &ky)) return false;
    //printf("FOUND KING \n" );
    bool in_check = is_my_king_in_check(board, player);
    //printf(" KING IN CHECK? %d \n", in_check );
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
                        memcpy(tmp, board, sizeof(board) );
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
    //printf(" checkers count: %d \n", chk.count );
    if (chk.count >= 2) {

        return any_valid_king_move( player);
    }

    // Only one checker  king moves OR capture checker OR block checker (if is_biroqu).
    //printf(" check for valid king move\n");
    if (any_valid_king_move( player)) {
        //printf(" VALID KING MOVE FOUND\n");
        return true;
    }

    //printf(" No valid king move found\n");

    // Try non-king pieces with constraints
    s8 checker_x = chk.x[0];
    s8 checker_y  = chk.y[0];
    bool is_biroqu = chk.is_biroqu[0];
    //printf("  is checker Bishop Rook or Queen? %d \n", is_biroqu );

    // entire board
    for (s8 y0=0;y0<8;y0++){
        for (s8 x0=0;x0<8;x0++){
            CHESS_PIECE p = board[(u8)x0][(u8)y0];
            if (p.player != player || p.type == EMPTY || p.type == KING) {

                continue;
            }
            // found a piece on our team
            //printf("   minion %d at %d,%d\n", p.type, x0, y0 );
            for (s8 y1=0;y1<8;y1++){
                for (s8 x1=0;x1<8;x1++){

                    if (x0==x1 && y0==y1) continue; // skip the current piece being checked

                    // Must either capture the checker, or (if is_biroqu) block the line.
                    bool candidate = false;
                    if (x1 == checker_x && y1 == checker_y) {
                        candidate = true; // capturing the checker
                    } else if (is_biroqu && is_block_square(kx,ky,checker_x,checker_y,x1,y1)) {
                        //printf("  attacker can be blocked! \n");
                        candidate = true; // blocking square
                    } else {
                        candidate = false;
                    }
                    if (!candidate) {
                        continue;
                    } 
                    //printf("test move from %d,%d to %d,%d\n", x0,y0,x1,y1);
                    if (!is_valid_move(x0,y0,x1,y1)) {
                        //printf("   invalid move from %d,%d to %d,%d\n", x0,y0,x1,y1);

                        continue;
                    } 

                    CHESS_PIECE tmp[BOARD_SIZE][BOARD_SIZE]; // X, Y
                    memcpy(tmp, board, sizeof(board) );
                    tmp[(u8)x1][(u8)y1] = tmp[(u8)x0][(u8)y0];
                    tmp[(u8)x0][(u8)y0].type = EMPTY;
                    tmp[(u8)x0][(u8)y0].player = NO_PLAYER;

                    //printf(" check king \n");
#ifdef CLITEST
                    draw_board( board );
                    draw_board( tmp );
#endif
                    if (!is_my_king_in_check(tmp, player)) {
                        return true;
                    }
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
        //printf("----------------------------------------------");
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




