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
#include "messages-common.pb.h"
#include "messages.h"
#include "fsm.h"

static PinNumberStatus  PinNumberGet    ( const char* msg, char* enteredPin, bool isUsb );
static void             PinNumberDone   ( void );

//**********************************
// 
//**********************************
bool PinNumberCheck ( bool use_cached )
{
    static CONFIDENTIAL char pin[MAX_PIN_LEN+1] = {0,0,0,0,0,0,0,0,0,0};

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

    bool result = PinNumberGet(_("Enter Pin"), pin, true);

    // Send Pin number done
    PinNumberDone();

    usbTiny(0);

    // Get Pin number
    if( result == false)
    {
        // Canceled
        fsm_sendFailure(FailureType_Failure_PinCancelled, NULL);
        return false;
    }
    
    // Check entered pins
    if(config_unlock(pin) == false)
    {
        // Wrong pin
        layoutDialog(&bmp_icon_error, NULL, "OK", NULL, NULL, "Pin Number is", "wrong.", NULL, NULL, NULL);
        buttonUpdate();
        ButtonGet(BTN_YES);
        fsm_sendFailure(FailureType_Failure_PinInvalid, NULL);
        usbPoll();
    }
    else
    {
        // Config 
        return true;
    }
        

    return false;
}
//**********************************
// 
//**********************************
PinNumberStatus PinNumberCheckNoUsb ( void )
{
    static CONFIDENTIAL char pin[MAX_PIN_LEN+1] = {0,0,0,0,0,0,0,0,0,0};

    // Return true if no pin
    if( config_hasPin() == false )
        return PinNumberOk;

    // Return true if session was unlocked
    if (session_isUnlocked()) {
        return PinNumberOk;
    }

    // Getting pin number without any USB activity
    PinNumberStatus result = PinNumberGet(_("Enter Pin"), pin, false);
    if(result != PinNumberOk)
    {
        return result;
    }

    // Check entered pins
    if(config_unlock(pin) == false)
    {
        // Wrong pin
        layoutDialog(&bmp_icon_error, NULL, "OK", NULL, NULL, "Pin Number is", "wrong.", NULL, NULL, NULL);
        buttonUpdate();
        ButtonGet(BTN_YES);

        return PinNumberWrong;
    }
    else
    {
        // Session is unlocked 
        return PinNumberOk;
    }
}
//**********************************
// 
//**********************************
PinNumberStatus PinNumberEnter(ButtonRequestType type, bool isSendDone, const char* title, char* pin)
{
    ButtonRequest resp;
    memzero(&resp, sizeof(ButtonRequest));
    resp.has_code = true;
    resp.code = type;
    
    msg_write(MessageType_MessageType_ButtonRequest, &resp);

    usbTiny(1);
    PinNumberStatus result = PinNumberGet(title, pin, true);
    
    if(isSendDone){
        // Done
        PinNumberDone();
    }

    usbTiny(0);

    return result;
}
//**********************************
// 
//**********************************
static PinNumberStatus    PinNumberGet(const char* msg, char* chr, bool isUsb)
{
    bool isLcdNeedUpdate = true;
    int pinIndex = 0;
    int longPressCounter = 0;
    int okProgressCounter = 0;
    int timeOutCounter = 0;
    const int y1 = 11;
    const int ym = 23;
    const int y2 = 30;
    int blinkCounter = 0;
    int blinkValue = (isUsb) ? 120000 : 200000;

    memzero(chr, MAX_PIN_LEN+1);
    chr[0] = '5';

    //! Timeout about 1 minute
    while(timeOutCounter++ < 60000000)
    {
        if(isUsb == true)
        {
            usbPoll();

            if(msg_tiny_id == MessageType_MessageType_Cancel)
            {
                return PinNumberCanceled;
            }
            else
            {
                msg_tiny_id = 0xFFFF;
            }
        }

        if(isLcdNeedUpdate)
        {
            isLcdNeedUpdate = false;
            blinkCounter = 0;

            oledClear();

            //! Header
            oledDrawStringCenter(OLED_WIDTH/2, 0, msg, FONT_STANDARD);
            oledHLine(8);

            //! Pin number body
            //! Line 1
            for(int n=0; n<6; n++)
            {
                if(pinIndex == n && okProgressCounter < 10)
                    oledDrawChar(9 + (n*19), (pinIndex < 6) ? ym : y1, chr[n], FONT_DOUBLE);
                else
                {
                    if(chr[n] != 0)
                        oledDrawChar(9 + (n*19), (pinIndex < 6) ? ym : y1, '*', FONT_DOUBLE);
                }
            }

            //! Line 2
            for(int n=6; n<MAX_PIN_LEN; n++)
            {
                if(pinIndex == n && okProgressCounter < 10)
                {
                    oledDrawChar(9 + ((n-6)*19), y2, chr[n], FONT_DOUBLE);
                }
                else
                {
                    if(chr[n] != 0)
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
                if(isUsb == true)
                    oledDrawString(1, OLED_HEIGHT-8, "Cancel", FONT_STANDARD);

                oledDrawString(OLED_WIDTH - 26, OLED_HEIGHT-8, "Next", FONT_STANDARD);
            }
            else if(pinIndex < 3 || chr[pinIndex] == ' ')
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
                oledFrame(0, OLED_HEIGHT-9, 64, OLED_HEIGHT-1);

                int m = okProgressCounter-10;
                if(m > 71)
                    m = 71;
                for(int v=0; v<m; v++)
                {
                    oledInvert(2+v, OLED_HEIGHT-7, 2+v, OLED_HEIGHT-3);
                }
            }

            oledRefresh();
        }


        if( ++blinkCounter > blinkValue)
        {
            blinkCounter = 0;
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

            oledRefresh();
        }

        buttonUpdate();

        if( button.UpUp )
        {
            if(chr[pinIndex] == 0 || chr[pinIndex] == '*')
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
            if(chr[pinIndex] == 0 || chr[pinIndex] == '*')
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
            isLcdNeedUpdate = true;
            okProgressCounter = 0;
            longPressCounter = 0;

            if(pinIndex == 0)
            {
                if(isUsb == true)
                {
                    return PinNumberCanceled;
                }
                else
                {
                    continue;
                }
            }

            chr[pinIndex--] = 0;
            chr[pinIndex] = 0;
        }
        else if(button.YesUp)
        {
            if(pinIndex < MAX_PIN_LEN-1 && okProgressCounter < 10 && chr[pinIndex] != 0)
            {
                chr[++pinIndex] = '5';
            }

            longPressCounter = 0;
            okProgressCounter = 0;
            isLcdNeedUpdate = true;
            timeOutCounter = 0;
        }
        else if(pinIndex >= 3 && chr[pinIndex] != 0 && button.YesDown > 2000 )
        {
            longPressCounter++;
            if(longPressCounter > 15)
            {
                longPressCounter = 0;
                okProgressCounter++;
                if(okProgressCounter > 71)
                {
                    while (true)
                    {
                        buttonUpdate();
                        if(button.YesUp)
                        {
                            return PinNumberOk;
                        }
                    } 
                }
            }
            isLcdNeedUpdate = true;
            timeOutCounter = 0;
        }
        
    }

    return PinNumberTimeout;
}

//**********************************
// Send Done to the UI
//**********************************
static void            PinNumberDone(void)
{
    // Done, This message helps the UI to close any message boxes or modal
    ButtonRequest resp;
    memzero(&resp, sizeof(ButtonRequest));
    resp.has_code = true;
    resp.code = ButtonRequestType_ButtonRequest_PinOnDeviceDone;

    msg_tiny_id = 0xFFFF;
    msg_write(MessageType_MessageType_ButtonRequest, &resp);

    // wait for ButtonAck
    while(1)
    {
        usbPoll();
        if(msg_tiny_id == MessageType_MessageType_ButtonAck || msg_tiny_id == MessageType_MessageType_Cancel)
        {
            msg_tiny_id = 0xFFFF;
            break;
        }
    }
}