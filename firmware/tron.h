/*
 * PROKEY HARDWARE WALLET
 * Copyright (C) Prokey.io
 * Copyright (C) Ali Akbar Mohammadi 2021

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

#ifndef __PROKEY_TRON_H__
#define __PROKEY_TRON_H__

#include "messages.pb.h"
#include "fsm.h"

// Get tron address
void tron_getAddress(const uint32_t* address_n, uint32_t address_n_count, 
                     char* address, uint32_t address_len);

bool tron_signingInit(const TronSignTx* msg);
bool tron_signingAbort(const char *reason);

#endif // __PROKEY_TRON_H__
