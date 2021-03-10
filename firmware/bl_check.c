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

#include <libopencm3/stm32/flash.h>
#include <stdint.h>
#include <string.h>
#include "bl_data.h"
#include "gettext.h"
#include "layout.h"
#include "memory.h"
#include "util.h"
#include "buttons.h"

void check_bootloader(void) {
	if (is_mode_unprivileged()) {
		return;
	}

	bool isSameVersion = true;
	for (int i = 0; i < FLASH_BOOT_LEN / 4; i++) 
	{
		const uint32_t *w = (const uint32_t *)(bl_data + i * 4);
		const uint32_t* c = (const uint32_t *)(FLASH_BOOT_START + i * 4);

		if( *w != *c )
		{
			isSameVersion = false;
			break;
		}
	}

	if( isSameVersion )
		return;
	
	layoutDialog(&bmp_icon_question, "Skip", "Install", NULL, _("New bootloader is"), _("available"), NULL, _("Are you ready to"), _("install it?"), NULL );

	do{
		delay(100000);
		buttonUpdate();
	} while( !button.YesUp && !button.NoUp );

	if( button.NoUp )
		return;

	layoutDialog(&bmp_icon_warning, NULL, NULL, NULL, _("Updating bootloader"), NULL, NULL, _("DO NOT UNPLUG"), _("YOUR Prokey!"), NULL);

	for (int tries = 0; tries < 3; tries++) {
		bool isOkay = true;
		// replace bootloader
		flash_unlock();
		for (int i = FLASH_BOOT_SECTOR_FIRST; i <= FLASH_BOOT_SECTOR_LAST; i++) {
			flash_erase_sector(i, FLASH_CR_PROGRAM_X32);
		}
		for (int i = 0; i < FLASH_BOOT_LEN / 4; i++) {
			const uint32_t *w = (const uint32_t *)(bl_data + i * 4);
			flash_program_word(FLASH_BOOT_START + i * 4, *w);

			const uint32_t* c = (const uint32_t *)(FLASH_BOOT_START + i * 4);

			if( *w != *c )
			{
				isOkay = false;
				break;
			}
		}
		flash_lock();

		if( isOkay == false )
			continue;
		else
		{
			layoutDialog(&bmp_icon_info, NULL, NULL, NULL, _("Update finished"), _("successfully."), NULL, _("Please reconnect"), _("the device."), NULL);
			shutdown();
			return;
		}
	}
	// show info and halt
	layoutDialog(&bmp_icon_error, NULL, NULL, NULL, _("Can not update"), _("bootloader."), NULL, _("Unplug your Prokey"), _("contact our support."), NULL);
	shutdown();
}
