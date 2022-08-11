/*
 * This file is part of the Prokey project, https://prokey.io/
 *
 * Copyright (C) 2014 Pavol Rusnak <stick@satoshilabs.com>
 * Copyright (C) 2022 Hadi Robati <hadi@prokey.io>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <string.h>

#include <libopencm3/cm3/scb.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/iwdg.h>
#include <libopencm3/stm32/desig.h>

#include "bootloader.h"
#include "buttons.h"
#include "layout.h"
#include "memory.h"
#include "oled.h"
#include "rng.h"
#include "setup.h"
#include "usb.h"
#include "util.h"
#include "auth.h"
#include "common.h"

bool get_button_response(void) {
  do {
    delay(100000);
    buttonUpdate();
  } while (!button.YesUp && !button.NoUp);
  return button.YesUp;
}

bool firmware_present(void)
{
	return (((*(volatile unsigned int*)FLASH_APP_START) & 0x2FFE0000 ) == 0x20000000);
}

void MenuShowFirmwareFingerprint(const uint8_t *hash) {
  char str[21] = {0};
  data2hex(hash, 4, str);
  str[8] = '.';
  str[9] = '.';
  str[10] = '.';
  str[11] = '.';
  data2hex(hash+28, 4, &str[12]);

  layoutDialog(&bmp_icon_question, "Cancel", "Install", NULL , "Compare start and",
               "end of fingerprints", NULL, str, NULL, NULL);
}

uint8_t MenuAskForRunBootloader( void )
{
	uint8_t y = 30;
	bool isNeedUpdate = true;
	while(1)
	{
		if(isNeedUpdate)
		{
			isNeedUpdate = false;

			oledClear();
			oledDrawStringCenter(OLED_WIDTH/2, 1, ("Bootloader"), FONT_STANDARD);
			oledInvert(0,0,127,8);

			oledDrawString(1, 10, "Do you want to run the", FONT_STANDARD);
			oledDrawString(1, 20, "bootloader?", FONT_STANDARD);

			oledDrawStringCenter(OLED_WIDTH/2, 30, ("-----NO-----"), FONT_STANDARD);
			oledDrawStringCenter(OLED_WIDTH/2, 40, ("-----NO-----"), FONT_STANDARD);
			oledDrawStringCenter(OLED_WIDTH/2, 50, ("++++YES++++"), FONT_STANDARD);

			oledInvert(0,y-1,127,y+8);

			oledRefresh();
		}

		buttonUpdate();
		for( volatile int i=0; i<199999; i++ );
		if( button.YesUp)
		{
			if( y != 50 )
			{
				return 0;
			}
			else
			{
				return 1;
			}
			
		}
		if( button.DownUp )
		{
			y+=10;
			if( y > 50)
				y=50;
			
			isNeedUpdate = true;
		}
		else if(button.UpUp)
		{
			y-=10;
			if( y < 30 )
				y = 30;

			isNeedUpdate = true;
		}
	}

	return false;
}

void BootloaderMainMenu     ( void )
{
	oledClear();
	oledDrawStringCenter(OLED_WIDTH/2, 1, "Bootloader", FONT_STANDARD);
	oledInvert(0,0,127,8);

	oledDrawBitmap(40, 10, &bmp_logo48);

	//! Show version
	unsigned char buf[7];
	buf[0] = 'v';
	buf[1] = VERSION_MAJOR + '0';
	buf[2] = '.';
	buf[3] = VERSION_MINOR + '0';
	buf[4] = '.';
	buf[5] = VERSION_PATCH + '0';
	buf[6] = 0;

	oledDrawString(0,56, buf, FONT_STANDARD);

	oledRefresh();
}

void bootloader_loop(void)
{
	if( firmware_present() )
	{
		if( MenuAskForRunBootloader() == 0 )
		{
			//! Enable Low Speed Internal clock 
			rcc_osc_on(RCC_LSI);

			/* Wait till LSI is ready */
			rcc_wait_for_sysclk_status(RCC_LSI);

			/* IWDG timeout equal to 250 ms */
			iwdg_set_period_ms(250);

			iwdg_reset();

			/* Enable IWDG (the LSI oscillator will be enabled by hardware) */
			iwdg_start();

			while(1);
		}
	}

	BootloaderMainMenu();

	usbLoop();
}


int main(void) {

	setup();

	__stack_chk_guard = random32();  // this supports compiler provided
									// unpredictable stack protection checks

	memory_protect();
	oledInit();

	// HW_UNIQUE_ID will be used in Authenticating
	desig_get_unique_id(HW_UNIQUE_ID);
	
	// Enable MPU
	mpu_config_bootloader();
	
	AuthInit();

	bootloader_loop();

	return 0;
}
