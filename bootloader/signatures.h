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

#ifndef __SIGNATURES_H__
#define __SIGNATURES_H__

#include <stdbool.h>
#include <stdint.h>

#define SIG_ERR_INVALID_SIG 0x60

typedef struct _sSigResponse {
    unsigned char   response[8];
    unsigned char   len;
} sSigResponse;

//! 1(Proto) + 1(version) + 2(Proto) + 3(indexes) + 64(sig1) + 64(sig2) + 64(sig3)  
#define SIG_RAW_DATA_LEN        202

#define SIG_ERR_PROTOBUF        0x60 
#define SIG_ERR_INDEX           0x61
#define SIG_ERR_LEN             0x62



#define SIG_OK 0x5A3CA5C3
#define SIG_FAIL 0x00000000

bool        SignatureSet( unsigned char* buf, sSigResponse* res);
uint32_t    SignatureCheck( const uint8_t* hash );

#endif
