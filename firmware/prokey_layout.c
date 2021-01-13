/*
 * PROKEY HARDWARE WALLET
 * Copyright (C) Prokey.io

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdint.h>
#include <string.h>
#include <ctype.h>

#include "prokey_layout.h"
#include "storage.h"
#include "oled.h"
#include "bitmaps.h"
#include "string.h"
#include "util.h"
#include "qrcodegen.h"
#include "timer.h"
#include "bignum.h"
#include "secp256k1.h"
#include "nem2.h"
#include "gettext.h"
#include "layout2.h"
#include "buttons.h"
#include "config.h"
#include "prokey_recovery.h"
//**********************************
//
//**********************************
void    ProkeyLayoutMain    ( void )
{
    static bool lcdNeedsUpdate = true;
    static int y = 1; 

    if( button.DownUp && y<4 )
    {
        y++;
        lcdNeedsUpdate = true;
    }
    else if( button.UpUp && y>1 )
    {
        y--;
        lcdNeedsUpdate = true;
    } 
    else if( button.YesUp )
    {
        if( y == 1 )
        {
            layoutScreensaver();
            return;
        }
        else if( y == 2 )
        {
            ProkeyLayoutWalletRecovery1();
            return;
        }
        else if( y == 3 )
        {
            ProkeyLayoutCheckRocoverySeed();
            return;
        }
        else if( y == 4 )
        {
            layoutHome();
            return;
        }
    }

    if( layoutLast != ProkeyLayoutMain || lcdNeedsUpdate )
    {
        oledClear();
        oledDrawStringCenter(OLED_WIDTH / 2, 1, "Prokey Main Menu", FONT_STANDARD);
        oledInvert(0,0,127,8); 

        oledDrawString(2, 11, "Standby", FONT_STANDARD);
        oledDrawString(2, 21, "Wallet Recovery", FONT_STANDARD);
        oledDrawString(2, 31, "Check Recovery Seed", FONT_STANDARD);
        oledDrawString(2, 41, "Exit", FONT_STANDARD);

        oledInvert(0, (y*10), 127, (y*10)+9);

        oledRefresh();
    }

    layoutLast = ProkeyLayoutMain;
}
//**********************************
//
//**********************************
void ProkeyLayoutWalletRecovery1 ( void )
{
    if(config_isInitialized())
    {
        layoutDialogSwipe(&bmp_icon_error, NULL, "OK", NULL, 
        "Your Prokey already",
        "has initialized.",
        NULL,
        "For recovering, it",
        "should be wiped first",
        NULL
        );

        ButtonGet(BTN_YES);
        layoutHome();
        return;
    }

    if( layoutLast != ProkeyLayoutWalletRecovery1 ){

        layoutDialogSwipe(&bmp_icon_warning, _("Cancel"), _("Confirm"), NULL, 
        "Are you really sure",
        "you want to recover",
        "your Prokey?",
        NULL,
        NULL,
        NULL );
        
        layoutLast = ProkeyLayoutWalletRecovery1;    

        return;
    }

    if( button.YesUp ){
        ProkeyLayoutWalletRecovery2();
    }
    else if( button.NoUp ){
        layoutLast = ProkeyLayoutMain;
    }
}
//**********************************
//
//**********************************
void ProkeyLayoutWalletRecovery2 ( void )
{
    if( layoutLast != ProkeyLayoutWalletRecovery2 ){

        layoutDialogSwipe(&bmp_icon_warning, _("Cancel"), _("OK"), NULL, 
        "It is not recommended",
        "if you want to recover",
        "your Prokey from ",
        "other wallet",
        NULL,
        NULL );
        
        layoutLast = ProkeyLayoutWalletRecovery2;    

        return;
    }

    if( button.YesUp ){
        ProRecovery(0);
    }
    else if( button.NoUp ){
        layoutLast = ProkeyLayoutMain;
    }
}
//**********************************
//
//**********************************
void ProkeyLayoutCheckRocoverySeed(void)
{
    if(!config_isInitialized())
    {
        layoutDialogSwipe(&bmp_icon_error, NULL, "OK", NULL, 
        "Your Prokey has not",
        "initialized yet.",
        NULL,
        "There is no recovery",
        "seed to check",
        NULL
        );

        ButtonGet(BTN_YES);
        layoutHome();
        return;
    }

    ProRecovery(1);
}