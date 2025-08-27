#include <genesis.h>
#include "resources.h"

#include "network.h"

#include "ip_input.h"

#include "logic.h"


#define INPUT_WAIT_COUNT 10

#define SND_MOVE 63
#define SND_PLACE 64

int text_cursor_x, text_cursor_y;
u8 buttons, buttons_prev;
bool game_won = false;
u8 current_player;

////////////////////////////////////////////////////////////////////////////
// **Identification**
// need to have an ID for each console that is unique
#define IM_NOBODY 0
#define IM_HOST 1
#define IM_CLIENT 2
u8 whoAmI = IM_NOBODY;


////////////////////////////////////////////////////////////////////////////
// network stuff
char server[16] = "000.000.000.000";
bool online = false;

////////////////////////////////////////////////////////////////////////////
// Board 
s16 board[BOARD_SIZE][BOARD_SIZE][BOARD_SIZE] = {0}; // 3D board initialized to 0
s16 board_pos_x_to_pixel_x[BOARD_SIZE][BOARD_SIZE][BOARD_SIZE] = {0};
s16 board_pos_y_to_pixel_y[BOARD_SIZE][BOARD_SIZE][BOARD_SIZE] = {0};

s16 board_pos_x_to_tile_x[BOARD_SIZE][BOARD_SIZE][BOARD_SIZE] = {0};
s16 board_pos_y_to_tile_y[BOARD_SIZE][BOARD_SIZE][BOARD_SIZE] = {0};


int x_o_tiles_index = 0;
int board_index = 0;
const s16 boardXStart = 12;
const s16 boardYStart = 3;
const s16 layerStep = 5;

void clear_board() {
    memset(board,0, sizeof(board));
    VDP_clearPlane(BG_A, true ); 
}

void clear_top_text() {
    VDP_clearTextArea( 0, 0, 40, 4 ); 
}

void init_board_pos_lookup () {
    for( s16 z=0; z < BOARD_SIZE; z++ ) {
        s16 currY = boardYStart +1+ z*layerStep;
        for( s16 y=0; y < BOARD_SIZE; y++ ) {
            s16 currX = boardXStart + 5 - y;
            for( s16 x=0; x < BOARD_SIZE; x++ ) {
                board_pos_x_to_pixel_x[x][y][z] = currX * 8;
                board_pos_y_to_pixel_y[x][y][z] = currY * 8;
                board_pos_x_to_tile_x[x][y][z] = currX;
                board_pos_y_to_tile_y[x][y][z] = currY;
                currX += 2;
            }
            currY++;
        }
    }
}


// Sprite data structures
typedef struct
{
    Sprite *p1_sprite;
    Sprite *p2_sprite;
    s8 col;     // col
    s8 row;     // row
    s8 layer;   // Z

    s16 pos_x;  // actual X position
    s16 pos_y;  // actual Y position
    // no corresponding Z, it's fictional

} CURSOR;

const s8 cursorStep = 24;
const s8 cursorColStart = 64;
const s8 cursorRowStart = 16;




void cursor_init( CURSOR *cursor, Sprite *p1, Sprite *p2 ) {
    cursor->p1_sprite = p1;
    cursor->p2_sprite = p2;
    // board position ( 2nd col, 2nd row, of 2nd layer from top
    cursor->col = 1;  
    cursor->row = 1;   
    cursor->layer = 1;   


    cursor->pos_x = board_pos_x_to_pixel_x[ (u8)cursor->col ][ (u8)cursor->row][(u8) cursor->layer ];
    cursor->pos_y = board_pos_y_to_pixel_y[ (u8)cursor->col ][ (u8)cursor->row][(u8) cursor->layer ];

    SPR_setVisibility( cursor->p1_sprite, VISIBLE );
    SPR_setVisibility( cursor->p2_sprite, HIDDEN );
}




bool cursor_move( CURSOR *cursor, u16 joypad ) {
    bool didMove = FALSE;
    if( joypad & BUTTON_LEFT ) {
        cursor->col--;
        didMove = TRUE;
    }
    if( joypad & BUTTON_RIGHT ) {
        cursor->col++;
        didMove = TRUE;
    }


    if( joypad & BUTTON_UP ) {
        cursor->row--;
        didMove = TRUE;
    }
    if( joypad & BUTTON_DOWN ) {
        cursor->row++;
        didMove = TRUE;
    }

    if( joypad & BUTTON_B ) {
        cursor->layer--;
        didMove = TRUE;
    }
    if( joypad & BUTTON_C ) {
        cursor->layer++;
        didMove = TRUE;
    }

    if( didMove ) {
        if( cursor->col < 0 ) {
            cursor->col = BOARD_SIZE - 1;
        } else if ( cursor->col > BOARD_SIZE - 1 ) {
            cursor->col = 0;
        }
        if( cursor->row < 0 ) {
            cursor->row= BOARD_SIZE - 1;
        } else if ( cursor->row > BOARD_SIZE - 1 ) {
            cursor->row = 0;
        }
        if( cursor->layer < 0 ) {
            cursor->layer= BOARD_SIZE - 1;
        } else if ( cursor->layer > BOARD_SIZE - 1 ) {
            cursor->layer = 0;
        }
        // update pos
        cursor->pos_x = board_pos_x_to_pixel_x[ (u8)cursor->col ][ (u8)cursor->row][ (u8)cursor->layer ];
        cursor->pos_y = board_pos_y_to_pixel_y[ (u8)cursor->col ][ (u8)cursor->row][ (u8)cursor->layer ];
    }
    return didMove;
}


void cursor_update_from_pos( CURSOR *cursor, s8 col, s8 row, s8 layer ) {
    cursor->col = col;
    cursor->row = row;
    cursor->layer = layer;

    cursor->pos_x = board_pos_x_to_pixel_x[ (u8)cursor->col ][ (u8)cursor->row][ (u8)cursor->layer ];
    cursor->pos_y = board_pos_y_to_pixel_y[ (u8)cursor->col ][ (u8)cursor->row][ (u8)cursor->layer ];

    SPR_setPosition( cursor->p1_sprite, cursor->pos_x, cursor->pos_y );
    SPR_update();

}

void update_board( u8 col, u8 row, u8 layer, u8 player ) {
    // get row and Layer offset.
    board[col][row][layer] = player;
    // empty space, fill it
    u16 startX = board_pos_x_to_tile_x[col][row][layer];
    u16 startY = board_pos_y_to_tile_y[col][row][layer];
    if ( player == PLAYER_ONE ) {
        // always place center piece.
        VDP_setTileMapXY( BG_A, TILE_ATTR_FULL( PAL0, TRUE, FALSE, FALSE, x_o_tiles_index+2 ), startX+1, startY );
        // check col 
        // * if col is 0, only need to draw tiles for 0 and 1, 
        u16 leftOffset = 1;
        u16 rightOffset = 3;
        // * if col 1 or 2 draw neighbor tiles as well
        // * if col 3, just draw tiles  for 2 and 3
        if ( col < 3  ) {
            if ( board[col+1][row][layer] == PLAYER_ONE )  {
                rightOffset = 0;
            } else if ( board[col+1][row][layer] == PLAYER_TWO )  {
                rightOffset = 4;
            }
        } 
        if ( col > 0 ) {
            if ( board[col-1][row][layer] == PLAYER_ONE )  {
                leftOffset = 0;
            }
        }
        VDP_setTileMapXY( BG_A, TILE_ATTR_FULL( PAL0, TRUE, FALSE, FALSE, x_o_tiles_index+leftOffset ), startX, startY );
        VDP_setTileMapXY( BG_A, TILE_ATTR_FULL( PAL0, TRUE, FALSE, FALSE, x_o_tiles_index+rightOffset ), startX+2, startY );
    }else if ( player == PLAYER_TWO ) {
        // always place center piece.
        VDP_setTileMapXY( BG_A, TILE_ATTR_FULL( PAL0, TRUE, FALSE, FALSE, x_o_tiles_index+6 ), startX+1, startY );
        // check col 
        // * if col is 0, only need to draw tiles for 0 and 1, 
        u16 leftOffset = 5;
        // * if col 1 or 2 draw neighbor tiles as well
        // * if col 3, just draw tiles  for 2 and 3
        if ( col > 0 ) {
            if ( board[col-1][row][layer] == PLAYER_ONE )  {
                leftOffset = 4;
            }
        }
        VDP_setTileMapXY( BG_A, TILE_ATTR_FULL( PAL0, TRUE, FALSE, FALSE, x_o_tiles_index+leftOffset ), startX, startY );
        //VDP_setTileMapXY( BG_A, TILE_ATTR_FULL( PAL0, TRUE, FALSE, FALSE, x_o_tiles_index ), startX+2, startY );
    }

}

bool cursor_action( CURSOR* cursor, s16 brd[BOARD_SIZE][BOARD_SIZE][BOARD_SIZE], u8 player ) {
    u16 col = cursor->col;
    u16 row = cursor->row;
    u16 layer = cursor->layer;
    if( board[col][row][layer] == NO_PLAYER ) {
        update_board( col, row, layer, player );
        return true;
    }
    return false;
}


void cursor_send_data( CURSOR* cursor, u8 type  ) {
    NET_sendByte( 128 + type ); // first bit is always on, and 4 bytesl
    NET_sendByte( 3 ); // sending two more bytes.
    NET_sendByte( cursor->col );
    NET_sendByte( cursor->row );
    NET_sendByte( cursor->layer );
}



void host_game() {
    // Allow client to join
    NET_allowConnections();
    VDP_drawText("   Wait for client:      ", 0, 5);

    // loop while waiting for a peer.
    u8 offset = 0;
    while( 1 ) {
        // Data available?
        while( !NET_RXReady() ){
            // writing some sort of text here?
            VDP_drawText(" .     ", 21+offset, 5);
            offset +=1;
            if( offset > 3 ) offset = 0;
            waitMs(16);

        }
        // look for 'C'
        u8 ret = NET_readByte();
        if( ret == 'C' ) {
            // clear text before losing out
            VDP_drawText("   Client Connected!     ", 0, 5);
            return;
        }
    }

}


bool join_game() {
    VDP_drawText("   Connect to server    ", 0, 5);
    text_cursor_y = 5;
    // blocks whilewaiting for network to be ready.
    char fullserver[21];
    memset(fullserver,0, sizeof(fullserver));
    sprintf( fullserver, "%s:5364", server);
    return NET_connect(text_cursor_x, text_cursor_y, fullserver); text_cursor_x=0; text_cursor_y++;
}


void setWhoAmI() {
    buttons = 0;
    buttons_prev = 0;
    VDP_clearTextArea( 0, 0,  40, 13  );
    VDP_drawText("          (A) - Host Game", 0, 5);
    VDP_drawText("          (C) - Join Game", 0, 7);
    NET_resetAdapter();
    while(1) // loop forever
    {
        buttons = JOY_readJoypad(JOY_1);
        // MODE NOT SET, button press will determine server or client.
        if(buttons & BUTTON_A && buttons_prev == 0x00) {
            VDP_clearTextArea( 0, 5,  40, 3 );
            text_cursor_y = 5;
            whoAmI = IM_HOST;
            // start listening
            host_game();
            break;
        }else if(buttons & BUTTON_C && buttons_prev == 0x00) {
            VDP_clearTextArea( 0, 5,  40, 3 );
            // try to connect to server.
            whoAmI = IM_CLIENT;
            join_game();
            break;

        }
        buttons_prev = buttons;
        SYS_doVBlankProcess();
    }
    NET_flushBuffers();

}

void reset_game() {
    current_player = PLAYER_ONE; 
    char message[40];
    memset(message,0, sizeof(message));
    game_won = false;
    clear_board();
    clear_top_text();
    VDP_setTextPalette(0); 
    if( !online ) { 
        VDP_drawText("Local Play", 15, 0);
    }
    sprintf( message, "Player %d turn    ", current_player);
    VDP_setTextPalette(current_player); 
    VDP_drawText(message, 13, 1 );
}



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



int main()
{

    //////////////////////////////////////////////////////////////
    // Setup Sound
    XGM_setPCM(  SND_MOVE, move_snd, sizeof(move_snd));
    XGM_setPCM(  SND_PLACE, place_snd, sizeof(place_snd));


    //////////////////////////////////////////////////////////////
    // setup screen and palettes
    SYS_disableInts();
    VDP_setScreenWidth320();
    VDP_setScreenHeight224();
    VDP_setBackgroundColor(16);
    VDP_setTextPlane(BG_A);
    SYS_enableInts();                      // enable interrupts for networking print routines.

    PAL_setPalette( PAL0, tictactoe_pal.data, CPU );

    //////////////////////////////////////////////////////////////
    // Networking setup
    text_cursor_x = 0; // networking text cursor location
    text_cursor_y = 0;
    SPR_init();

    // Establish Comms (find/talk to other console)
    VDP_drawText("Detecting adapter...[  ]", text_cursor_x, text_cursor_y); text_cursor_x+=21;
    NET_initialize(); // Detect cartridge and set 'cart_present' variable

    if(cart_present)
    {
        VDP_setTextPalette(0);
        VDP_drawText("Ok", text_cursor_x, text_cursor_y); text_cursor_x=0; text_cursor_y+=2;
        VDP_setTextPalette(3); 



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

        VDP_drawText( "Got Address", 13 ,12 );
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
        if( whoAmI == 1 ) {
            while(1) { 
                u16 joypad  = JOY_readJoypad( JOY_1 );
                SYS_doVBlankProcess();
                if( joypad & BUTTON_START ) {
                    break;
                }
            }
        }
        online = true;
    }
    else
    {
        online = false;
        VDP_setTextPalette(0); 
        VDP_drawText("XX", text_cursor_x, text_cursor_y); text_cursor_x=0; text_cursor_y+=2;
        VDP_setTextPalette(1); 
        VDP_drawText("Adapter not present", text_cursor_x, text_cursor_y); text_cursor_y++;
        VDP_setTextPalette(0);
        VDP_drawText("Press START to continue", text_cursor_x, text_cursor_y);
        while(1) { 
            u16 joypad  = JOY_readJoypad( JOY_1 );
            SYS_doVBlankProcess();
            if( joypad & BUTTON_START ) {
                break;
            }
        }
    }


    //////////////////////////////////////////////////////////////
    // Background setup
    x_o_tiles_index = TILE_USER_INDEX;
    VDP_loadTileSet( &tictactoe_x_o_tiles, x_o_tiles_index, CPU);
    board_index = x_o_tiles_index + tictactoe_x_o_tiles.numTile;
    VDP_loadTileSet(tictactoe_board.tileset, board_index, CPU);


    VDP_drawImageEx(BG_B, &tictactoe_board, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, board_index), 0, 0, FALSE, TRUE);

    init_board_pos_lookup();

    //////////////////////////////////////////////////////////////
    // Sprite setup
    SPR_init();
    CURSOR cursor;
    cursor_init(&cursor,
            SPR_addSprite( &blue_cursor, -32, -32, TILE_ATTR(PAL0, FALSE, FALSE, FALSE )),
            SPR_addSprite( &yellow_cursor, -32, 32, TILE_ATTR(PAL0, FALSE, FALSE, FALSE )) );

    //////////////////////////////////////////////////////////////
    // MAIN LOOP
    u8 inputWait = 0;
    current_player = PLAYER_ONE; 

    char message[40];
    reset_game();

    while(1) // Loop forever
    {
        // read joypad to move cursor
        if( online ) {
            if( current_player == whoAmI ) {
                if( ! game_won ) {
                    u16 joypad  = JOY_readJoypad( JOY_1 );
                    if( inputWait == 0 ) {
                        if( cursor_move( &cursor, joypad ) == TRUE ) {
                            XGM_startPlayPCM(SND_MOVE,1,SOUND_PCM_CH2);
                            inputWait = INPUT_WAIT_COUNT;
                            cursor_send_data( &cursor, 0 );
                        }
                        if( joypad & BUTTON_A ) {
                            bool didMove = cursor_action( &cursor, board, current_player );
                            if ( didMove ) {
                                XGM_startPlayPCM(SND_PLACE,1,SOUND_PCM_CH2);

                                if( check_win( board, current_player ) ) {
                                    cursor_send_data( &cursor, 2 );
                                    // TODO: figure out how to make horn/trumpet music
                                    sprintf( message, "PLAYER %d WINS    ", current_player);
                                    VDP_drawText(message, 13, 1 );
                                    game_won = true;
                                } else {
                                    cursor_send_data( &cursor, 1 );
                                    current_player = current_player == PLAYER_TWO ? PLAYER_ONE : PLAYER_TWO;
                                    VDP_setTextPalette(current_player); 
                                    sprintf( message, "Player %d turn    ", current_player);
                                    VDP_drawText(message, 13, 1 );
                                }
                            } else {
                                cursor_send_data( &cursor, 0 );
                            }
                            inputWait = INPUT_WAIT_COUNT;
                        } 
                    } else {
                        if( inputWait > 0 ) {
                            --inputWait;
                        }
                    }


                    //////////////////////////////////////////////////////////////
                    // update sprites
                    SPR_setPosition( cursor.p1_sprite, cursor.pos_x, cursor.pos_y );
                    SPR_update();

                }
            } else {
                // current player is NOT me, listen for data
                if( NET_RXReady() ) {
                    // read the hea    der
                    u8 header[2];
                    read_bytes_n( header, 2 );
                    u8 data_type = header[0];
                    u8 data_length = header[1];
                    // read the data
                    u8 buffer[16];
                    read_bytes_n( buffer, data_length );
                    if( data_type == 128 ) {
                        XGM_startPlayPCM(SND_MOVE,1,SOUND_PCM_CH2);
                        // just moving the cursor around
                        //VDP_drawText("128 ", 2, 6 );
                        // cursor update
                        cursor_update_from_pos( &cursor, (s8)buffer[0], (s8)buffer[1], (s8)buffer[2] );
                    }else if( data_type == 129 || data_type == 130 ) {
                        XGM_startPlayPCM(SND_PLACE,1,SOUND_PCM_CH2);
                        // 129 means button was pressed.
                        //VDP_drawText("129 ", 2, 7 );
                        //
                        cursor_update_from_pos( &cursor, (s8)buffer[0], (s8)buffer[1], (s8)buffer[2] );

                        // board update
                        update_board( (s8)buffer[0], (s8)buffer[1], (s8)buffer[2], current_player );
                        if( data_type == 130 ) {
                            sprintf( message, "PLAYER %d WINS    ", current_player);
                            VDP_drawText(message, 13, 1 );
                            game_won = true;
                        } else {
                            if( current_player == PLAYER_ONE ) {
                                current_player = PLAYER_TWO;
                                VDP_drawText("TWO", 20, 0);
                            } else {
                                current_player = PLAYER_ONE;
                                VDP_drawText("ONE", 20, 0);
                            }
                        }
                    }
                }
            }

        } else { //if( online ) {
            if( ! game_won ) {
                u16 joypad  = JOY_readJoypad( JOY_1 );
                if( inputWait == 0 ) {
                    if( cursor_move( &cursor, joypad ) == TRUE ) {
                        XGM_startPlayPCM(SND_MOVE,1,SOUND_PCM_CH2);
                        inputWait = INPUT_WAIT_COUNT;
                    }
                    if( joypad & BUTTON_A ) {
                        bool didMove = cursor_action( &cursor, board, current_player );
                        if ( didMove ) {
                            XGM_startPlayPCM(SND_PLACE,1,SOUND_PCM_CH2);
                            if( check_win( board, current_player ) ) {
                                sprintf( message, "PLAYER %d WINS    ", current_player);
                                VDP_drawText(message, 13, 1 );
                                game_won = true;
                            } else {
                                current_player = current_player == PLAYER_TWO ? PLAYER_ONE : PLAYER_TWO;
                                VDP_setTextPalette(current_player); 
                                sprintf( message, "Player %d turn    ", current_player);
                                VDP_drawText(message, 13, 1 );
                            }
                        }
                        inputWait = INPUT_WAIT_COUNT;
                    } 
                } else {
                    if( inputWait > 0 ) {
                        --inputWait;
                    }
                }


                //////////////////////////////////////////////////////////////
                // update sprites
                SPR_setPosition( cursor.p1_sprite, cursor.pos_x, cursor.pos_y );
                SPR_update();
            } else {
                u16 joypad  = JOY_readJoypad( JOY_1 );
                SYS_doVBlankProcess();
                if( joypad & BUTTON_START ) {
                    reset_game();
                }
            }


        } //if( online ) {



        //////////////////////////////////////////////////////////////
        // SGDK Do your thing.
        SYS_doVBlankProcess();
        }

        //------------------------------------------------------------------
        return(0);

        }
