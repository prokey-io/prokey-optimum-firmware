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

#include "pin_number.h"
#include "layout2.h"
#include "buttons.h"
#include "oled.h"
#include "memzero.h"
#include "config.h"
#include "gettext.h"
#include "usb.h"
#include "messages.pb.h"
#include "messages.h"
#include "fsm.h"

static bool            PinNumberGet    ( const char* msg, char* enteredPin );

//**********************************
// 
//**********************************
bool PinNumberCheck ( bool use_cached )
{
    if( config_hasPin() == false )
        return true;

    if (use_cached && session_isUnlocked()) {
        return true;
    }

    ButtonRequest resp;
    memzero(&resp, sizeof(ButtonRequest));
    resp.has_code = true;
    resp.code = ButtonRequestType_ButtonRequest_EnterPinOnDevice;
    usbTiny(1);
    msg_write(MessageType_MessageType_ButtonRequest, &resp);

    int failedCounter = 0;
    char pin[MAX_PIN_LEN+1] = {0,0,0,0,0,0,0,0,0,0};

    while (failedCounter++ < 3)
    {
        if(PinNumberGet(_("Enter Pin"), pin) == false)
        {
            fsm_sendFailure(FailureType_Failure_PinCancelled, NULL);
            return false;
        }
        
        if(config_unlock(pin) == false)
        {
            oledClear();
            layoutDialog(&bmp_icon_error, NULL, "OK", NULL, NULL, "Pin Number is", "wrong.", NULL, NULL, NULL);
            fsm_sendFailure(FailureType_Failure_PinInvalid, NULL);
            oledRefresh();
            buttonUpdate();
            ButtonGet(BTN_YES);
        }
        else
        {
            return true;
        }
        
    }

    return false;
}
//**********************************
// 
//**********************************
static bool    PinNumberGet(const char* msg, char* enteredPin)
{
    bool isLcdNeedUpdate = true;
    int pinIndex = 0;
    int longPressCounter = 0;
    int okProgressCounter = 0;
    int timeOutCounter = 0;
    char chr[MAX_PIN_LEN] = {'5', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
    const int y1 = 11;
    const int ym = 23;
    const int y2 = 30;

    //! Timeout about 1 minute
    while(timeOutCounter++ < 60000000)
    {
        usbPoll();

        if (msg_tiny_id == MessageType_MessageType_ButtonAck) 
        {
            msg_tiny_id = 0xFFFF;
        }
        else if(msg_tiny_id == MessageType_MessageType_Cancel)
        {
            msg_tiny_id = 0xFFFF;
            usbTiny(0);
            return false;
        }
        else
        {
            msg_tiny_id = 0xFFFF;
        }
        

        if(isLcdNeedUpdate)
        {
            isLcdNeedUpdate = false;

            oledClear();

            //! Header
            oledDrawStringCenter(OLED_WIDTH/2, 0, msg, FONT_STANDARD);
            oledHLine(8);

            //! Pin number body
            //! Line 1
            for(int n=0; n<6; n++)
            {
                if(pinIndex == n )
                    oledDrawChar(9 + (n*19), (pinIndex < 6) ? ym : y1, chr[n], FONT_DOUBLE);
                else
                {
                    if(chr[n] != ' ')
                        oledDrawChar(9 + (n*19), (pinIndex < 6) ? ym : y1, '*', FONT_DOUBLE);
                }
            }

            //! Line 2
            for(int n=6; n<MAX_PIN_LEN; n++)
            {
                if(pinIndex == n)
                {
                    oledDrawChar(9 + ((n-6)*19), y2, chr[n], FONT_DOUBLE);
                }
                else
                {
                    if(chr[n] != ' ')
                        oledDrawChar(9 + ((n-6)*19), y2, '*', FONT_DOUBLE);
                }
                
            }

            //! Invert(highlight) the current editing pin
            //! if first line
            if(pinIndex < 6)
            {
                oledInvert(5+(pinIndex*19), ym-1, 5+(pinIndex*19)+19, ym+16);
            }
            //! if second line
            else
            {
                oledInvert(5+((pinIndex-6)*19), y2-1, 5+((pinIndex-6)*19)+19, y2+16);   
            }
            

            //! Footer
            oledHLine( OLED_HEIGHT-10);
            if(pinIndex == 0)
            {
                oledDrawString(1, OLED_HEIGHT-8, "Cancel", FONT_STANDARD);
                oledDrawString(OLED_WIDTH - 26, OLED_HEIGHT-8, "Next", FONT_STANDARD);
            }
            else if(pinIndex < 3)
            {
                oledDrawString(OLED_WIDTH - 26, OLED_HEIGHT-8, "Next", FONT_STANDARD);
            }
            else if(pinIndex >=3 && pinIndex < 8)
            {
                oledDrawString(OLED_WIDTH - 44, OLED_HEIGHT-8, "Next/OK", FONT_STANDARD);
            }
            else
            {
                oledDrawString(OLED_WIDTH - 3*6, OLED_HEIGHT-8, "OK", FONT_STANDARD);
            }

            if(okProgressCounter > 10)
            {
                oledFrame(0, OLED_HEIGHT-9, 66, OLED_HEIGHT-1);

                int m = okProgressCounter-10;
                if(m > 64)
                    m = 64;
                for(int v=0; v<m; v++)
                {
                    oledInvert(2+v, OLED_HEIGHT-7, 2+v, OLED_HEIGHT-3);
                }
            }

            oledRefresh();
        }

        buttonUpdate();

        if( button.UpUp )
        {
            if(chr[pinIndex] == ' ' || chr[pinIndex] == '*')
                chr[pinIndex] = '0';
            else if( ++chr[pinIndex] > '9' )
                chr[pinIndex] = '0';
            
            isLcdNeedUpdate = true;
            
            okProgressCounter = 0;
            longPressCounter = 0;
            timeOutCounter = 0;
        }
        else if( button.DownUp )
        {
            if(chr[pinIndex] == ' ' || chr[pinIndex] == '*')
                chr[pinIndex] = '9';
            else if( --chr[pinIndex] < '0' )
                chr[pinIndex] = '9';
            
            isLcdNeedUpdate = true;

            okProgressCounter = 0;
            longPressCounter = 0;
            timeOutCounter = 0;
        }
        else if( button.NoUp )
        {
            if(pinIndex == 0)
            {
                return false;
            }

            chr[pinIndex--] = ' ';
            chr[pinIndex] = '*';

            isLcdNeedUpdate = true;
            okProgressCounter = 0;
            longPressCounter = 0;
        }
        else if(button.YesUp)
        {
            if(pinIndex < MAX_PIN_LEN-1 && okProgressCounter < 10 && chr[pinIndex] != '*')
            {
                chr[++pinIndex] = '5';
            }

            longPressCounter = 0;
            okProgressCounter = 0;
            isLcdNeedUpdate = true;
            timeOutCounter = 0;
        }
        else if(pinIndex >= 3 && button.YesDown > 2000 )
        {
            longPressCounter++;
            if(longPressCounter > 15)
            {
                longPressCounter = 0;
                okProgressCounter++;
                if(okProgressCounter >= 64)
                {
                    int d=0;
                    for(; d<=pinIndex; d++)
                    {
                        enteredPin[d] = chr[d];
                    }
                    enteredPin[d] = '\0';

                    usbTiny(0);
                    return true;
                }
            }
            isLcdNeedUpdate = true;
            timeOutCounter = 0;
        }
        
    }

    usbTiny(0);
    return false;
}
