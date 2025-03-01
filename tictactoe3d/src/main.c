#include <genesis.h>
#include "resources.h"

//#include "network.h"
//#include "ip_input.h"

#include "logic.h"


#define INPUT_WAIT_COUNT 10

int text_cursor_x, text_cursor_y;


#define SIZE 4

int board[SIZE][SIZE][SIZE] = {0}; // 3D board initialized to 0
int tiles_index = 0;
const s16 boardXStart = 12;
const s16 boardYStart = 3;


// Sprite data structures
typedef struct
{
    Sprite *p1_sprite;
    Sprite *p2_sprite;
    s8 col;     // col
    s8 row;     // row
    s8 board;   // board
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
    cursor->board = 1;   

    cursor->pos_x = cursor->col * cursorStep + cursorColStart; 
    cursor->pos_y = cursor->row * cursorStep + cursorRowStart;  

    SPR_setAnim( cursor->p1_sprite, 1 );
    SPR_setVisibility( cursor->p2_sprite, HIDDEN );
}


bool cursor_move( CURSOR *cursor, u16 joypad ) {
    bool didMove = FALSE;                                                   
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
    for( s16 board = 0; board < 4; ++board ) {
        draw_row( x + 5, y + 1, false );
        draw_row( x + 4, y + 2, false );
        draw_row( x + 3, y + 3, false );
        draw_row( x + 2, y + 4, true );
        y += 5;
    }
}


void add_move() {
}



int main()
{

    //////////////////////////////////////////////////////////////
    // setup screen and palettes
    SYS_disableInts();
    VDP_setScreenWidth320();
    VDP_setScreenHeight224();
    VDP_setBackgroundColor(16);
    SYS_enableInts();                      // enable interrupts for networking print routines.


    //////////////////////////////////////////////////////////////
    // Networking setup


    //////////////////////////////////////////////////////////////
    // Background setup
    PAL_setPalette( PAL0, tictactoe_pal.data, CPU );
    tiles_index = TILE_USER_INDEX;
    VDP_loadTileSet( &tictactoe_tiles, tiles_index, CPU);

    draw_boards();
    //////////////////////////////////////////////////////////////
    // Sprite setup
    SPR_init();
    CURSOR cursor;
    cursor_init(&cursor,
            SPR_addSprite( &blue_cursor, cursor.pos_x, cursor.pos_y, TILE_ATTR(PAL0, FALSE, FALSE, FALSE )),
            SPR_addSprite( &yellow_cursor, cursor.pos_x, cursor.pos_y, TILE_ATTR(PAL0, FALSE, FALSE, FALSE )) );

    //////////////////////////////////////////////////////////////
    // MAIN LOOP
    while(1) // Loop forever
    {
        SYS_doVBlankProcess();
    }

    //------------------------------------------------------------------
    return(0);

}
