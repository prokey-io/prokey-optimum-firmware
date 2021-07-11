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
#include "pb_encode.h"

static const int8_t tron_prefix = 0x41; // Tron addresses must start with T

void tron_getAddress(const uint32_t* address_n, uint32_t address_n_count, 
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

// data must be 21 in length
bool tron_decodeAddress(uint8_t *data, uint32_t data_len, const char* address)
{
    if (base58_decode_check(address, HASHER_SHA2D, data, data_len) == 0)
    {
        return false;
    }
    return true;
}

bool tron_setContractAddress(pb_byte_t* bytes, pb_size_t* size, const char* address)
{
    *size = 21;
    return tron_decodeAddress(bytes, 21, address);
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
    
    // Get the user address
    char owner_address[36];
    tron_getAddress(msg->address_n, msg->address_n_count, owner_address, sizeof(owner_address));
    uint8_t owner_address_decoded[21];
    if (!tron_decodeAddress(owner_address_decoded, sizeof(owner_address_decoded), owner_address))
        return tron_signingAbort("Cannot decode owner address");    

    Transaction tx; // internal Tron transaction
    tx.raw_data.contract_count = 1;

    if (msg->contract.has_transfer_contract)
    {
        // transfer contract
        tx.raw_data.contract[0].type = ContractType_TransferContract;
        TransferContract contract;

        contract.amount = msg->contract.transfer_contract.amount; // set amount

        contract.owner_address.size = sizeof(owner_address_decoded); 
        memcpy(contract.owner_address.bytes, owner_address_decoded, sizeof(owner_address_decoded)); // set owner address

        if (!tron_setContractAddress(contract.to_address.bytes, &contract.to_address.size, 
                                     msg->contract.transfer_contract.to_address)) // set to address
            return tron_signingAbort("Cannot decode to address");

        // encode the data
        strncpy(
            tx.raw_data.contract[0].parameter.type_url,
            "type.googleapis.com/protocol.TransferContract",
            sizeof(tx.raw_data.contract[0].parameter.type_url)
        );
        pb_ostream_t stream = pb_ostream_from_buffer(tx.raw_data.contract[0].parameter.value.bytes, 
                                                     sizeof(tx.raw_data.contract[0].parameter.value.bytes));
        if (!pb_encode(&stream, TransferContract_fields, &contract))
            return tron_signingAbort("Failed to encode TransferContract");            
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
