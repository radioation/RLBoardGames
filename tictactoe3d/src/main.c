#include <genesis.h>
#include "resources.h"
#include "network.h"
#include "ip_input.h"

#define INPUT_WAIT_COUNT 10

int text_cursor_x, text_cursor_y;



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
    // MAIN LOOP
    net_buffer_init();

    while(1) // Loop forever
    {
        SYS_doVBlankProcess();
    }

    //------------------------------------------------------------------
    return(0);

}
