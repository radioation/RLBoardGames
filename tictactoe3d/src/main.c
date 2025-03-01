#include <genesis.h>
#include "resources.h"

//#include "network.h"
//#include "ip_input.h"

#include "logic.h"


#define INPUT_WAIT_COUNT 10

int text_cursor_x, text_cursor_y;


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
    int tiles_index = TILE_USER_INDEX;
    VDP_loadTileSet( &tictactoe_tiles, tiles_index, CPU);


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
