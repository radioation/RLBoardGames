#include <genesis.h>
#include "Network.h"

int cursor_x, cursor_y;
u8 buttons, buttons_prev;
extern Buffer RxBuffer;

// network stuff
char server[16] = "010.025.050.061";
char request[64];
char response[128];


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


int main()
{
    char str[40];
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

    u8 xpn_r = 0;
    //    RLN_Initialize();
    if (xpn_r = XPN_Initialize()) // Check if xPort device is present
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

        char ip[16];
        memset( str, 0, sizeof(str) );
        memset( ip, 0, sizeof(ip) );
        //XPN_GetIP( ip ); 
        NET_GetIP( ip ); 
        sprintf(str, "IP: %s", ip); 
        VDP_drawText(str, cursor_x, cursor_y); cursor_y++;

        // reach out to server
        text_cursor_y = 5;
        // blocks whilewaiting for network to be ready.
        char fullserver[21];
        memset(fullserver,0, sizeof(fullserver));
        sprintf( fullserver, "%s:55558", server);

        NET_connect(text_cursor_x, text_cursor_y, fullserver); text_cursor_x=0; text_cursor_y++;

        if( strcmp( response, "HELO" ) != 0 ) {
            // TODO: we need to handle this error somehow. think about it
            //VDP_drawText("NOT HELO?", 0, 2 );
            return;
        }

        // request to join
        msg = "J";
        NET_sendMessage( msg );

        memset(response, 0, sizeof(response ));
        count = read_line( response, sizeof(response) );



        // send fake position



        // request status





//        if( NET_Connect( "172.29.84.124:5000" ) ) {
//
//            VDP_drawText("CONNECTED!", cursor_x, cursor_y); cursor_y++;
//
//            while(1) // Loop forever and print out any data we receive in the hardware receive fifo
//            { 
//                buttons = JOY_readJoypad(JOY_1);
//                if(buttons & BUTTON_START && buttons_prev == 0x00) { 
//                    VDP_drawText("send string", cursor_x, cursor_y); cursor_y++;
//                    NET_SendString("Test 1, 2, 3\n"); 
//                    waitMs(500); // slower than cart
//                }
//                if(buttons & BUTTON_A && buttons_prev == 0x00) { 
//                    NET_SendString("Button A Pressed\n"); 
//                    waitMs(500); 
//                }
//                if(buttons & BUTTON_B && buttons_prev == 0x00) { 
//                    NET_SendString("Button B Pressed\n"); 
//                    waitMs(500);
//                }
//                if(buttons & BUTTON_C && buttons_prev == 0x00) { 
//                    NET_SendString("Button C Pressed\n"); 
//                    waitMs(500);
//                }
//
//
//                int bw = Buffer_GetNum( &RxBuffer); // bytes waiting
//                if ( bw > 0 ) {
//                    memset( str, 0, sizeof( str ) );
//                    //sprintf(str, "bw: %d", bw );
//                    //VDP_drawText(str, cursor_x, cursor_y); cursor_y++;
//
//                    memset( str, 0, sizeof( data ) );
//                    Buffer_PeekLast( &RxBuffer, bw, data );
//                    Buffer_Flush0( &RxBuffer );
//
//                    for( int i=0; i < bw; ++i )  
//                    {   
//                        switch(data[i])
//                        {
//                            case 0x0A: // newline
//                                cursor_y++;
//                                cursor_x=0;
//                                break;              
//                            case 0x0D: // carridge return
//                                cursor_x=0;
//                                break; 
//                            default:   // print
//                                if (cursor_x >= 40) { cursor_x=0; cursor_y++; }
//                                if (cursor_y >= 28) { cursor_x=0; cursor_y=0; }
//                                sprintf(str, "%c", data[i]); // Convert
//                                VDP_drawText(str, cursor_x, cursor_y); cursor_x++;
//                                break;
//                        }
//                    }
//                    
//                }
//                buttons_prev = buttons;
//                SYS_doVBlankProcess(); 
//            }
//        } else {
//            VDP_drawText("NOT CONNECTED!", cursor_x, cursor_y); cursor_y++;
//        }
    }


    // if we're here, something went wrong.
    VDP_drawText("xPort no worky", cursor_x, cursor_y); cursor_y++;
    while(TRUE) {
        SYS_doVBlankProcess(); 
    }



    return 0;

}
