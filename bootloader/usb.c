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
  STATE_SIGNATURE,
  STATE_FLASHSTART,
  STATE_FLASHING,
  STATE_END,
};

static uint32_t flashPos = 0;
static uint32_t flashLen = 0;
static char flash_state = STATE_READY;
static uint8_t flash_anim = 0;
static uint32_t buttonsTestCounter = 0;
static uint32_t buttonsTestThreshold = 4000000;
static SHA256_CTX ctx;
static uint32_t firstChunkDataLen = 0;
static uint8_t firstFirmwareChunk[64] __attribute__((aligned(4)));

static void rx_callback(usbd_device *dev, uint8_t ep) {
  (void)ep;
  static uint16_t msg_id = 0xFFFF;
  static uint8_t buf[64] __attribute__((aligned(4)));
  static uint8_t code[32] __attribute__((aligned(4)));
  static uint8_t firstChuckOfFirmware[32] __attribute__((aligned(4)));
	static int di = 0;
  static uint32_t flashProgrammedLen = 0;
  static uint32_t tmpSigIndex = 0;

  static uint8_t  tmpSigData[202];
	

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
		if (msg_id == USB_MSG_ID_RESTART_REG) 
		{		
			DeviceReset(false);
			return;
		}

		//! Prokey Challenge (id 65521)
		if (msg_id == USB_MSG_ID_CHALLENGE_REQ) 
		{ 	
			flash_state = STATE_OPEN;
			sAuthResponse ar;
			if( AuthNext( buf, 9, &ar ) == false )
			{
				sendMsgFailureWithReason(dev, ar.response[0]);
				return;
			}

      if( AuthIsOkay() )
      {
        layoutDialog(&bmp_icon_info, NULL, NULL, NULL, "Your Prokey is", "genuine", NULL, NULL, NULL, NULL);
      }

			SendPacketToUsb( dev, USB_MSG_ID_CHALLENGE_RES, ar.response, ar.len );
			return;
		}

    //! Get Authenticate status
    if (msg_id == USB_MSG_ID_AUTH_STAT_REQ)
    {
      sAuthResponse ar;
      AuthStatus(&ar);
      SendPacketToUsb( dev, USB_MSG_ID_AUTH_STAT_RES, ar.response, ar.len);
      return;
    }

    //! Request to set the AuthKey in OTP
    if(msg_id == USB_MSG_ID_SET_OTP_REQ)
    {
      sAuthResponse ar;
      //! If OTP is already set, the function returns error
      if( AuthSetKey(&ar) == false )
      {
        sendMsgFailureWithReason(dev, ar.response[0]);
      }
      else
      {
        SendPacketToUsb( dev, USB_MSG_ID_SET_OTP_RES, ar.response, ar.len);
      }

      return;
    }

    //! Request ACK
    if(msg_id == USB_MSG_ID_OTP_WRITE_REQ)
    {
      sAuthResponse ar;
      if(AuthWriteAuthKeyToOpt(buf, 9, &ar) == false)
      {
        sendMsgFailureWithReason(dev, ar.response[0]);
      }
      else
      {
        SendPacketToUsb(dev, USB_MSG_ID_OTP_WRITE_RES, ar.response, ar.len);
      }
      return;
    }

    if (msg_id == USB_MSG_ID_INITIALIZE) 
    {  // Initialize message (id 0)
      send_msg_features(dev);
      flash_state = STATE_OPEN;
      return;
    }

    if (msg_id == USB_MSG_ID_GET_FEATURES) 
    {  // GetFeatures message (id 55)
      send_msg_features(dev);
      return;
    }

    if (msg_id == USB_MSG_ID_PING) 
    {  // Ping message (id 1)
      send_msg_success(dev);
      return;
    }
  }

  if( AuthIsOkay() == false )
  {
    sendMsgFailureWithReason(dev, AUTH_ERR_UNAUTHORIZED);
    return;
  }

  if (flash_state == STATE_READY || flash_state == STATE_OPEN)
  {
    if (msg_id == USB_MSG_ID_WIPE) 
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

    //! Set the firmware signature
    if(msg_id == USB_MSG_ID_SET_FIRMWARE_SIG_REQ)
    {
      //! First 9 bytes are for start bytes, msgId and len
      memcpy(tmpSigData, &buf[9], 64-9);
      tmpSigIndex = 64-9;

      //! Receiving remaining data in the next USB data
      flash_state = STATE_SIGNATURE;
      return;
    }
  }

  //! Receiving firmware signature data
  if(flash_state == STATE_SIGNATURE)
  {
    if (buf[0] != '?') // invalid contents
		{	
			send_msg_failure(dev);
			flash_state = STATE_END;
			layoutDialog(&bmp_icon_error, NULL, NULL, NULL, "Error setting", "firmware signatures.", NULL, "Unplug your ProKey", "and try again.", NULL);
			return;
		}

    int remaining = SIG_RAW_DATA_LEN - tmpSigIndex;
    if(remaining > 63)
      remaining = 63;

    memcpy(&tmpSigData[tmpSigIndex], &buf[1], remaining);
    tmpSigIndex += remaining;

    if(tmpSigIndex >= SIG_RAW_DATA_LEN)
    {
      tmpSigIndex = 0;
      sSigResponse sr;
      if(SignatureSet(tmpSigData, &sr) == false)
      {
        sendMsgFailureWithReason(dev, sr.response[0]);
        flash_state = STATE_END;
        layoutDialog(&bmp_icon_error, NULL, NULL, NULL, "Error setting", "firmware signatures.", NULL, "Unplug your ProKey", "and try again.", NULL);
				return;
      }

      SendPacketToUsb( dev, USB_MSG_ID_SET_FIRMWARE_SIG_RES, sr.response, sr.len );
      flash_state = STATE_OPEN;
    }

    return;

  }

  if (flash_state == STATE_OPEN) 
  {
    if (msg_id == USB_MSG_ID_ERASE_FIRMWARE) 
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
    if(msg_id == USB_MSG_ID_WRITE_FIRMWARE)
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
      int varIntLen = 0;
			flashLen = readprotobufint(&p, &varIntLen);

			if (flashLen > FLASH_TOTAL_SIZE - (FLASH_APP_START - FLASH_ORIGIN)) // firmware is too big
			{ 
				send_msg_failure(dev);
				flash_state = STATE_END;
				layoutDialog(&bmp_icon_error, NULL, NULL, NULL, "Firmware is too big.", NULL, NULL, NULL, NULL, NULL);
				return;
			}

			flash_state = STATE_FLASHING;

      //! Initial Hasher
      sha256_Init(&ctx);
      di = 0;
      flashPos = 0;
      flashProgrammedLen = 0;

      //! Write bytes to flash
      while(p < buf + 64)
      {
        code[di++] = *p;
        flashProgrammedLen++;
        //! We update the flash and hasher after every 32 bytes
			  if( di == 32 )
			  {
				  di = 0;

          //! Update hasher
          sha256_Update(&ctx, code, 32);

          //! Will write this part after checking the signature
          memcpy(firstChuckOfFirmware, code, 32);
          flashPos +=32;
        }
        p++;
      }

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
			layoutProgress("Installing, Please wait", 1000 * flashPos / flashLen);
		}
		flash_anim++;
		

		while (p < buf + 64 && flashProgrammedLen < flashLen) 
		{
			code[di++] = *p;
      flashProgrammedLen++;

      //! We update the flash and hasher after every 32 bytes
			if( di == 32 )
			{
				di = 0;

        //! Update hasher
        sha256_Update(&ctx, code, 32);
				
        flash_unlock();
        //! Write the 32 bytes to flash
				for( int i=0; i<32; i+=4 )
				{
					const uint32_t *w = (uint32_t *)&code[i];
					flash_program_word(FLASH_APP_START + flashPos, *w);
					flashPos += 4;
				}
        flash_lock();
				
			}
			p++;
		}
		
		// flashing done
		if (flashProgrammedLen == flashLen) 
		{
      //! if anything left in the buffer
      if(di > 0)
      {
        sha256_Update(&ctx, code, di);
        flash_unlock();
        for(int i=0; i<di; i++)
        {
          flash_program_byte( FLASH_APP_START + flashPos, code[i]);
          flashPos++;
        }
        flash_lock();
      }

      uint8_t hash[32];
      sha256_Final(&ctx, hash);
      if(SignatureCheck(hash) == SIG_FAIL) 
      {
        flash_unlock();
        flash_erase_sector(FLASH_CODE_SECTOR_FIRST, FLASH_CR_PROGRAM_X32);
        flash_lock();
        flash_state = STATE_END;
        sendMsgFailureWithReason(dev, SIG_ERR_INVALID_SIG);
        layoutDialog(&bmp_icon_error, NULL, NULL, NULL, "Error installing ", "firmware.", NULL, "Unplug your ProKey", "and try again.", "ERR:SIG1");
        return;
      }

      //! After checking the signature, the first chunck of firmware(the first 32 bytes) will be written to flash memory
			flash_unlock();
      for( int i=0; i<32; i+=4 )
      {
        const uint32_t *w = (uint32_t *)&firstChuckOfFirmware[i];
        flash_program_word(FLASH_APP_START + i, *w);
      }
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

  //! MsgId not supported
  sendMsgFailureWithReason(dev, 0xFF);
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
