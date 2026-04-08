// Note: This clusterfuck is a prototype and may be slightly rewritten in the future

#include "DevMgr.h"
//#include "devices/Keyboard_PS2.h"   // KB_SendCommand() and KB_Poll()
//#include "devices/Keyboard_Saturn.h"
//#include "devices/MegaMouse.h"
//#include "Keyboard.h"
#include "RL_Network.h"
#include "XP_Network.h"
#include "Network.h"
//#include "QMenu.h"                  // ChangeText() when KB is detected
//#include "Utils.h"                  // Definitions
//#include "Input.h"                  // Input_JP
//#include "system/PseudoFile.h"
//#include "system/StatusBar.h"

/*
ID	        Peripheral
1111 ($0F)	(undetectable)
1101 ($0D)	Mega Drive controller
1100 ($0C)	Mega Drive controller
1011 ($0B)	Saturn controller
1010 ($0A)	Printer
0111 ($07)	Sega multitap
0101 ($05)	Other Saturn peripherals
0011 ($03)	Mouse
0001 ($01)	Justifier
0000 ($00)	Menacer 
*/

#define DEVICE_UNKNOWN 0xF
#define DEVICE_MD_CTRL0 0xD
#define DEVICE_MD_CTRL1 0xC
#define DEVICE_SATURN_CTRL 0xB
#define DEVICE_PRINTER 0xA
#define DEVICE_MULTITAP 0x7
#define DEVICE_SATURN_PERIPHERAL 0x5
#define DEVICE_MOUSE 0x3
#define DEVICE_JUSTIFIER 0x1
#define DEVICE_MENACER 0

SM_Device DRV_Joypad;               // Joypad device
SM_Device DRV_Detector;             // Detector device
SM_Device *DevList[DEV_MAX];        // Device list
u8 DevSeq = 0;                      // Number of devices
bool bRLNetwork = FALSE;            // Use RetroLink cartridge instead of built-in UART
bool bXPNetwork = FALSE;            // Use XPort network adapter
bool bMegaCD = FALSE;               // Mega/Sega CD detected flag
bool bVRAM_128KB = FALSE;
bool bMouse = FALSE;
bool bKeyboard = FALSE;
DevPort sv_ListenPort = DP_Port2;   // Default UART port to listen on


/// @brief Get four bit device identifier (Sega devices only)
/// @param p Port to check (DP_Port1, DP_Port2, DP_Port3)
/// @return Four bit device identifier
u8 GetDeviceID(DevPort p)
{
    u8 r = 0;
    u8 dH = 0;
    u8 dL = 0;

    // Setup dummy device to test ports with
    DRV_Detector.Id.Bitmask = 0x7F;
    DRV_Detector.Id.Bitshift = 0;
    DRV_Detector.Id.Mode = DEVMODE_PARALLEL;

    SetDevicePort(&DRV_Detector, p);

    // Get device ID
    DEV_SetCtrl(DRV_Detector, 0x40);

    DEV_SetData(DRV_Detector, 0x40);
    dH = DEV_GetData(DRV_Detector, 0xF);

    DEV_ClrData(DRV_Detector);
    dL = DEV_GetData(DRV_Detector, 0xF);

    r |= (((dH & 8) >> 3) | ((dH & 4) >> 2)) << 3;
    r |= (((dH & 2) >> 1) | ((dH & 1)     )) << 2;
    r |= (((dL & 8) >> 3) | ((dL & 4) >> 2)) << 1;
    r |= (((dL & 2) >> 1) | ((dL & 1)     ));

    //kprintf("dH: $%X - dL: $%X", dH, dL);
    //kprintf("Device ID: $%X", r);

    return r;
}

/// @brief Set device to desired port
/// @param d Device pointer
/// @param p Desired port
void SetDevicePort(SM_Device *d, DevPort p)
{
    switch (p)
    {
        case DP_Port1:
            d->Ctrl   = (vu8*)PORT1_CTRL;
            d->Data   = (vu8*)PORT1_DATA;
            d->SCtrl  = (vu8*)PORT1_SCTRL;
            d->RxData = (vu8*)PORT1_SRx;
            d->TxData = (vu8*)PORT1_STx;
        break;

        case DP_Port2:
            d->Ctrl   = (vu8*)PORT2_CTRL;
            d->Data   = (vu8*)PORT2_DATA;
            d->SCtrl  = (vu8*)PORT2_SCTRL;
            d->RxData = (vu8*)PORT2_SRx;
            d->TxData = (vu8*)PORT2_STx;
        break;

        case DP_Port3:
            d->Ctrl   = (vu8*)PORT3_CTRL;
            d->Data   = (vu8*)PORT3_DATA;
            d->SCtrl  = (vu8*)PORT3_SCTRL;
            d->RxData = (vu8*)PORT3_SRx;
            d->TxData = (vu8*)PORT3_STx;
        break;

        default:
        break;
    }

    d->PAssign = p;
}

/// @brief Detect and initialize found devices
void DetectDevices()
{
    u8 DevId0 = GetDeviceID(DP_Port1);
    u8 DevId1 = GetDeviceID(DP_Port2);
    u8 DevId2 = GetDeviceID(DP_Port3);

    bKeyboard = FALSE;
    bMouse = FALSE;


    // -- UART setup -----------------------------------
    DRV_UART.Id.sName = "UART";
    DRV_UART.Id.Bitmask = 0x40; // 0x40 - Pin 7
    DRV_UART.Id.Bitshift = 0;
    DRV_UART.Id.Mode = DEVMODE_SERIAL | DEVMODE_PARALLEL;

    DevList[DevSeq++] = &DRV_UART;
    SetDevicePort(&DRV_UART, sv_ListenPort);
    *((vu8*) DRV_UART.SCtrl) = 0x38;
    
    Stdout_Push(" \e[97mChecking for network adapters...\e[0m\n");

    #ifndef EMU_BUILD
    u8 xpn_r = 0;

    if (RLN_Initialize())   // Check if RetroLink network adapter is present
    {
        bRLNetwork = TRUE;

        VDP_setReg(0xB, 0);   // Disable VDP ext interrupt (Enable: 8 - Disable: 0)

        NET_SetConnectFunc(RLN_Connect);
        NET_SetDisconnectFunc(RLN_BlockConnections);
        NET_SetGetIPFunc(RLN_GetIP);
        NET_SetPingFunc(RLN_PingIP);
        
        Stdout_Push(" \e[92mRLN: RetroLink found\e[0m\n");
    }
    else if ((xpn_r = XPN_Initialize())) // Check if xPort device is present
    {
        DRV_UART.Id.sName = "xPort UART";

        DEV_SetCtrl(DRV_UART, 0x40);
        DEV_ClrData(DRV_UART);

        bXPNetwork = TRUE;

        NET_SetConnectFunc(XPN_Connect);
        NET_SetDisconnectFunc(XPN_Disconnect);
        NET_SetGetIPFunc(XPN_GetIP);
        NET_SetPingFunc(XPN_PingIP);

        switch (xpn_r)
        {
            case 1:
                Stdout_Push(" \e[92mXPN: xPort module OK\e[0m\n");
            break;
            case 2:
                Stdout_Push(" \e[91mXPN: Error\e[0m\n");
            break;
        
            default:
            break;
        }
    }
    else    // No external network adapters found
    #endif
    {
        bRLNetwork = FALSE;
        bXPNetwork = FALSE;

        //Stdout_Push(" \e[93mNo network adapters found\e[0m\n");
        //Stdout_Push(" \e[97mListening on built in UART\e[0m\n");

        //SB_SetStatusIcon(ICO_NET_ERROR, ICO_POS_1);
        //SB_SetStatusIcon(ICO_NET_ERROR, ICO_POS_2);
    }
}

/// @brief Initialize device manager and find/init devices
void DeviceManager_Init()
{
    for (u8 s = 0; s < DEV_MAX; s++)
    {
        DevList[s] = NULL;
    }

    DevSeq = 0;
    
    bRLNetwork = FALSE;
    bXPNetwork = FALSE;
    bMegaCD = FALSE;
    bVRAM_128KB = FALSE;
    bMouse = FALSE;

    DetectDevices();    // Detect and setup
}
