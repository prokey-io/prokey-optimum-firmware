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

#include "tron.h"
#include "bip32.h"
#include "curves.h"
#include "messages-tron-internal.pb.h"
#include "gettext.h"

static const int8_t tron_prefix = 0x41; // Tron addresses must start with T

void tron_getAddress(uint32_t* address_n, uint32_t address_n_count, 
                     char* address, uint32_t address_len)
{
    HDNode *node = fsm_getDerivedNode(SECP256K1_NAME, address_n,
                                      address_n_count, NULL);
    if (!node)
        return;
    hdnode_fill_public_key(node);

    uint8_t hash[SHA3_256_DIGEST_LENGTH];
    keccak_256(node->public_key, sizeof(node->public_key), hash);
    uint8_t key[21];
    key[0] = tron_prefix;  
    // copy last 20 bytes of hash
    memcpy(key + 1, hash + (SHA3_256_DIGEST_LENGTH - 21), 20);
    base58_encode_check(key, sizeof(key), HASHER_SHA2D, address, address_len);    
}

bool tron_signingInit(const TronSignTx* msg)
{
    // check required fileds
    if (!msg->has_contract)
    {
        return tron_signingAbort("Contract is missing");
    }
    if (!msg->has_timestamp)
    {
        return tron_signingAbort("Timestamp is missing");
    }
    
    Transaction tx; // internal Tron transaction
    if (msg->contract.has_transfer_contract)
    {
        // transfer contract
        tx.raw_data.contract[0].type = ContractType_TransferContract;
        //msg->contract.transfer_contract
    }
    else if (msg->contract.has_transfer_asset_contract)
    {
        // transfer asset contract 
        tx.raw_data.contract[0].type = ContractType_TransferAssetContract;
    }
    
    return true;
}

bool tron_signingAbort(const char *reason)
{
  if (!reason) {
    reason = _("Unknown error");
  }

  fsm_sendFailure(FailureType_Failure_ProcessError, reason);
  layoutHome();
  return false;
}
