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

#include "util.h"
#include "rng.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/iwdg.h>

inline void delay(uint32_t wait) {
  while (--wait > 0) __asm__("nop");
}

void wait_random(void) {
  int wait = random32() & 0xff;
  volatile int i = 0;
  volatile int j = wait;
  while (i < wait) {
    if (i + j != wait) {
      shutdown();
    }
    ++i;
    --j;
  }
  // Double-check loop completion.
  if (i != wait || j != 0) {
    shutdown();
  }
}

static const char *hexdigits = "0123456789ABCDEF";

void uint32hex(uint32_t num, char *str) {
  for (uint32_t i = 0; i < 8; i++) {
    str[i] = hexdigits[(num >> (28 - i * 4)) & 0xF];
  }
}

// converts data to hexa
void data2hex(const void *data, uint32_t len, char *str) {
  const uint8_t *cdata = (uint8_t *)data;
  for (uint32_t i = 0; i < len; i++) {
    str[i * 2] = hexdigits[(cdata[i] >> 4) & 0xF];
    str[i * 2 + 1] = hexdigits[cdata[i] & 0xF];
  }
  str[len * 2] = 0;
}

uint32_t readprotobufint(const uint8_t **ptr, int* len) {
  uint32_t result = (**ptr & 0x7F);
  *len = 1;
  if (**ptr & 0x80) {
    *len=2;
    (*ptr)++;
    result += (**ptr & 0x7F) * 128;
    if (**ptr & 0x80) {
      *len=3;
      (*ptr)++;
      result += (**ptr & 0x7F) * 128 * 128;
      if (**ptr & 0x80) {
        *len=4;
        (*ptr)++;
        result += (**ptr & 0x7F) * 128 * 128 * 128;
        if (**ptr & 0x80) {
          *len=5;
          (*ptr)++;
          result += (**ptr & 0x7F) * 128 * 128 * 128 * 128;
        }
      }
    }
  }
  (*ptr)++;
  return result;
}

#if EMULATOR
void DeviceReset(bool isRunBootloader) {}
#else
void __attribute__((noreturn)) DeviceReset( bool isRunBootloader )
{
	(void)isRunBootloader;

	//! Enable Low Speed Internal clock 
	rcc_osc_on(RCC_LSI);

	/* Wait till LSI is ready */
	rcc_wait_for_sysclk_status(RCC_LSI);

	/* IWDG timeout equal to 250 ms */
	iwdg_set_period_ms(250);

	iwdg_reset();

	/* Enable IWDG (the LSI oscillator will be enabled by hardware) */
	iwdg_start();

	while(true);
}
#endif
