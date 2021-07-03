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

#include <ctype.h>
#include "prokey_recovery.h"
#include "bip39.h"
#include "config.h"
#include "fsm.h"
#include "gettext.h"
#include "layout2.h"
#include "memzero.h"
#include "messages.h"
#include "messages.pb.h"
#include "oled.h"
#include "protect.h"
#include "recovery-table.h"
#include "rng.h"
#include "usb.h"
#include "buttons.h"
#include "pin_number.h"

#if EMULATOR
#include <SDL2/SDL.h>                    
#endif

/* number of words expected in the new seed */
static uint32_t wordCount = 0;

// How many words has the user already entered.
static uint32_t wordIndex = 0;

static char isUsePassphrase = 0;

static char words[24][12];

//**********************************
//
//**********************************
static void ProkeyRecoveryFormatNumber(char *dest, int number, int x) 
{
  if (number < 10) 
  {
    dest[x] = ' ';
  } 
  else 
  {
    dest[x] = '0' + number / 10;
  }

  dest[x+1] = '0' + number % 10;
  
  if (number == 1 || number == 21) 
  {
    dest[x+2] = 's';
    dest[x+3] = 't';
  } 
  else if (number == 2 || number == 22) 
  {
    dest[x+2] = 'n';
    dest[x+3] = 'd';
  } 
  else if (number == 3 || number == 23) 
  {
    dest[x+2] = 'r';
    dest[x+3] = 'd';
  }
}
//**********************************
//
//**********************************
void ProRecovery( char isOnlyCheck )
{
    wordIndex = 0;
    wordCount = 24;
    memzero(words, sizeof(words));

    if( isOnlyCheck )
    {
        if (!PinNumberCheck(true)) 
        { 
            layoutHome();          
            return; 
        }               
    }

    //! Get Number of words
    ProRecoveryGetNumberOfWords();

    //! Is use Passphrase
    // ProRecoveryIsPassphrase();

    char tmpList[45][12] = {0};
    while(true)
    {
        char chr[3] = {0};
        if( ProRecoveryGetChar(chr) == 1 )
        {
            char foundedWords = ProRecoveryGetSuggestions( chr, tmpList);
            if( foundedWords > 0 )
            {
                int itemCounter = 0;
                int y = 0;
                int ly = 0;
                int wc = 0;
                while(true)
                {
                    oledClear();
                    ProRecoveryTitle(1);
                    ly = 0;
                    for( int i=0; i<foundedWords; i++)
                    {
                        oledDrawString(1, i*9 + 10, tmpList[i+wc], FONT_STANDARD);
                        if(++ly > 4)
                            break;   
                    }

                    oledInvert(0, y*9 + 10, 127, y*9 + 17);
                    oledHLine( OLED_HEIGHT-9);
                    oledDrawString(0, OLED_HEIGHT-7, "Cancel   Up           Down        OK", FONT_STANDARD);
                    oledRefresh();

                    buttonUpdate();
#if EMULATOR
                    SDL_Delay(100);                    
#endif
                    if( (button.DownDown > 10 || button.DownUp) && itemCounter < foundedWords - 1)
                    {
                        itemCounter++;
                        y++;

                        if( y > 4 )
                        {
                            y = 4;
                            wc++;
                        }
                    }
                    else if( (button.UpDown > 10 || button.UpUp) && itemCounter > 0)
                    {
                        itemCounter--;
                        y--;
                        if( y < 0 )
                        {
                            y = 0;
                            wc--;
                        }
                    }
                    else if( button.NoUp )
                    {
                        break;
                    }
                    else if( button.YesUp )
                    {
                        const char* tmp = tmpList[itemCounter];
                        strcpy(words[wordIndex], tmp);
                        wordIndex++;

                        if( wordIndex >= wordCount )
                        {
                            char new_mnemonic[MAX_MNEMONIC_LEN + 1] = {0};

                            strlcpy(new_mnemonic, words[0], sizeof(new_mnemonic));
                            for (uint32_t i = 1; i < wordCount; i++) {
                                strlcat(new_mnemonic, " ", sizeof(new_mnemonic));
                                strlcat(new_mnemonic, words[i], sizeof(new_mnemonic));
                            } 

                            int check = mnemonic_check(new_mnemonic);
                            if( check == 0 )
                            {
                                layoutDialog(&bmp_icon_error, "Exit", "Retry", NULL,
                                    "The seed is INVALID",
                                    NULL,
                                    NULL,
                                    NULL,
                                    NULL,
                                    NULL
                                );

                                eButtons btn = ButtonGet(BTN_YES | BTN_NO);
                                if( btn == BTN_NO )
                                    return;
                                else
                                {
                                    wordIndex = 0;
                                    break;
                                }
                            }
                            else
                            {
                                if( isOnlyCheck == 0 )
                                {
                                    config_setU2FCounter(0);
                                    config_setImported(true);
                                    config_setMnemonic(new_mnemonic);

                                    layoutDialog(&bmp_icon_ok, NULL, NULL, NULL, 
                                        "Wallet created",
                                        NULL,
                                        "You may unplug your",
                                        "Prokey now.",
                                        NULL,
                                        "Go to Prokey.io"
                                    );

                                    memzero(new_mnemonic, sizeof(new_mnemonic));

                                    while(true);
                                }
                                else
                                {
                                    char current_mnemonic[MAX_MNEMONIC_LEN + 1] = {0};
                                    config_getMnemonic(current_mnemonic, sizeof(current_mnemonic));

                                    if( strcmp(current_mnemonic, new_mnemonic) == 0 )
                                    {
                                        layoutDialog( &bmp_icon_ok, NULL, _("OK"), NULL,
                                            "The seed is correct",
                                            "and matches the",
                                            "one in the device",
                                            NULL,
                                            NULL,
                                            NULL
                                        );

                                        ButtonGet(BTN_YES);
                                        layoutHome();
                                        layoutLast = layoutHome;
                                        return;
                                    }
                                    else 
                                    {
                                        layoutDialog( &bmp_icon_ok, NULL, _("OK"), NULL,
                                            "The seed is correct",
                                            "but does NOT matches",
                                            "the one in the device",
                                            NULL,
                                            NULL,
                                            NULL
                                        );

                                        ButtonGet(BTN_YES);
                                        layoutHome();
                                        layoutLast = layoutHome;
                                        return;
                                    }

                                }
                            }
                        }

                        break;
                    }

                    for( volatile int w=0; w < 599999; w++ );
                }
            }
            else
            {
                layoutDialog(&bmp_icon_error, NULL, _("Retry"), NULL,
                    "The word is not",
                    "exist.",
                    NULL,
                    "Please try again",
                    NULL,
                    NULL);
                ButtonGet(BTN_YES);
            }
        }
        else
        {
            layoutDialog(&bmp_icon_question, _("No"), _("Yes"), NULL,
            "Do you really want to",
            "exit?",
            NULL,
            NULL,
            NULL,
            NULL);

            eButtons key = ButtonGet(BTN_NO | BTN_YES);
            if( key == BTN_YES )
            {
                layoutHome();
                layoutLast = layoutHome;
                return;
            }
        }
        
    }
}
//**********************************
//
//**********************************
char ProRecoveryGetSuggestions( char chrToFind[3], char sugg[15][12] )
{
    const char *const *wl = mnemonic_wordlist();
    int isFind = 0;
    while(*wl)
    {
        if( strncmp(chrToFind, *wl, 2) == 0 )
        {
            strcpy(sugg[isFind], *wl);

            isFind++;
        }

        wl++;
    }

    return isFind;
}
//**********************************
//
//**********************************
char ProRecoveryGetChar( char* chr )
{
    int n = 0;
    chr[0] = 'a';
    chr[1] = ' ';
    int x = 5; 
    int y = 23; 

    char isLedNeedUpdate = 1;
    while (true)
    {
        buttonUpdate();
#if EMULATOR
        SDL_Delay(100);                    
#endif

        if( button.DownUp || button.DownDown > 5 ){
            if( chr[n] == ' ')
                chr[n] = 'a';
            else
                chr[n]++;

            if( chr[n] > 'z' )
                chr[n] = 'a';

            isLedNeedUpdate = true;
        }
        else if( button.UpUp || button.UpDown > 5 ){
            chr[n]--;
            if( chr[n] < 'a' )
                chr[n] = 'z';

            isLedNeedUpdate = true;
        }
        else if( button.YesUp ){
            n++;
            if( n == 1 && chr[n] == ' ')
                chr[n] = 'a';

            if( n == 2 ){
                return 1;
            }

            isLedNeedUpdate = 1;
        }
        else if( button.NoUp ){
            n--;
            if( n == 0 )
                chr[1] = ' ';

            if( n == -1 )
                return 0;
            
            isLedNeedUpdate = 1;
        }

        if( isLedNeedUpdate == 1 )
        {
            isLedNeedUpdate = 0;
            oledClear();

            ProRecoveryTitle(0);

            if( n == 0 )
                oledDrawString( 1, 12, _("Choose the 1st character:"), FONT_STANDARD);
            else 
                oledDrawString( 1, 12, _("Choose the 2nd character:"), FONT_STANDARD);

            oledDrawChar(x + 2, y, chr[0], FONT_DOUBLE);
            oledDrawChar(x + 2 + 19, y, chr[1], FONT_DOUBLE);

            if( n == 0 ){
                oledInvert(x, y-1, x+19, y+16);
            }
            else
            {
                oledInvert(x+19, y-1, x+37, y+16);
            }

            oledHLine( OLED_HEIGHT-10);
            if( n == 0 )
                oledDrawString(0, OLED_HEIGHT-8, "Cancel    Up          Down       OK", FONT_STANDARD);
            else
                oledDrawString(0, OLED_HEIGHT-8, "Back      Up           Down         OK", FONT_STANDARD);

            oledRefresh();
        }

        if( button.DownDown != 0 || button.UpDown != 0 ){

            for( volatile int i=0; i<1599999; i++ );
        }
    }
}
//**********************************
//
//**********************************
void ProRecoveryTitle(char isShowSelect)
{

    if( isShowSelect )
    {
        char str[] = "Select the ##th word";
        ProkeyRecoveryFormatNumber(str, wordIndex + 1, 11);
        oledDrawString(0,0, str, FONT_STANDARD );
    }
    else
    {
        char str[] = "##th word";
        ProkeyRecoveryFormatNumber(str, wordIndex + 1, 0);
        oledDrawStringCenter(OLED_WIDTH/2,0, str, FONT_STANDARD );
    }

    oledHLine(8);
}
//**********************************
//
//**********************************
void ProRecoveryGetNumberOfWords()
{
    char isLcdNeedUpdate = 1;
    int itemCounter = 0;
    //int y = 1;
    while(true)
    {
        if(isLcdNeedUpdate)
        {
            isLcdNeedUpdate = 0;

            oledClear();

            oledDrawStringCenter(OLED_WIDTH/2, 0, _("Recovery words"), FONT_STANDARD);
            oledHLine(8);
            oledDrawString(0, 10, _("Set the number of words:"), FONT_STANDARD);

            oledDrawString(1, 20, _("12 words"), FONT_STANDARD);
            oledDrawString(1, 29, _("18 words"), FONT_STANDARD);
            oledDrawString(1, 38, _("24 words"), FONT_STANDARD);

            oledInvert(0, itemCounter*9 + 19, OLED_WIDTH, itemCounter*9 + 27);

            oledHLine( OLED_HEIGHT-10);
            oledDrawString(38, OLED_HEIGHT-8, "Up        Down      Next", FONT_STANDARD);

            oledRefresh();
        }

        eButtons btn = ButtonGet(BTN_ALL);

        if( btn == BTN_DOWN && itemCounter < 2)
        {
            itemCounter++;
            isLcdNeedUpdate = true;
        }
        else if( btn == BTN_UP && itemCounter > 0 )
        {
            itemCounter--;
            isLcdNeedUpdate = true;
        }
        else if( btn == BTN_YES )
        {
            if( itemCounter == 0 )
                wordCount = 12;
            else if( itemCounter == 1 )
                wordCount = 18;
            else 
                wordCount = 24;

            return;
        }
    }
}
//**********************************
//
//**********************************
void ProRecoveryIsPassphrase()
{
    layoutDialog(&bmp_icon_question, "No", "Yes", NULL,
        "Did you set passphrase?",
        NULL,
        NULL,
        NULL,
        NULL,
        NULL);
    
    eButtons btn = ButtonGet(BTN_YES | BTN_NO);
    if( btn == BTN_YES )
    {
        isUsePassphrase = 1;
    }
    else
    {
        isUsePassphrase = 0;
    }
    
    return;
}

