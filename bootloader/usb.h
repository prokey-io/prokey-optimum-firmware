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

#ifndef __USB_H__
#define __USB_H__

#define USB_MSG_ID_RESTART_REG      0xFFF0
#define USB_MSG_ID_CHALLENGE_REQ    0xFFF1
#define USB_MSG_ID_CHALLENGE_RES    0xFFF1 // Same as REQ

#define USB_MSG_ID_AUTH_STAT_REQ    0xFFF3
#define USB_MSG_ID_AUTH_STAT_RES    0xFFF4

#define USB_MSG_ID_SET_OTP_REQ      0xFFF5
#define USB_MSG_ID_SET_OTP_RES      0xFFF6

#define USB_MSG_ID_OTP_WRITE_REQ    0xFFF7
#define USB_MSG_ID_OTP_WRITE_RES    0xFFF8

void usbLoop(void);

#endif
