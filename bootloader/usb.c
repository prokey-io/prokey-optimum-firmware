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

#include <libopencm3/stm32/flash.h>
#include <libopencm3/usb/usbd.h>

#include <string.h>

#include "bootloader.h"
#include "buttons.h"
#include "ecdsa.h"
#include "layout.h"
#include "memory.h"
#include "memzero.h"
#include "oled.h"
#include "rng.h"
#include "secp256k1.h"
#include "sha2.h"
#include "aes/aes.h"
#include "signatures.h"
#include "usb.h"
#include "util.h"
#include "auth.h"

#include <libopencm3/usb/dwc/otg_fs.h>
#include <libopencm3/usb/dwc/otg_common.h>

#include "usb21_standard.h"
#include "webusb.h"
#include "winusb.h"

#include "usb_desc.h"
#include "usb_erase.h"
#include "usb_send.h"

enum {
  STATE_READY,
  STATE_OPEN,
  STATE_FLASHSTART,
  STATE_FLASHING,
  STATE_END,
};

static uint32_t flash_pos = 0, flash_len = 0;
static char flash_state = STATE_READY;
static uint8_t flash_anim = 0;
static uint32_t stackPointer = 0;
static uint32_t buttonsTestCounter = 0;
static uint32_t buttonsTestThreshold = 4000000;

aes_decrypt_ctx decCtx;

static void rx_callback(usbd_device *dev, uint8_t ep) {
  (void)ep;
  static uint16_t msg_id = 0xFFFF;
  static uint8_t buf[64] __attribute__((aligned(4)));
  static uint8_t toDecript[16] __attribute__((aligned(4)));
	static int di;
	

  if (usbd_ep_read_packet(dev, ENDPOINT_ADDRESS_OUT, buf, 64) != 64) return;

  if (flash_state == STATE_END) {

    msg_id = (buf[3] << 8) + buf[4];
    
    if (msg_id == 0xFFF0) 
		{		
			DeviceReset(true);
			return;
		}
		return;
  }

  if (flash_state == STATE_READY || flash_state == STATE_OPEN ||
      flash_state == STATE_FLASHSTART ) {
    if (buf[0] != '?' || buf[1] != '#' ||
        buf[2] != '#') {  // invalid start - discard
      return;
    }
    // struct.unpack(">HL") => msg, size
    msg_id = (buf[3] << 8) + buf[4];
  }

  if (flash_state == STATE_READY || flash_state == STATE_OPEN) {

    //! Prokey Commands
		//! Prokey Restart (id 65520)
		if (msg_id == 0xFFF0) 
		{		
			DeviceReset(false);
			return;
		}

		//! Prokey Challenge (id 65521)
		if (msg_id == 0xFFF1) 
		{ 	
			flash_state = STATE_OPEN;
			sAuthResponse ar;
			uint8_t err=0x40;
			if( AuthNext( buf, 9, &ar, &err ) == false )
			{
				sendMsgFailureWithReason(dev, err);
				return;
			}

      if( AuthIsOkay() )
      {
        layoutDialog(&bmp_icon_info, NULL, NULL, NULL, "Your Prokey is", "genuine", NULL, NULL, NULL, NULL);
      }

			SendPacketToUsb( dev, 0xFFF1, ar.response, ar.len );
			return;
		}

    if (msg_id == 0x0000) 
    {  // Initialize message (id 0)
      send_msg_features(dev);
      flash_state = STATE_OPEN;
      return;
    }

    if (msg_id == 0x0037) 
    {  // GetFeatures message (id 55)
      send_msg_features(dev);
      return;
    }

    if (msg_id == 0x0001) 
    {  // Ping message (id 1)
      send_msg_success(dev);
      return;
    }
  }

  if( AuthIsOkay() == false )
  {
    sendMsgFailureWithReason(dev, 0x49);
    return;
  }

  if (flash_state == STATE_READY || flash_state == STATE_OPEN)
  {
    if (msg_id == 0x0005) 
    {  // WipeDevice message (id 5)
      layoutDialog(&bmp_icon_question, "Cancel", "Confirm", NULL, "Do you really want to", "wipe the device?", NULL, "All data will be lost.", NULL, NULL);
      bool but = get_button_response();

      if( but )
      {
        layoutDialog(&bmp_icon_question, "Cancel", "Confirm", NULL, "Never do this without", "your recovery sheet.", NULL, "All data will be lost.", NULL, NULL);
        but = get_button_response();
        if (but) 
        {
          erase_storage_code_progress();

          flash_state = STATE_END;
          layoutDialog(&bmp_icon_ok, NULL, NULL, NULL, "Device", "wiped.", NULL, "You may now", "unplug your Prokey.", NULL);
          send_msg_success(dev);

          return;
        } 
      }

      flash_state = STATE_END;
      layoutDialog(&bmp_icon_warning, NULL, NULL, NULL, "Device wipe", "aborted.", NULL, "You may now", "unplug your Prokey.", NULL);
      send_msg_failure(dev);
      return;
    }
  }

  if (flash_state == STATE_OPEN) 
  {
    if (msg_id == 0x0006) 
    {  // FirmwareErase message (id 6)

      bool proceed = false;
      if( firmware_present() )
      {
        layoutDialog(&bmp_icon_question, "Abort", "Continue", NULL, "Install new", "firmware?", NULL, "Never do this without", "your recovery card!", NULL);
        proceed = get_button_response();
      }
      else 
      {
        proceed = true;
      }

      if( proceed )
      {
          erase_code_progress();

          send_msg_success(dev);
          flash_state = STATE_FLASHSTART;

          return;
      }

      send_msg_failure(dev);
			flash_state = STATE_END;
			layoutDialog(&bmp_icon_warning, NULL, NULL, NULL, "Firmware installation", "canceled.", NULL, "You may now", "unplug your Prokey.", NULL);
			return;
    }

    return;
  }

  if (flash_state == STATE_FLASHSTART) 
  {
    if(msg_id == 0x0007)
		{
			//! Firmware Lenght
			//! Length-delimited type
			if(buf[9] != 0x0A)
			{
				send_msg_failure(dev);
				flash_state = STATE_END;
				layoutDialog(&bmp_icon_error, NULL, NULL, NULL, "Error installing ", "firmware.", NULL, "Unplug your Prokey", "and try again.", NULL);
				return;
			}

			const uint8_t *p = &buf[10];
			flash_len = readprotobufint(&p);

			if (flash_len > FLASH_TOTAL_SIZE - (FLASH_APP_START - FLASH_ORIGIN)) // firmware is too big
			{ 
				send_msg_failure(dev);
				flash_state = STATE_END;
				layoutDialog(&bmp_icon_error, NULL, NULL, NULL, "Firmware is too big.", NULL, NULL, NULL, NULL, NULL);
				return;
			}

			aes_init();
			aes_decrypt_key256(AuthGet()->sessionKeyHash, &decCtx );

			flash_state = STATE_FLASHING;
			flash_pos=0;
			di = 0;
			bool isFirst = true;

			flash_unlock();
			while (p < buf + 64) 
			{
				toDecript[di++] = *p;
				if( di == 16 )
				{
					di = 0;

					uint8_t plain[16] __attribute__((aligned(4)));
					aes_decrypt( toDecript, plain, &decCtx );
					
					for( int i=0; i<16; i+=4 )
					{
						if( isFirst )
						{
							isFirst = false;
							stackPointer = plain[i];
							stackPointer |= plain[i+1] << 8;
							stackPointer |= plain[i+2] << 16;
							stackPointer |= plain[i+3] << 24;
							flash_pos += 4;
							continue;
						}

						const uint32_t *w = (uint32_t *)&plain[i];
						//const uint32_t *w = (uint32_t *)&toDecript[i];
						flash_program_word(FLASH_APP_START + flash_pos, *w);
						flash_pos += 4;
					}
					
				}
				p++;
			}
			flash_lock();

			return;
		}
		return;
  }

  if (flash_state == STATE_FLASHING) 
  {
    if (buf[0] != '?') // invalid contents
		{	
			send_msg_failure(dev);
			flash_state = STATE_END;
			layoutDialog(&bmp_icon_error, NULL, NULL, NULL, "Error installing ", "firmware.", NULL, "Unplug your ProKey", "and try again.", NULL);
			return;
		}

		const uint8_t *p = buf + 1;
		if (flash_anim % 400 == 4) {
			layoutProgress("Installing, Please wait", 1000 * flash_pos / flash_len);
		}
		flash_anim++;
		flash_unlock();
		while (p < buf + 64 && flash_pos < flash_len) 
		{
			toDecript[di++] = *p;
			if( di == 16 )
			{
				di = 0;
				
				uint8_t plain[16] __attribute__((aligned(4)));
				aes_decrypt( toDecript, plain, &decCtx );
				
				for( int i=0; i<16; i+=4 )
				{
					const uint32_t *w = (uint32_t *)&plain[i];
					flash_program_word(FLASH_APP_START + flash_pos, *w);
					flash_pos += 4;
				}
				
			}
			p++;
		}
		flash_lock();

		// flashing done
		if (flash_pos == flash_len) 
		{
			flash_unlock();
			flash_program_word(FLASH_APP_START, stackPointer);
			flash_lock();

			layoutDialog(&bmp_icon_ok, NULL, NULL, NULL, "New firmware", "successfully installed.", NULL, "You may now", "unplug your Prokey.", NULL);
			send_msg_success(dev);

			flash_state = STATE_END;
			return;
		} 
		else 
		{
			return;
		}
  }
}

static void set_config(usbd_device *dev, uint16_t wValue) {
  (void)wValue;

  usbd_ep_setup(dev, ENDPOINT_ADDRESS_IN, USB_ENDPOINT_ATTR_INTERRUPT, 64, 0);
  usbd_ep_setup(dev, ENDPOINT_ADDRESS_OUT, USB_ENDPOINT_ATTR_INTERRUPT, 64,
                rx_callback);
}

static usbd_device *usbd_dev;
static uint8_t usbd_control_buffer[256] __attribute__((aligned(2)));

static const struct usb_device_capability_descriptor *capabilities[] = {
    (const struct usb_device_capability_descriptor
         *)&webusb_platform_capability_descriptor,
};

static const struct usb_bos_descriptor bos_descriptor = {
    .bLength = USB_DT_BOS_SIZE,
    .bDescriptorType = USB_DT_BOS,
    .bNumDeviceCaps = sizeof(capabilities) / sizeof(capabilities[0]),
    .capabilities = capabilities};

static void usbInit(void) {
  usbd_dev = usbd_init(&otgfs_usb_driver, &dev_descr, &config, usb_strings,
                       sizeof(usb_strings) / sizeof(const char *),
                       usbd_control_buffer, sizeof(usbd_control_buffer));

  //! Force USB to work without VBUS_DET
  OTG_FS_GCCFG = 0;
	OTG_FS_GCCFG = OTG_GCCFG_NOVBUSSENS | OTG_GCCFG_PWRDWN;

  usbd_register_set_config_callback(usbd_dev, set_config);
  usb21_setup(usbd_dev, &bos_descriptor);
  webusb_setup(usbd_dev, "prokey.io");
  winusb_setup(usbd_dev, USB_INTERFACE_INDEX_MAIN);
}

static void testButtons(void)
{
  uint16_t key = gpio_port_read(BTN_PORT);

  if((key & (BTN_PIN_YES | BTN_PIN_NO | BTN_PIN_DOWN | BTN_PIN_UP)) != (BTN_PIN_YES | BTN_PIN_NO | BTN_PIN_DOWN | BTN_PIN_UP))
  {
    if(buttonsTestCounter++ >= buttonsTestThreshold)
    {
      buttonsTestCounter = 0;
      buttonsTestThreshold = 10000;

      oledClear();
      oledDrawString(0,0, "TEST BUTTONS", FONT_STANDARD);

      key = gpio_port_read(BTN_PORT);

      int x = 0;
      if((key & BTN_PIN_NO) != BTN_PIN_NO)
        x = 3;
      else if((key & BTN_PIN_UP) != BTN_PIN_UP)
        x = 50;
      else if((key & BTN_PIN_DOWN) != BTN_PIN_DOWN)
        x = 80;
      else if((key & BTN_PIN_YES) != BTN_PIN_YES)
        x = OLED_WIDTH - 10;

      oledDrawString(x, OLED_HEIGHT - 8, "*", FONT_STANDARD);
      oledRefresh();
    }
  }
  else
  {
    buttonsTestCounter = 0;
  }
}


void usbLoop(void) {
  usbInit();
  for (;;) {
    usbd_poll(usbd_dev);
    
    testButtons();
  }
}
