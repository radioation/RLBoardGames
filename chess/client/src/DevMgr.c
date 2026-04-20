//  // Note: This clusterfuck is a prototype and may be slightly rewritten in the future
//  
#include "DevMgr.h"
//bool bRLNetwork = FALSE;            // Use RetroLink cartridge instead of built-in UART
//bool bXPNetwork = FALSE;            // Use XPort network adapter
DevPort sv_ListenPort = DP_Port2;   // Default UART port to listen on

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

