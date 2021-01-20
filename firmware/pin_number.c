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
bool    PinNumberGet(const char* msg, char* enteredPin)
{
    bool isLcdNeedUpdate = true;
    int pinIndex = 0;
    int longPressCounter = 0;
    int okProgressCounter = 0;
    char chr[MAX_PIN_LEN] = {'5', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
    int x = 5; 
    int y1 = 11;
    int ym = 23;
    int y2 = 30;

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
            for(int n=0; n<6; n++)
            {
                if(pinIndex == n )
                    oledDrawChar(x + 4 + (n*19), (pinIndex < 6) ? ym : y1, chr[n], FONT_DOUBLE);
                else
                {
                    if(chr[n] != ' ')
                        oledDrawChar(x + 4 + (n*19), (pinIndex < 6) ? ym : y1, '*', FONT_DOUBLE);
                }
            }

            //! Line 2
            for(int n=6; n<MAX_PIN_LEN; n++)
            {
                if(pinIndex == n)
                {
                    oledDrawChar(x + 4 + ((n-6)*19), y2, chr[n], FONT_DOUBLE);
                }
                else
                {
                    if(chr[n] != ' ')
                        oledDrawChar(x + 4 + ((n-6)*19), y2, '*', FONT_DOUBLE);
                }
                
            }

            //! Invert(highlight) the current editing pin
            //! if first line
            if(pinIndex < 6)
            {
                oledInvert(x+(pinIndex*19), ym-1, x+(pinIndex*19)+19, ym+16);
            }
            //! if second line
            else
            {
                oledInvert(x+((pinIndex-6)*19), y2-1, x+((pinIndex-6)*19)+19, y2+16);   
            }
            

            //! Footer
            oledHLine( OLED_HEIGHT-10);
            if(pinIndex < 4)
            {
                oledDrawString(OLED_WIDTH - 26, OLED_HEIGHT-8, "NEXT", FONT_STANDARD);
            }
            else if(pinIndex >=4 && pinIndex < 8)
            {
                oledDrawString(OLED_WIDTH - 44, OLED_HEIGHT-8, "NEXT/OK", FONT_STANDARD);
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

        if( button.DownUp )
        {
            if(chr[pinIndex] == ' ' || chr[pinIndex] == '*')
                chr[pinIndex] = '0';
            else if( ++chr[pinIndex] > '9' )
                chr[pinIndex] = '0';
            
            isLcdNeedUpdate = true;
            
            okProgressCounter = 0;
            longPressCounter = 0;
        }
        else if( button.UpUp )
        {
            if(chr[pinIndex] == ' ' || chr[pinIndex] == '*')
                chr[pinIndex] = '9';
            else if( --chr[pinIndex] < '0' )
                chr[pinIndex] = '9';
            
            isLcdNeedUpdate = true;

            okProgressCounter = 0;
            longPressCounter = 0;
        }
        else if( button.NoUp )
        {
            if(pinIndex == 0)
            {
                continue;
                //return false;
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
                pinIndex++;
                chr[pinIndex] = '5';
            }

            longPressCounter = 0;
            okProgressCounter = 0;
            isLcdNeedUpdate = true;
        }
        else if(button.YesDown > 2000 )
        {
            longPressCounter++;
            if(longPressCounter > 25)
            {
                longPressCounter = 0;
                okProgressCounter++;
            }
            isLcdNeedUpdate = true;
        }
        
    }
}