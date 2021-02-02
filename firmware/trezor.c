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

#include "trezor.h"
#include "bitmaps.h"
#include "bl_check.h"
#include "buttons.h"
#include "common.h"
#include "config.h"
#include "gettext.h"
#include "layout.h"
#include "layout2.h"
#include "memzero.h"
#include "oled.h"
#include "rng.h"
#include "setup.h"
#include "timer.h"
#include "usb.h"
#include "util.h"
#include "prokey_layout.h"
#include "auth.h"
#include <libopencm3/stm32/gpio.h>
#include "prokey_recovery.h"
#include "pin_number.h"
#if !EMULATOR
#include <libopencm3/stm32/desig.h>
#include "otp.h"
#endif

/* Screen timeout */
uint32_t system_millis_lock_start;

void	ProkeyMenu	( void )
{
	buttonUpdate();

	// wake from screensaver on any button
	if (layoutLast == layoutScreensaver && (button.DownUp || button.YesUp || button.UpUp || button.DownUp)) {
		layoutHome();
		return;
	}

	// if homescreen is shown for too long
	if (layoutLast == layoutHome) 
	{
		if ((timer_ms() - system_millis_lock_start) >= config_getAutoLockDelayMs()) 
		{
			// lock the screen
			session_clear(true);
			layoutScreensaver();
			return;
		}
	}
	
	if( layoutLast == layoutHome )
	{
		if( button.NoDown >= 285000 * 2 )
		{
			ProkeyLayoutMain();
			return;
		}
	}
	
	if( layoutLast == ProkeyLayoutMain )
	{
		ProkeyLayoutMain();
		return;
	}

  if( layoutLast == ProkeyLayoutWalletRecovery1)
  {
    ProkeyLayoutWalletRecovery1();
    return;
  }

  if( layoutLast == ProkeyLayoutWalletRecovery2)
  {
    ProkeyLayoutWalletRecovery2();
    return;
  }

}

static void collect_hw_entropy(bool privileged) {

#if EMULATOR
  (void)privileged;
  memzero(HW_ENTROPY_DATA, HW_ENTROPY_LEN);
#else
  if (privileged) {
    desig_get_unique_id((uint32_t *)HW_ENTROPY_DATA);
    desig_get_unique_id(HW_UNIQUE_ID);
    // set entropy in the OTP randomness block
    if (!flash_otp_is_locked(FLASH_OTP_BLOCK_RANDOMNESS)) {
      uint8_t entropy[FLASH_OTP_BLOCK_SIZE];
      random_buffer(entropy, FLASH_OTP_BLOCK_SIZE);
      flash_otp_write(FLASH_OTP_BLOCK_RANDOMNESS, 0, entropy,
                      FLASH_OTP_BLOCK_SIZE);
      flash_otp_lock(FLASH_OTP_BLOCK_RANDOMNESS);
    }
    // collect entropy from OTP randomness block
    flash_otp_read(FLASH_OTP_BLOCK_RANDOMNESS, 0, HW_ENTROPY_DATA + 12,
                   FLASH_OTP_BLOCK_SIZE);
  } else {
    // unprivileged mode => use fixed HW_ENTROPY
    memset(HW_ENTROPY_DATA, 0x3C, HW_ENTROPY_LEN);
  }
#endif
}

int main(void) 
{
  setup();  
  __stack_chk_guard = random32();  // this supports compiler provided
                                   // unpredictable stack protection check
  oledInit();
  
  // TODO: Check First boot to update
  check_bootloader();
  
  setupApp();

  if (!is_mode_unprivileged()) {
    collect_hw_entropy(true);
    timer_init();
#ifdef APPVER
    // enable MPU (Memory Protection Unit)
    mpu_config_firmware();
#endif
  } 
  else 
  {
    collect_hw_entropy(false);
  }

#if DEBUG_LINK
  oledSetDebugLink(1);
  config_wipe();
#endif

  AuthInit();

  oledDrawBitmap(40, 0, &bmp_logo64);
  oledRefresh();

  config_init();
  layoutHome();
  usbInit();

  for (;;) {
    usbPoll();
    
    ProkeyMenu();
  }

  return 0;
}
