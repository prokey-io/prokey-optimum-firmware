/*
 * This file is part of the TREZOR project, https://trezor.io/
 *
 * Copyright (C) 2014 Pavol Rusnak <stick@satoshilabs.com>
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

#include "buttons.h"
#include "util.h"

struct buttonState button;

#if !EMULATOR
uint16_t buttonRead(void) {
	return gpio_port_read(BTN_PORT);
}
#endif

void buttonUpdate()
{
	uint16_t state;
	static uint16_t last_state = BTN_PIN_YES | BTN_PIN_NO | BTN_PIN_UP | BTN_PIN_DOWN;

	state = buttonRead();

	if ((state & BTN_PIN_YES) == 0) {	// Yes button is down
		if ((last_state & BTN_PIN_YES) == 0) {		// last Yes was down
			if (button.YesDown < 2000000000) button.YesDown++;
			button.YesUp = false;
		} else {					// last Yes was up
			button.YesDown = 0;
			button.YesUp = false;
		}
	} else {				// Yes button is up
		if ((last_state & BTN_PIN_YES) == 0) {		// last Yes was down
			button.YesDown = 0;
			button.YesUp = true;
		} else {					// last Yes was up
			button.YesDown = 0;
			button.YesUp = false;
		}
	}

	if ((state & BTN_PIN_NO) == 0) {	// No button is down
		if ((last_state & BTN_PIN_NO) == 0) {		// last No was down
			if (button.NoDown < 2000000000) button.NoDown++;
			button.NoUp = false;
		} else {					// last No was up
			button.NoDown = 0;
			button.NoUp = false;
		}
	} else {				// No button is up
		if ((last_state & BTN_PIN_NO) == 0) {		// last No was down
			button.NoDown = 0;
			button.NoUp = true;
		} else {					// last No was up
			button.NoDown = 0;
			button.NoUp = false;
		}
	}
	
	//! <ProKey>
	if ((state & BTN_PIN_UP) == 0) 
	{	// No button is down
		if ((last_state & BTN_PIN_UP) == 0) 
		{	// last No was down
			if (button.UpDown < 2000000000) 
				button.UpDown++;
			
			button.UpUp = false;
		} 
		else 
		{	// last No was up
			button.UpDown = 0;
			button.UpUp = false;
		}
	} 
	else 
	{	// up button is up
		if ((last_state & BTN_PIN_UP) == 0) 
		{	// last up was down
			button.UpDown = 0;
			button.UpUp = true;
		} 
		else 
		{	// last up was up
			button.UpDown = 0;
			button.UpUp = false;
		}
	}
	
	if ((state & BTN_PIN_DOWN) == 0) 
	{	// No button is down
		if ((last_state & BTN_PIN_DOWN) == 0) 
		{	// last No was down
			if (button.DownDown < 2000000000) 
				button.DownDown++;
			
			button.DownUp = false;
		} 
		else 
		{	// last No was up
			button.DownDown = 0;
			button.DownUp = false;
		}
	} 
	else 
	{	// up button is up
		if ((last_state & BTN_PIN_DOWN) == 0) 
		{	// last up was down
			button.DownDown = 0;
			button.DownUp = true;
		} 
		else 
		{	// last up was up
			button.DownDown = 0;
			button.DownUp = false;
		}
	}
	//! </ProKey>

	last_state = state;
}
//**********************
//
//**********************
eButtons	ButtonGet	( eButtons expected )
{
	while(true)
	{
		#if !EMULATOR
		delay(100000);
		#endif
		buttonUpdate();

		eButtons b = BTN_NONE;
		if( button.YesUp )
			b |= BTN_YES;
		
		if( button.NoUp )
			b |= BTN_NO;

		if( button.DownUp )
			b |= BTN_DOWN;
		
		if( button.UpUp )
			b |= BTN_UP;

		if( expected & b )
			return b;
	}
}