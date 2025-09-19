#include <genesis.h>
#include "resources.h"
#include "network.h" 

#define INPUT_WAIT_COUNT 10

#define SND_MOVE 64
#define SND_BUZZ 63

#define BOARD_SIZE 8


// Enum to represent piece types (using offsets for lookup into image)
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

#define FILE_X 97
#define RANK_Y 49

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



int text_cursor_x, text_cursor_y;
u8 buttons, buttons_prev;

////////////////////////////////////////////////////////////////////////////
// network stuff
char server[16] = "010.025.050.061";
char request[64];
char response[128];
char game_id[16];
char player_id[16];
bool singlePlayer = false;
u8 whoAmI = NO_PLAYER;

void read_bytes_n(u8* data, u8 length ) {
    s16 bytePos = 0;
    while( bytePos < length ) {
        // read data
        if( NET_RXReady() ) {
            data[bytePos] = NET_readByte(); // Retrieve byte from RX hardware Fifo directly
            bytePos++;
        } else {
            waitMs(5);
        }
    }
}

s16 read_line(u8* data, u8 data_len ){
    s16 bytePos = 0;
    while( bytePos < data_len ) {
        // read data
        if( NET_RXReady() ) {
            data[bytePos] = NET_readByte(); // Retrieve byte from RX hardware Fifo directly
            if( data[bytePos] == 0x0A ) {
                data[bytePos] = 0;
                return bytePos;
            }
            bytePos++;
        } else {
            waitMs(5);
        }
    } 
    return bytePos;

}



// Chess Piece data
CHESS_PIECE board[BOARD_SIZE][BOARD_SIZE]; // X, Y
int piecesTileIndex = -1;
const s8 boardStartCol = 8;
const s8 boardStartRow = 2;
const s8 boardStep = 3;

void clear_board() {
    for( u8 x=0; x < BOARD_SIZE; x++ ) {
        for( u8 y=0; y < BOARD_SIZE; y++ ){
            board[x][y].type = EMPTY;
            board[x][y].player = NO_PLAYER;
        }
    }
}

void setup_pieces() {
    // clear the board
    memset(board, 0, sizeof(CHESS_PIECE) * 8 * 8); // Set all to empty

    // set  pieces up
    board[0][7] = (CHESS_PIECE){ROOK, PLAYER_ONE};   board[7][7] = (CHESS_PIECE){ROOK, PLAYER_ONE};
    board[1][7] = (CHESS_PIECE){KNIGHT, PLAYER_ONE}; board[6][7] = (CHESS_PIECE){KNIGHT, PLAYER_ONE};
    board[2][7] = (CHESS_PIECE){BISHOP, PLAYER_ONE}; board[5][7] = (CHESS_PIECE){BISHOP, PLAYER_ONE};
    board[3][7] = (CHESS_PIECE){QUEEN, PLAYER_ONE};  board[4][7] = (CHESS_PIECE){KING, PLAYER_ONE};
    for (int i = 0; i < 8; i++) {
        board[i][6] = (CHESS_PIECE){PAWN, PLAYER_ONE};
    }

    board[0][0] = (CHESS_PIECE){ROOK, PLAYER_TWO};   board[7][0] = (CHESS_PIECE){ROOK, PLAYER_TWO};
    board[1][0] = (CHESS_PIECE){KNIGHT, PLAYER_TWO}; board[6][0] = (CHESS_PIECE){KNIGHT, PLAYER_TWO};
    board[2][0] = (CHESS_PIECE){BISHOP, PLAYER_TWO}; board[5][0] = (CHESS_PIECE){BISHOP, PLAYER_TWO};
    board[3][0] = (CHESS_PIECE){QUEEN, PLAYER_TWO};  board[4][0] = (CHESS_PIECE){KING, PLAYER_TWO};
    for (int i = 0; i < 8; i++) {
        board[i][1] = (CHESS_PIECE){PAWN, PLAYER_TWO};
    }
}

void draw_pieces(){
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if( board[col][row].player > 0 ) {
                s8 yStart = 0;
                if( board[col][row].player == 2 ) {
                    yStart = 3;
                }
                VDP_setTileMapEx( BG_A, pieces_img.tilemap, TILE_ATTR_FULL(PAL1, TRUE, FALSE, FALSE, piecesTileIndex),    
                        boardStartCol + col * boardStep,  // PLANE X Dest in tiles
                        boardStartRow + row * boardStep,  // PLANE Y Dest in tiles
                        board[col][row].type,  // REGION X start
                        yStart,  // REGION Y start
                        boardStep,  // Width
                        boardStep,  // Height
                        CPU);
            } else {
                VDP_setTileMapEx( BG_A, pieces_img.tilemap, TILE_ATTR_FULL(PAL1, TRUE, FALSE, FALSE, piecesTileIndex),    
                        boardStartCol + col * boardStep,  // PLANE X Dest in tiles
                        boardStartRow + row * boardStep,  // PLANE Y Dest in tiles
                        0,  // REGION X start
                        0,  // REGION Y start
                        boardStep,  // Width
                        boardStep,  // Height
                        CPU);
            }
        }
    }
}

void clear_space( s8 startCol, s8 startRow ) {
    VDP_setTileMapEx( BG_A, pieces_img.tilemap, TILE_ATTR_FULL(PAL1, TRUE, FALSE, FALSE, piecesTileIndex),    
            boardStartCol + startCol * boardStep,  // PLANE X Dest in tiles
            boardStartRow + startRow * boardStep,  // PLANE Y Dest in tiles
            EMPTY,  // REGION X start
            0,  // REGION Y start
            boardStep,  // Width
            boardStep,  // Height
            CPU);

}

void move_piece( s8 startCol, s8 startRow, s8 endCol, s8 endRow, s8 promotype ){
    //if( do_move( startCol, startRow, endCol, endRow ) ) {
    PLAYER p = board[startCol][startRow].player;
    PIECE_TYPE cp = board[startCol][startRow].type;
    board[endCol][endRow] = board[startCol][startRow];
    board[startCol][startRow] = (CHESS_PIECE){EMPTY, NO_PLAYER}; 

    // check for special cases
    /*
       ' castles to check
       ' white
       ' e1g1 -  4,7,6,7
       ' e1c1 -  4,7,2,7
       ' black
       ' e8g8 -  4,0,6,0
       ' e8c8 -  4,0,2,0
     */
    
    if ( cp == KING && p == PLAYER_TWO &&  startCol == 4 && startRow ==0 && endCol == 6 && endRow == 0 ) {
        // move black rook from right
        board[7][0] = (CHESS_PIECE){EMPTY, NO_PLAYER}; 
        clear_space( 7, 0 );
        board[5][0] = (CHESS_PIECE){ROOK, p}; 

    } else if ( cp == KING && p == PLAYER_TWO && startCol == 4 && startRow ==0 && endCol == 2 && endRow == 0 ) {
        // move rook from left
        board[0][0] = (CHESS_PIECE){EMPTY, NO_PLAYER}; 
        clear_space( 0, 0 );
        board[3][0] = (CHESS_PIECE){ROOK, p}; 
    } else if ( cp == KING && p == PLAYER_ONE && startCol == 4 && startRow ==7 && endCol == 6 && endRow == 7 ) {
        // move rook from right
        board[7][7] = (CHESS_PIECE){EMPTY, NO_PLAYER}; 
        clear_space( 7, 7 );
        board[5][7] = (CHESS_PIECE){ROOK, p}; 
    } else if ( cp == KING && p == PLAYER_ONE && startCol == 4 && startRow ==7 && endCol == 2 && endRow == 7 ) {
        // move rook from left
        board[0][7] = (CHESS_PIECE){EMPTY, NO_PLAYER}; 
        clear_space( 0, 7 );
        board[3][7] = (CHESS_PIECE){ROOK, p}; 
    }
    // if pawn,

    draw_pieces();
    clear_space( startCol, startRow );

    //}
}

// Sprite data structures
typedef struct 
{
    Sprite *sprite;
    s8 col;     // board col
    s8 row;     // board row
    s16 pos_x;  
    s16 pos_y;
    s8 sel_col; // selected board column
    s8 sel_row; // selected board row
    s16 sel_pos_x;
    s16 sel_pos_y;
    Sprite *selected_spr;
} CURSOR;

const s8 cursorStep = 24;
const s8 cursorColStart = 64;
const s8 cursorRowStart = 16;

void cursor_init( CURSOR *cursor, Sprite *sprite, Sprite *selected_sprite ) {
    cursor->col = 4;  // board position
    cursor->row = 4;
    cursor->pos_x = cursor->col * cursorStep + cursorColStart;
    cursor->pos_y = cursor->row * cursorStep + cursorRowStart;
    cursor->sprite =  sprite;

    cursor->sel_col = -1;  // not on board
    cursor->sel_row = -1;
    cursor->sel_pos_x = -32;
    cursor->sel_pos_y = -32;
    cursor->selected_spr = selected_sprite;
    SPR_setAnim( cursor->selected_spr, 1 );
    SPR_setVisibility( cursor->selected_spr, HIDDEN );
}

bool cursor_move( CURSOR *cursor, u16 joypad ) {
    bool didMove = FALSE;
    if( joypad & BUTTON_LEFT ) {
        cursor->col--;
        if( cursor->col < 0 ) {
            cursor->col = 7;
        }
        cursor->pos_x = cursor->col * cursorStep + cursorColStart;
        didMove = TRUE;
    } 
    if( joypad & BUTTON_RIGHT ) {
        cursor->col++;
        if( cursor->col > 7 ) {
            cursor->col = 0;
        }
        cursor->pos_x = cursor->col * cursorStep + cursorColStart;
        didMove = TRUE;
    } 
    if( joypad & BUTTON_UP ) {
        cursor->row--;
        if( cursor->row < 0 ) {
            cursor->row = 7;
        }
        cursor->pos_y = cursor->row * cursorStep + cursorRowStart;
        didMove = TRUE;
    }
    if( joypad & BUTTON_DOWN ) {
        cursor->row++;
        if( cursor->row > 7 ) {
            cursor->row = 0;
        }
        cursor->pos_y = cursor->row * cursorStep + cursorRowStart;
        didMove = TRUE;
    }
    return didMove;
}

void cursor_update_from_pos( CURSOR *cursor, s8 col, s8 row, s8 sel_col, s8 sel_row ) {
    cursor->col = col;
    cursor->pos_x = cursor->col * cursorStep + cursorColStart;
    cursor->row = row;
    cursor->pos_y = cursor->row * cursorStep + cursorRowStart;

    cursor->sel_col = sel_col;
    cursor->sel_pos_x = cursor->sel_col * cursorStep + cursorColStart;
    cursor->sel_row = sel_row;
    cursor->sel_pos_y = cursor->sel_row * cursorStep + cursorRowStart;
    if( cursor->sel_col >= 0 ) {
        SPR_setVisibility( cursor->selected_spr, VISIBLE );
    } else {
        SPR_setVisibility( cursor->selected_spr, HIDDEN );
    }
}

void cursor_clear_selected( CURSOR* cursor ) {
    cursor->sel_col = -1;
    cursor->sel_row = -1;
    cursor->sel_pos_x = -32;
    cursor->sel_pos_y = -32;
    SPR_setVisibility( cursor->selected_spr, HIDDEN );
    char message[40];
    strclr(message);
    sprintf( message, "X: %d y: %d sx: %d sy %d    ", cursor->col, cursor->row, cursor->sel_col, cursor->sel_row);
}



bool cursor_action( CURSOR* cursor, CHESS_PIECE brd[8][8], u8 player ) {
    if( cursor->sel_col < 0 ) {
        // no piece selected yet, check if player owns the current piece.
        if( brd[(u8)cursor->col][(u8)cursor->row].player == player ) { 
            cursor->sel_col = cursor->col;
            cursor->sel_row = cursor->row;
            cursor->sel_pos_x = cursor->sel_col * cursorStep + cursorColStart;
            cursor->sel_pos_y = cursor->sel_row * cursorStep + cursorRowStart;
            SPR_setVisibility( cursor->selected_spr, VISIBLE );
        }
    } else {
        //char message[40];
        //strclr(message);
        //sprintf( message, "X: %d y: %d sx: %d sy %d    ", cursor->col, cursor->row, cursor->sel_col, cursor->sel_row);
        //VDP_drawText( message, 0, 1 );
        // return true if destination is clear or a different player, BUT DON"T UPDATE BOARD 
        return ( brd[(u8)cursor->col][(u8)cursor->row].player != player );

    }
    return false;
}



void setup_game() {
    // get input for level.
    VDP_clearTextArea( 0, 0,  40, 20  );
    s8 level = 1;
    s8 players = 1;
    char message[40];
     s8 selectedRow = 5;
    while(1) {
        //

        u16 joypad  = JOY_readJoypad( JOY_1 );
        if( joypad & BUTTON_UP || joypad & BUTTON_DOWN ) {
            selectedRow = selectedRow == 5 ? 6 : 5;
        }
        if (selectedRow == 5) {
            if( joypad & BUTTON_LEFT  || joypad & BUTTON_RIGHT ) {
                players = players == 1 ? 2 : 1;
            }
        }
        if (selectedRow == 6) {
            if( joypad & BUTTON_LEFT  ) {
                level -=1;
                if( level < 1) {
                    level = 10;
                }
            }
            if( joypad & BUTTON_RIGHT ) {
                level +=1;
                if( level > 10) {
                    level = 1;
                }
            }
        }



        if( joypad & BUTTON_START ) {
            break;
        }

        sprintf( message, "  players (1-2): %d  ", players );
        VDP_drawText(message, 11, 5);
        sprintf( message, "   level (1-10): %d  ", level );
        VDP_drawText(message, 11, 6);
        VDP_drawText("*", 11, selectedRow);
        SYS_doVBlankProcess();
        waitMs(200);

    }

    memset( message, 0, sizeof(message ));
    if( players == 1 ) {
        sprintf( message, "N:S:W:%d\n", level );
    } else {
        sprintf( message, "N:D:W:%d\n", level );
    }
    start_game(message);
    singlePlayer = true;
}




void start_game(char* msg) {

/*
HELO
N:S:W:1
ACK 0F673352:2CACA131
*/

    // reach out to server 
    text_cursor_y = 5;
    // blocks whilewaiting for network to be ready.
    char fullserver[21];
    memset(fullserver,0, sizeof(fullserver));
    sprintf( fullserver, "%s:55558", server);
    NET_connect(text_cursor_x, text_cursor_y, fullserver); text_cursor_x=0; text_cursor_y++;


    s16 count = read_line( response, sizeof(response) );
    response[4] = 0;
 
    if( strcmp( response, "HELO" ) != 0 ) {
        // TODO: we need to handle this error somehow. think about it
        //VDP_drawText("NOT HELO?", 0, 2 );
        return;
    }
    
    // request a new game.
    NET_sendMessage( msg );
    
    memset(response, 0, sizeof(response ));
    count = read_line( response, sizeof(response) );
   
  
    memset( game_id, 0, sizeof( game_id ) );
    memset( player_id, 0, sizeof( player_id ) );

    if ( count >= 3 ) {
        // if starts with ACK, save the rest into game_id
        //              11111111111
        //    012345678901234567890
        //    ACK e78c2852:b6dc3dda


        if( response[0] == 'A' && response[1] == 'C' && response[2] == 'K' ) {
            // get game id
            strncpy(  game_id, response + 4, 8 );
            strncpy(  player_id, response + 13, 8 );
            VDP_drawText(game_id, 16, 0 );

        } else {
            // TODO: handle error somehow.
        }
    } 

}


bool join_game() {
    //VDP_drawText("   Connect to server    ", 0, 5);
    text_cursor_y = 5;
    // blocks whilewaiting for network to be ready.
    char fullserver[21];
    memset(fullserver,0, sizeof(fullserver));
    sprintf( fullserver, "%s:55558", server);
    NET_connect(text_cursor_x, text_cursor_y, fullserver); text_cursor_x=0; text_cursor_y++;

    s16 count = read_line( response, sizeof(response) );
    response[4] = 0;
    if( strcmp( response, "HELO" ) != 0 ) {
        // we need to handle this error somehow. think about it
        return;
    }

    // get a list of games
    /*
      L:
 
      ACK F64C68C7:5AAAEC90:BC14B103:4EEA1451:4F538D91
    */
    // send out LIST command
    NET_sendMessage("L:\n");
    // wait until we can read bytes.
    while( ! NET_RXReady() ) { }

    //            11111111112222222222333333333344444444
    //  012345678901234567890123456789012345678901234567
    //  ACK F64C68C7:5AAAEC90:BC14B103:4EEA1451:4F538D91
    //
    //     48 bytes, response[48] = 0
    s16 byteCount = read_line( response, sizeof(response) );
    //VDP_drawText(response, 0, 0 );
    //char message[40];
    //strclr(message);
    //sprintf( message, "Byte Count: %d", byteCount );
    //VDP_drawText(message, 10, 1 );
    
    
    if( byteCount > 5 ) {
        // parse up response
        // skip the ACK in  'ACK 90CE42ED:329829E7'
        char *start = response + 4; 
        //    skip 4
        //            1111111111222222222233333333334444
        //  01234567890123456789012345678901234567890123
        //  F64C68C7:5AAAEC90:BC14B103:4EEA1451:4F538D91
        //
        //     (44 bytes + 1) / 9 = 5
        char gameIds[128];
        memset( gameIds, 0, sizeof( gameIds ) );

        memcpy( gameIds, start, byteCount - 4 );
        gameIds[byteCount -3] = 0;
        //VDP_drawText(gameIds, 0, 2 );
      
        s8 rowOffset = 6;
        s8 gameIdCount = (byteCount -3 ) / 9;
        //sprintf( message, "Game ID  Count: %d   ", gameIdCount );
        //VDP_drawText(message, 10, 3 );
        for( s16 i = 0; i < gameIdCount; ++ i ) {
            gameIds[i*9 +8] = 0;
        }

        for( s16 i = 0; i < gameIdCount; ++ i ) {
            VDP_drawText(gameIds + i*9, 16, rowOffset+i );
        }

        s8 selectedRow = 0;
        while(1) {
            // 
            u16 joypad  = JOY_readJoypad( JOY_1 );
            if( joypad & BUTTON_UP  ) {
                VDP_drawText(" ", 11, rowOffset + selectedRow);
                selectedRow -= 1;
                if( selectedRow < 0 ) {
                    selectedRow = gameIdCount -1;
                }
            }
            if( joypad & BUTTON_DOWN ) {
                VDP_drawText(" ", 11, rowOffset + selectedRow);
                selectedRow += 1;
                if( selectedRow >= gameIdCount ) {
                    selectedRow = 0;
                }
            }

            if( joypad & BUTTON_START ) {
                break;
            }
            
            VDP_drawText("*", 11, rowOffset + selectedRow);
            SYS_doVBlankProcess();
            waitMs(200);
        }

        memset(request,0, sizeof(request));
        memset( game_id, 0, sizeof( game_id ) );
        strncpy(  game_id, gameIds + selectedRow * 9, 8 );
        sprintf( request, "J:%s\n", game_id );
        NET_sendMessage( request );
     
        while( ! NET_RXReady() ) { }
        byteCount = read_line( response, sizeof(response) );
        if( byteCount > 5 ) {
            //  J:4F538D91

            //  012345678901234567890
            //  ACK A673E220
            memset( player_id, 0, sizeof( player_id ) );
            strncpy(  player_id, response + 4, 8 );
        }
    }
}

void do_move() {
}

void get_board() {
}


void setWhoAmI() {
    buttons = 0;
    buttons_prev = 0;
    VDP_clearTextArea( 0, 0,  40, 20  );
    VDP_drawText( "Server Address", 13 ,2 );
    VDP_drawText( server, 13 , 3 );
    VDP_drawText("         (A) - Start Game", 0, 5);
    VDP_drawText("         (C) - Join Game", 0, 7);
    NET_resetAdapter();
    while(1) // loop forever
    {
        buttons = JOY_readJoypad(JOY_1);
        // MODE NOT SET, button press will determine server or client.
        if(buttons & BUTTON_A && buttons_prev == 0x00) {
            VDP_clearTextArea( 0, 5,  40, 3 );
            text_cursor_y = 5;
            whoAmI = PLAYER_ONE;
            // start listening
            setup_game();
            break;
        }else if(buttons & BUTTON_C && buttons_prev == 0x00) {
            VDP_clearTextArea( 0, 5,  40, 3 );
            // try to connect to server.
            whoAmI = PLAYER_TWO;
            join_game();
            break;

        }
        buttons_prev = buttons;
        SYS_doVBlankProcess();
    }
    NET_flushBuffers();

}


bool send_move( CURSOR* cursor, u8 type  ) {

    // TODO: promote pawns...
    char move[5];
    move[0] = FILE_X + cursor->sel_col;
    move[1] = RANK_Y + 7 - cursor->sel_row;
    move[2] = FILE_X + cursor->col;
    move[3] = RANK_Y + 7 - cursor->row;
    move[4] = 0;

    memset( request,0, sizeof(request) ); 
    sprintf(request,"M:%s:%s:%s\n", game_id, player_id, move );
    NET_sendMessage(request);
    s16 count = read_line( response, sizeof(response) );


    /*
    M:e78c2852:b6dc3dda
    ERR:bad format
    M:e78c2852:b6dc3dda:d2d3
    ACK d7d5
    
    M:e78c2852:b6dc3dda:d1d3
    ACK illegal move
    M:e78c2852:b6dc3dda:e2e4
    ACK e7e6
    */


    if( strcmp( response, "ACK legal move" ) == 0 ) {
        move_piece( cursor->sel_col, cursor->sel_row, cursor->col, cursor->row, 0 );
        return true;
    }

    return false;
  
}



void read_status( ){
    /*
    S:e78c2852
    ACK TURN -:LAST -----:MVNO 0
    */

    // send out STATUS command
    strclr( request ); 
    sprintf(request,"S:%s\n", game_id );
    NET_sendMessage(request);

    // wait until we can read bytes.
    while( ! NET_RXReady() ) {
    }
    s16 bytes = read_line( response, sizeof(response) );
}

void read_board( ){
    /*
    B:e78c2852
    ACK rnbqkbnrppp..ppp....p......p........P......P....PPP..PPPRNBQKBNR
    */
    
    // send out BOARDS command
    strclr( request ); 
    sprintf(request,"B:%s\n", game_id );
    NET_sendMessage(request);
    while( ! NET_RXReady() ) {
    }
    s16 bytes = read_line( response, sizeof(response) );
}



int main(bool hard) {

    //////////////////////////////////////////////////////////////
    // Setup Sound
    XGM_setPCM(  SND_MOVE, move_snd, sizeof(move_snd));
    XGM_setPCM(  SND_BUZZ, buzz_snd, sizeof(buzz_snd));


    //////////////////////////////////////////////////////////////
    // setup screen and palettes
    SYS_disableInts();
    VDP_setScreenWidth320();
    VDP_setScreenHeight224();
    VDP_setBackgroundColor(16);
    SYS_enableInts();                      // enable interrupts for networking print routines.


    //////////////////////////////////////////////////////////////
    // Networking setup
    text_cursor_x = 0; // networking text cursor location
    text_cursor_y = 0; // networking text cursor location
    SPR_init();

    VDP_drawText("Detecting adapter...[  ]", text_cursor_x, text_cursor_y); text_cursor_x+=21;   
    NET_initialize(); // Detect cartridge and set 'cart_present'
    whoAmI= 0; // 0 - not set, 1 - PLAYER_ONE/HOST/LIGHT, 2 - PLAYER_TWO/CLIENT/DARK

    if(cart_present)
    {
        VDP_setTextPalette(2); // Green text
        VDP_drawText("Ok", text_cursor_x, text_cursor_y); text_cursor_x=0; text_cursor_y+=2;
        VDP_setTextPalette(0); // White text


        VDP_drawText("IP Address:", text_cursor_x, text_cursor_y);
        NET_printIP(text_cursor_x+12, text_cursor_y); text_cursor_y++;

        VDP_drawText("MAC:", text_cursor_x, text_cursor_y);
        NET_printMAC(text_cursor_x+5, text_cursor_y); text_cursor_y+=2;

        waitMs(2000);


        // get server address from SRAM or user.
        SRAM_enable();
        u8 part = SRAM_readByte(0);
        sprintf( server, "%03d.", part );
        part = SRAM_readByte(1);
        sprintf( server+4, "%03d.", part );
        part = SRAM_readByte(2);
        sprintf( server+8, "%03d.", part );
        part = SRAM_readByte(3);
        sprintf( server+12, "%03d", part );


        VDP_drawText( server, 13 , 3 );

        getIPFromUser(server);

        SYS_doVBlankProcess();
        // clear out last input and wait a sec.
        waitMs(1000);
        JOY_update();

        VDP_drawText( "Server Address", 13 ,12 );
        VDP_drawText( server, 13 , 13 );

        // TODO: add ping here and save if successful.
        //  for now, saving always.
        SRAM_writeByte(0, atoi( server ));
        SRAM_writeByte(1, atoi( server + 4 ));
        SRAM_writeByte(2, atoi( server + 8 ));
        SRAM_writeByte(3, atoi( server + 12));

        whoAmI = 0; // 0 - not set, 1 - PLAYER_ONE, 2 - PLAYER_TWO

        setWhoAmI(); // loops until true. TODO: let you break out and stay local
                     // or loop back to pick a different host.


        VDP_drawText( " Press Start  ", 13 ,12 );
        if( whoAmI == 1 ) {
            while(1) {
                u16 joypad  = JOY_readJoypad( JOY_1 );
                SYS_doVBlankProcess();
                if( joypad & BUTTON_START ) {
                    break;
                }
            }
        }
        singlePlayer = true;


    }
    else
    {
        VDP_setTextPalette(1); // Red text
        VDP_drawText("XX", text_cursor_x, text_cursor_y); text_cursor_x=0; text_cursor_y+=2;
        VDP_setTextPalette(0); // White text
        VDP_drawText("Adapter not present", text_cursor_x, text_cursor_y);
        while(1) { 
            u16 joypad = JOY_readJoypad( JOY_1 );
            if( joypad & BUTTON_START ) {
                break;
            }
            SYS_doVBlankProcess();
        }
        whoAmI = 1; 
    }
    VDP_clearPlane( BG_A, TRUE);
    VDP_clearPlane( BG_B, TRUE);

    //////////////////////////////////////////////////////////////
    // setup background
    PAL_setPalette( PAL0, board_pal.data, CPU );
    PAL_setPalette( PAL1, pieces_pal.data, CPU );
    PAL_setPalette( PAL2, cursor_pal.data, CPU );

    int ind = TILE_USER_INDEX;
    VDP_drawImageEx(BG_B, &board_img, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, ind), 0, 0, FALSE, TRUE);
    ind += board_img.tileset->numTile; // get offset of tiles
    piecesTileIndex = ind;
    // now load pieces tileset
    VDP_loadTileSet(pieces_img.tileset, piecesTileIndex, CPU);
    setup_pieces();
    draw_pieces();

    //////////////////////////////////////////////////////////////
    // sprites 
    SPR_init();
    CURSOR cursor;
    cursor_init(&cursor, 
            SPR_addSprite( &cursor_spr, cursor.pos_x, cursor.pos_y, TILE_ATTR(PAL2, FALSE, FALSE, FALSE )),
            SPR_addSprite( &cursor_spr, cursor.pos_x, cursor.pos_y, TILE_ATTR(PAL2, FALSE, FALSE, FALSE )) );


    //////////////////////////////////////////////////////////////
    // main loop.
    u8 inputWait = 0;
    u8 currentPlayer = PLAYER_ONE;

    VDP_setTextPlane( BG_A );
    //VDP_drawText("PLAYER ONE MOVE", 13, 1);
    while(TRUE)
    {
        if( currentPlayer == whoAmI  ){
            // read joypad to mover cursor
            if( inputWait == 0 ) {
                u16 joypad  = JOY_readJoypad( JOY_1 );
                // update local position
                if( cursor_move( &cursor, joypad ) == TRUE ) {
                    XGM_startPlayPCM(SND_MOVE,1,SOUND_PCM_CH2);
                    inputWait = INPUT_WAIT_COUNT;
                }
                // if A, 
                if( joypad & BUTTON_A ) {
                    bool trySend =  cursor_action( &cursor, board, currentPlayer );
                    inputWait = INPUT_WAIT_COUNT;
                    if( trySend ) {
                        // send possible move
                        if( send_move( &cursor, 1 ) ) {
                           currentPlayer = currentPlayer == PLAYER_ONE ? PLAYER_TWO : PLAYER_ONE; 
                        }
                    }
                } else if( joypad & BUTTON_C ) {
                    cursor_clear_selected( &cursor );
                    inputWait = INPUT_WAIT_COUNT;
                }
            } else {
                if( inputWait > 0 ) {
                    --inputWait;
                }
            }
        } else { // if( currentPlayer == whoAmI  ){

            // not the player, POLL status
            // current player is not me, waiting for update from chess server
            // check if readable
            read_status();
            //VDP_drawText(response, 0, 5 );
            if ( response[4] == 'T'  ) {
               /*
                  ' 'T'
                  '              111111111122225
                  '    0123456789012345678901234
                  '    ACK TURN w:LAST d7d5:MVNO 2 
                */
                if (response[9] == 'w') {
                   currentPlayer = PLAYER_ONE; 
                    VDP_drawText("ONE", 20, 1);
                }else if (response[9] == 'b') {
                   currentPlayer = PLAYER_TWO; 
                    VDP_drawText("TWO", 20, 1);
                }else{
                   currentPlayer = NO_PLAYER; 
                }

                // parse move
                XGM_startPlayPCM(SND_MOVE,1,SOUND_PCM_CH2);
                // need to convert uci to local 2d array coords
                if( currentPlayer == whoAmI && response[16] != '-' ) {
                    move_piece( (s8)response[16]-FILE_X, RANK_Y +7 - (s8)response[17], (s8)response[18]-FILE_X, RANK_Y + 7 - (s8)response[19], (s8)response[20] );
                }

                cursor_clear_selected( &cursor );

            } else if ( response[4] == 'O'  ) {
               /*
                     ' 'O'
                     '              1111111111222222222233
                     '    01234567890123456789012345678901
                     '    ACK OVER 0-1 1:TURN w:LAST d8h4:MVNO 4
                     '
                     '    1-0, 0-1,  1/2-1/2   (so we can check position 7 for 0,1,2 (who won)
                     '
                     '    position 9 - 1 = checkmate, 2 = stalemate, 3 - insufficient material, 4- seventyfive moves, 5 - repetition, 6 fifty moves, 6 reps, 8 variant wih, 9 variant loss 10 variant draw
               */

                // Game over man.
                if(  response[11] == '0' ) {
                    VDP_drawText("PLAYER ONE WINS", 12, 1);
                }else if(  response[11] == '1' ) {
                    VDP_drawText("PLAYER TWO WINS", 12, 1);
                }else if(  response[11] == '2' ) {
                    VDP_drawText("DRAW           ", 12, 1);
                }
                move_piece( (s8)response[27]-FILE_X, RANK_Y +7 - (s8)response[28], (s8)response[29]-FILE_X, RANK_Y + 7 - (s8)response[30], (s8)response[31] );
            
            }
            if( currentPlayer != whoAmI ) {
                waitMs(2000); // wait a bit if response is not as expected.
            }

        } // if( currentPlayer == whoAmI  ){



        // TODO add options to enter a different server or start a new game.

        //////////////////////////////////////////////////////////////
        // update sprites
        SPR_setPosition( cursor.sprite, cursor.pos_x, cursor.pos_y );
        SPR_setPosition( cursor.selected_spr, cursor.sel_pos_x, cursor.sel_pos_y );
        SPR_update();

        //////////////////////////////////////////////////////////////
        // SGDK Do your thing.
        SYS_doVBlankProcess();
    }
    return 0;

}
