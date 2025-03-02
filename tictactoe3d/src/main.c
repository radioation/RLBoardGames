#include <genesis.h>
#include "resources.h"

#include "network.h"

#include "ip_input.h"

#include "logic.h"


#define INPUT_WAIT_COUNT 10

int cursor_x, cursor_y;
u8 buttons, buttons_prev;


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

////////////////////////////////////////////////////////////////////////////
// Board 
s16 board[BOARD_SIZE][BOARD_SIZE][BOARD_SIZE] = {0}; // 3D board initialized to 0
s16 board_pos_x[BOARD_SIZE][BOARD_SIZE][BOARD_SIZE] = {0};
s16 board_pos_y[BOARD_SIZE][BOARD_SIZE][BOARD_SIZE] = {0};



int tiles_index = 0;
const s16 boardXStart = 12;
const s16 boardYStart = 3;
const s16 layerStep = 5;


// Sprite data structures
typedef struct
{
    Sprite *p1_sprite;
    Sprite *p2_sprite;
    s16 col;     // col
    s16 row;     // row
    s16 layer;   // Z

    s16 pos_x;  // actual X position
    s16 pos_y;  // actual Y position
   
  
} CURSOR;

const s8 cursorStep = 24;
const s8 cursorColStart = 64;
const s8 cursorRowStart = 16;




void cursor_init( CURSOR *cursor, Sprite *p1, Sprite *p2 ) {
    cursor->p1_sprite = p1;
    cursor->p2_sprite = p2;
    cursor->col = 1;  // board position
    cursor->row = 1;   
    cursor->layer = 1;   


    cursor->pos_x = board_pos_x[ cursor->col ][ cursor->row][ cursor->layer ];
    cursor->pos_y = board_pos_y[ cursor->col ][ cursor->row][ cursor->layer ];

    SPR_setVisibility( cursor->p1_sprite, VISIBLE );
    SPR_setVisibility( cursor->p2_sprite, HIDDEN );
}




void draw_row( s16 startX, s16 startY, bool bottom ){
    
    VDP_setTileMapXY( BG_B, TILE_ATTR_FULL( PAL0, TRUE, FALSE, FALSE, tiles_index+1 ), startX, startY );
    VDP_setTileMapXY( BG_B, TILE_ATTR_FULL( PAL0, TRUE, FALSE, FALSE, tiles_index+2 ), startX+1, startY );
    VDP_setTileMapXY( BG_B, TILE_ATTR_FULL( PAL0, TRUE, FALSE, FALSE, tiles_index+3 ), startX+2, startY );
    VDP_setTileMapXY( BG_B, TILE_ATTR_FULL( PAL0, TRUE, FALSE, FALSE, tiles_index+2 ), startX+3, startY );
    VDP_setTileMapXY( BG_B, TILE_ATTR_FULL( PAL0, TRUE, FALSE, FALSE, tiles_index+3 ), startX+4, startY );
    VDP_setTileMapXY( BG_B, TILE_ATTR_FULL( PAL0, TRUE, FALSE, FALSE, tiles_index+2 ), startX+5, startY );
    VDP_setTileMapXY( BG_B, TILE_ATTR_FULL( PAL0, TRUE, FALSE, FALSE, tiles_index+3 ), startX+6, startY );
    VDP_setTileMapXY( BG_B, TILE_ATTR_FULL( PAL0, TRUE, FALSE, FALSE, tiles_index+2 ), startX+7, startY );
    VDP_setTileMapXY( BG_B, TILE_ATTR_FULL( PAL0, TRUE, FALSE, FALSE, tiles_index+4 ), startX+8, startY );

    if( bottom ) {
        VDP_setTileMapXY( BG_B, TILE_ATTR_FULL( PAL0, TRUE, FALSE, FALSE, tiles_index ), startX, startY +1 );
        VDP_setTileMapXY( BG_B, TILE_ATTR_FULL( PAL0, TRUE, FALSE, FALSE, tiles_index ), startX+1, startY+1 );
        VDP_setTileMapXY( BG_B, TILE_ATTR_FULL( PAL0, TRUE, FALSE, FALSE, tiles_index ), startX+2, startY+1 );
        VDP_setTileMapXY( BG_B, TILE_ATTR_FULL( PAL0, TRUE, FALSE, FALSE, tiles_index ), startX+3, startY+1 );
        VDP_setTileMapXY( BG_B, TILE_ATTR_FULL( PAL0, TRUE, FALSE, FALSE, tiles_index ), startX+4, startY+1 );
        VDP_setTileMapXY( BG_B, TILE_ATTR_FULL( PAL0, TRUE, FALSE, FALSE, tiles_index ), startX+5, startY+1 );
        VDP_setTileMapXY( BG_B, TILE_ATTR_FULL( PAL0, TRUE, FALSE, FALSE, tiles_index ), startX+6, startY+1 );
        VDP_setTileMapXY( BG_B, TILE_ATTR_FULL( PAL0, TRUE, FALSE, FALSE, tiles_index ), startX+7, startY+1 );
    }
}

void draw_boards() { 
    // 4 in all
    s16 x = boardXStart;
    s16 y = boardYStart; 
    for( s16 layer = 0; layer < 4; ++layer ) {
        // draw rows
        draw_row( x + 5, y + 1, false );
        draw_row( x + 4, y + 2, false );
        draw_row( x + 3, y + 3, false );
        draw_row( x + 2, y + 4, true );


        // go to next 
        y += layerStep;
    }

 
}

void init_board_pos () {
    for( s16 z=0; z < BOARD_SIZE; z++ ) {
        s16 currY = boardYStart +1+ z*layerStep;
        for( s16 y=0; y < BOARD_SIZE; y++ ) {
            s16 currX = boardXStart + 5 - y;
            for( s16 x=0; x < BOARD_SIZE; x++ ) {
                board_pos_x[x][y][z] = currX * 8;
                board_pos_y[x][y][z] = currY * 8;
                currX += 2;
            }
            currY++;
        }
    }
}

void add_move() {
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
        // 
        cursor->pos_x = board_pos_x[ cursor->col ][ cursor->row][ cursor->layer ];
        cursor->pos_y = board_pos_y[ cursor->col ][ cursor->row][ cursor->layer ];
    }
    return didMove;
}


bool cursor_action( CURSOR* cursor, s16 brd[BOARD_SIZE][BOARD_SIZE][BOARD_SIZE], u8 player ) {
    return false;
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
    cursor_y = 5;
    // blocks whilewaiting for network to be ready.
    char fullserver[21];
    memset(fullserver,0, sizeof(fullserver));
    sprintf( fullserver, "%s:5364", server);
    return NET_connect(cursor_x, cursor_y, fullserver); cursor_x=0; cursor_y++;
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
            cursor_y = 5;
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
}





int main()
{

    //////////////////////////////////////////////////////////////
    // setup screen and palettes
    SYS_disableInts();
    VDP_setScreenWidth320();
    VDP_setScreenHeight224();
    VDP_setBackgroundColor(16);
    VDP_setTextPlane(BG_A);
    SYS_enableInts();                      // enable interrupts for networking print routines.


    //////////////////////////////////////////////////////////////
    // Networking setup
    cursor_x = 0;
    cursor_y = 0;

    // Establish Comms (find/talk to other console)
    VDP_drawText("Detecting adapter...[  ]", cursor_x, cursor_y); cursor_x+=21;
    NET_initialize(); // Detect cartridge and set boolean variable

    if(cart_present)
    {
        VDP_setTextPalette(12);
        VDP_drawText("Ok", cursor_x, cursor_y); cursor_x=0; cursor_y+=2;
        VDP_setTextPalette(15); 
    }
    else
    {
        VDP_setTextPalette(11); 
        VDP_drawText("XX", cursor_x, cursor_y); cursor_x=0; cursor_y+=2;
        VDP_setTextPalette(15); 
        VDP_drawText("Adapter not present", cursor_x, cursor_y);
        while(1) { SYS_doVBlankProcess(); }
    }


    VDP_drawText("IP Address:", cursor_x, cursor_y);
    NET_printIP(cursor_x+12, cursor_y); cursor_y++;

    VDP_drawText("MAC:", cursor_x, cursor_y);
    NET_printMAC(cursor_x+5, cursor_y); cursor_y+=2;


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

    SPR_init();

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

    setWhoAmI();


    //////////////////////////////////////////////////////////////
    // Background setup
    PAL_setPalette( PAL0, tictactoe_pal.data, CPU );
    tiles_index = TILE_USER_INDEX;
    VDP_loadTileSet( &tictactoe_tiles, tiles_index, CPU);

    draw_boards();
    init_board_pos();

    //////////////////////////////////////////////////////////////
    // Sprite setup
    SPR_init();
    CURSOR cursor;
    cursor_init(&cursor,
            SPR_addSprite( &blue_cursor, cursor.pos_x, cursor.pos_y, TILE_ATTR(PAL0, TRUE, FALSE, FALSE )),
            SPR_addSprite( &yellow_cursor, cursor.pos_x, cursor.pos_y, TILE_ATTR(PAL0, TRUE, FALSE, FALSE )) );

    //////////////////////////////////////////////////////////////
    // MAIN LOOP
    u8 inputWait = 0;
    u8 me = PLAYER_ONE; 
    while(1) // Loop forever
    {
        // read joypad to mover cursor
        u16 joypad  = JOY_readJoypad( JOY_1 );
        if( inputWait == 0 ) {
            if( cursor_move( &cursor, joypad ) == TRUE ) {
                inputWait = INPUT_WAIT_COUNT;
                // send cursor data
                //cursor_send_data( &cursor, 0 );
            }
            if( joypad & BUTTON_A ) {
                bool didMove =  cursor_action( &cursor, board, me );
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
        //SPR_setPosition( cursor.selected_spr, cursor.sel_pos_x, cursor.sel_pos_y );
        SPR_update();

        //////////////////////////////////////////////////////////////
        // SGDK Do your thing.

        SYS_doVBlankProcess();
    }

    //------------------------------------------------------------------
    return(0);

}
