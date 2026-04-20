#include <genesis.h>
#include "Network.h"

int cursor_x, cursor_y;
u8 buttons, buttons_prev;
extern Buffer RxBuffer;

// network stuff
char server[16] = "127.0.0.1";
char request[64];
unsigned char response[128];


s16 read_n_bytes(u8* data, u8 data_len ){
    s16 bytePos = 0;
    while( bytePos < data_len ) {
        // read data
        int bw = Buffer_GetNum( &RxBuffer); // bytes waiting
        if ( bw > 0 ) {
            if ( bw > ( data_len - bytePos ) ) {
                bw = data_len - bytePos;
            }
            Buffer_PeekLast( &RxBuffer, bw, data );   
            Buffer_Flush0( &RxBuffer );
            bytePos += bw; 
        } else {
            waitMs(5);
        }
    }
    return bytePos;
}

s16 read_line(u8* data, u8 data_len ){
    s16 bytePos = 0;
    while( bytePos < data_len ) {
        // read data
        int bw = Buffer_GetNum( &RxBuffer); // bytes waiting
        if ( bw > 0 ) {
            if ( bw > ( data_len - bytePos ) ) {
                bw = data_len - bytePos;
            }
            Buffer_PeekLast( &RxBuffer, bw, data );   
            Buffer_Flush0( &RxBuffer ); // TODO: Don't flush everything automatically. \n might not be at the end of the bytes waiting in a real world server.
            for( s16 i=0; i < bw; ++i ) {
                if( data[bytePos] == 0x0A ) {
                    data[bytePos] = 0;
                    return bytePos;
                }
                bytePos++;
            }
        } else {
            waitMs(5);
        }
    }
    return bytePos;
}


int main()
{
    char msg[40];
    char data[2048];
    cursor_x = 0;
    cursor_y = 0;
    VDP_setBackgroundColor( 0 );
    PAL_setColor(0, RGB24_TO_VDPCOLOR(0x0000FF));
    VDP_drawText("started", cursor_x, cursor_y); cursor_y++;
    SYS_doVBlankProcess(); 
    waitMs(1000);

    ////////////////////////////////////////////////////////////////y
    // Initialize

    // disable interrupts during setup
    //VDP_setEnable(FALSE);
    SYS_disableInts();

    // init rx/tx count
    RXBytes = 0;
    TXBytes = 0;

    //VDP_setEnable(TRUE);

    JOY_setSupport(PORT_2, JOY_SUPPORT_OFF); // PORT 2 is the serial device 

    SYS_setExtIntCallback(NET_RxIRQ);   // Set external IRQ callback`

    // setup network
    VDP_setReg(0xB, 0x8);   
    SYS_enableInts();
    SYS_setInterruptMaskLevel(0);       // Enable all interrupts
    waitMs(1000);

    // -- UART setup -----------------------------------
    DRV_UART.Id.sName = "UART";
    DRV_UART.Id.Bitmask = 0x40; // 0x40 - Pin 7
    DRV_UART.Id.Bitshift = 0;
    DRV_UART.Id.Mode = DEVMODE_SERIAL | DEVMODE_PARALLEL;


    SetDevicePort(&DRV_UART, sv_ListenPort);
    *((vu8*) DRV_UART.SCtrl) = 0x38;
    waitMs(1000);

    u8 xpn_r = XPN_Initialize(); // Check if xPort device is present
                                 //    RLN_Initialize();
    if (xpn_r )
    {
        VDP_drawText("xPort init", cursor_x, cursor_y); cursor_y++;
        DRV_UART.Id.sName = "xPort UART";

        DEV_SetCtrl(DRV_UART, 0x40);
        DEV_ClrData(DRV_UART);

        //bXPNetwork = TRUE; maybe later. just get stuff working for now

        NET_SetConnectFunc(XPN_Connect);
        NET_SetDisconnectFunc(XPN_Disconnect);
        NET_SetGetIPFunc(XPN_GetIP);
        NET_SetPingFunc(XPN_PingIP); 

        switch (xpn_r)
        {
            case 1:
                //Stdout_Push(" \e[92mXPN: xPort module OK\e[0m\n");
                VDP_drawText("xPort module OK", cursor_x, cursor_y); cursor_y++;
                break;
            case 2:
                //Stdout_Push(" \e[91mXPN: Error\e[0m\n");
                VDP_drawText("xPort module Error", cursor_x, cursor_y); cursor_y++;
                break;

            default:
                break;
        }

        // get IP
        char ip[16];
        memset( msg, 0, sizeof(msg) );
        memset( ip, 0, sizeof(ip) );
        //XPN_GetIP( ip ); 
        NET_GetIP( ip ); 
        sprintf(msg, "IP: %s", ip); 
        VDP_drawText(msg, cursor_x, cursor_y); cursor_y++;



        // Setup IP and port for server.
        char fullserver[21];
        memset(fullserver,0, sizeof(fullserver));
        sprintf( fullserver, "%s:5364", server);


        //NET_connect(cursor_x, cursor_y, fullserver); cursor_x=0; cursor_y++;
        if( NET_Connect( fullserver ) ) {
            //s16 count = read_n_bytes( response, 4 );
            //response[4] = 0;
            s16 count = read_line( response, sizeof(response) );
            if( strcmp( (char*)response, "HELO" ) != 0 ) {
                // TODO: we need to handle this error somehow. think about it
                VDP_drawText("NOT HELO?", 0, 2 );

            } else {

                // request to join
                memset( msg, 0, sizeof(msg ));
                sprintf( msg, "J" );
                NET_SendStringLen( msg, 1 );
                waitMs(500); // slower than cart

                memset(response, 0, sizeof(response ));
                count = read_n_bytes( response, 1 );
                u8 player_num = response[0];
                sprintf(msg, "player number: %d", player_num); 
                VDP_drawText(msg, cursor_x, cursor_y); cursor_y++;


                // send fake position
                sprintf( msg, "U" );
                msg[1] = 24;
                msg[2] = 34;
                NET_SendStringLen( msg, 3 );

                waitMs(500); // slower than cart
                memset(response, 0, sizeof(response ));
                count = read_n_bytes( response, 8 );
                sprintf(msg, "[%d,%d] [%d,%d] [%d,%d] [%d,%d]", response[0], response[1], response[2], response[3], response[4], response[5], response[6], response[7] );
                VDP_drawText(msg, cursor_x, cursor_y); cursor_y++;

                while(TRUE){
                    // do something
                    SYS_doVBlankProcess(); 
                }

                // request status

            }


        }
    }


    // if we're here, something went wrong.
    VDP_drawText("xPort no worky", cursor_x, cursor_y); cursor_y++;
    while(TRUE) {
        SYS_doVBlankProcess(); 
    }

    return 0;

}
