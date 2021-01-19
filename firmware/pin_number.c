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

#include "layout2.h"
#include "buttons.h"
#include "oled.h"
#include "memzero.h"
#include "config.h"

//**********************************
//
//**********************************
static bool    PinNumberGet(const char* msg, char* enteredPin)
{
    bool isLcdNeedUpdate = true;
    int pinIndex = 0;
    int longPressCounter = 0;
    int okProgressCounter = 0;
    char chr[MAX_PIN_LEN] = {'0', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
    int x = 5; 
    int y1 = 23;
    int y2 = 33;

    while(true)
    {
        if(isLcdNeedUpdate)
        {
            isLcdNeedUpdate = false;

            oledClear();

            //! Header
            oledDrawStringCenter(OLED_WIDTH/2, 0, msg, FONT_STANDARD);
            oledHLine(8);

            //! Pin number body
            //! Line 1
            for(int n=0; n<5; n++)
            {
                if(pinIndex == n )
                    oledDrawChar(x + 2 + (n*19), y1, chr[n], FONT_DOUBLE);
                else
                {
                    oledDrawChar(x + 2 + (n*19), y1, '*', FONT_DOUBLE);
                }
            }

            //! Line 2
            for(int n=5; n<MAX_PIN_LEN; n++)
            {
                if(pinIndex == n)
                {
                    oledDrawChar(x + 2 + ((n-5)*19), y2, chr[n], FONT_DOUBLE);
                }
                else
                {
                    oledDrawChar(x + 2 + ((n-5)*19), y2, '*', FONT_DOUBLE);
                }
                
            }

            //! Invert(highlight) the current editing pin
            //! if first line
            if(pinIndex < 5)
            {
                oledInvert(x+(pinIndex*19), y1-1, x+(pinIndex*19)+19, y1+16);
            }
            //! if second line
            else
            {
                oledInvert(x+((pinIndex-5)*19), y2-1, x+((pinIndex-5)*19)+19, y2+16);   
            }
            

            //! Footer
            oledHLine( OLED_HEIGHT-10);
            if(pinIndex < 0)
            {
                oledDrawString(OLED_WIDTH - 6*6, OLED_HEIGHT-8, "NEXT", FONT_STANDARD);
            }
            else if(pinIndex >=4 & pinIndex < 8)
            {
                oledDrawString(OLED_WIDTH - 9*6, OLED_HEIGHT-8, "NEXT/OK", FONT_STANDARD);
            }
            else
            {
                oledDrawString(OLED_WIDTH - 3*6, OLED_HEIGHT-8, "OK", FONT_STANDARD);
            }

            if(longPressCounter > 0)
            {
                int m = longPressCounter;
                if(m > 30)
                    m = 30;
                for(int v=0; v<m; v++)
                {
                    oledInvert(2+v, OLED_HEIGHT-8, 2+v, OLED_HEIGHT-1);
                }
            }

            oledRefresh();
        }

        buttonUpdate();

        if( button.DownUp )
        {
            if( ++chr[pinIndex] > '9' )
                chr[pinIndex] = '0';
            
        }
        else if( button.UpUp )
        {
            if( --chr[pinIndex] < '0' )
                chr[pinIndex] = '9';
        }
        else if( button.NoUp )
        {
            if(pinIndex == 0)
            {
                return false;
            }

            chr[pinIndex--] = ' ';
        }
        else if(button.YesUp)
        {
            if(pinIndex < MAX_PIN_LEN)
            {
                pinIndex++;
                chr[pinIndex] = '0';
            }
        }
        else if(button.YesDown > 1000 )
        {
            longPressCounter++;
            isLcdNeedUpdate = true;
        }
    }
}