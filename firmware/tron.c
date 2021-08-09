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
#include "secp256k1.h"
#include "util.h"

static const int8_t tron_prefix = 0x41; // Tron addresses must start with T

HDNode *tron_getAddress(const uint32_t *address_n, uint32_t address_n_count,
                        char *address, uint32_t address_len)
{
    HDNode *node = fsm_getDerivedNode(SECP256K1_NAME, address_n,
                                      address_n_count, NULL);
    if (!node)
        return NULL;
    uint8_t public_key[65];
    ecdsa_get_public_key65(&secp256k1, node->private_key, public_key);

    uint8_t hash[SHA3_256_DIGEST_LENGTH];
    keccak_256(public_key + 1, sizeof(public_key) - 1, hash);
    uint8_t key[21];
    key[0] = tron_prefix;
    // copy last 20 bytes of hash
    memcpy(key + 1, hash + (SHA3_256_DIGEST_LENGTH - 20), 20);
    base58_encode_check(key, sizeof(key), HASHER_SHA2D, address, address_len);

    return node;
}

// data must be 21 in length
bool tron_decodeAddress(uint8_t *data, uint32_t data_len, const char *address)
{
    if (base58_decode_check(address, HASHER_SHA2D, data, data_len) == 0)
    {
        return false;
    }
    return true;
}

bool tron_setContractAddress(pb_byte_t *bytes, pb_size_t *size, const char *address)
{
    *size = 21;
    return tron_decodeAddress(bytes, 21, address);
}

bool tron_setBlockReference(const TronSignTx *msg, Transaction *tx)
{
    uint8_t hash[32];
    hex2data(msg->block_id, hash);
    memcpy(tx->raw_data.ref_block_hash.bytes, hash + 8, 8);
    tx->raw_data.ref_block_hash.size = 8;

    tx->raw_data.ref_block_bytes.size = 2;
    tx->raw_data.ref_block_bytes.bytes[0] = hash[6];
    tx->raw_data.ref_block_bytes.bytes[1] = hash[7];
    return true;
}

// encode contract inside the transaction
bool tron_encodeContract(Transaction *tx, char *contract_name, const pb_field_t *fileds, const void *contract)
{
    strncpy(
        tx->raw_data.contract[0].parameter.type_url,
        contract_name,
        sizeof(tx->raw_data.contract[0].parameter.type_url));
    pb_ostream_t stream = pb_ostream_from_buffer(tx->raw_data.contract[0].parameter.value.bytes,
                                                 sizeof(tx->raw_data.contract[0].parameter.value.bytes));
    if (!pb_encode(&stream, fileds, contract))
    {
        return false;
    }
    tx->raw_data.contract[0].parameter.value.size = stream.bytes_written;

    return true;
}

bool tron_signTransaction(const TronSignTx *msg, TronSignedTx *resp)
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
    HDNode *node = tron_getAddress(msg->address_n, msg->address_n_count, owner_address, sizeof(owner_address));
    uint8_t owner_address_decoded[21];
    if (!tron_decodeAddress(owner_address_decoded, sizeof(owner_address_decoded), owner_address))
        return tron_signingAbort("Cannot decode owner address");

    Transaction tx = Transaction_init_default; // internal Tron transaction
    tx.raw_data.contract_count = 1;

    if (msg->contract.has_transfer_contract)
    {
        //==================
        // transfer contract
        //==================
        tx.raw_data.contract[0].type = ContractType_TransferContract;
        TransferContract contract;

        contract.amount = msg->contract.transfer_contract.amount; // set amount

        // set owner address
        contract.owner_address.size = sizeof(owner_address_decoded);
        memcpy(contract.owner_address.bytes, owner_address_decoded, sizeof(owner_address_decoded)); // set owner address

        // set to address
        if (!tron_setContractAddress(contract.to_address.bytes, &contract.to_address.size,
                                     msg->contract.transfer_contract.to_address)) // set to address
        {
            return tron_signingAbort("Cannot decode to address");
        }

        // encode the contract
        if (!tron_encodeContract(&tx, "type.googleapis.com/protocol.TransferContract",
                                 TransferContract_fields, &contract))
        {
            return tron_signingAbort("Failed to encode TransferContract");
        }
    }
    else if (msg->contract.has_transfer_asset_contract)
    {
        //========================
        // transfer asset contract
        //========================
        tx.raw_data.contract[0].type = ContractType_TransferAssetContract;
        TransferAssetContract contract;

        // set the asset name
        contract.asset_name.size = strlen(msg->contract.transfer_asset_contract.asset_name);
        memcpy(contract.asset_name.bytes, msg->contract.transfer_asset_contract.asset_name,
               contract.asset_name.size);

        // set owner address
        contract.owner_address.size = sizeof(owner_address_decoded);
        memcpy(contract.owner_address.bytes, owner_address_decoded, sizeof(owner_address_decoded)); // set owner address

        // set to address
        if (!tron_setContractAddress(contract.to_address.bytes, &contract.to_address.size,
                                     msg->contract.transfer_contract.to_address)) // set to address
        {
            return tron_signingAbort("Cannot decode to address");
        }

        // set amount
        contract.amount = msg->contract.transfer_asset_contract.amount;

        // encode the contract
        if (!tron_encodeContract(&tx, "type.googleapis.com/protocol.TransferAssetContract",
                                 TransferAssetContract_fields, &contract))
        {
            return tron_signingAbort("Failed to encode TransferAssetContract");
        }
    }
    else if (msg->contract.has_freeze_balance_contract)
    {
        //========================
        // freeze balance contract
        //========================
        tx.raw_data.contract[0].type = ContractType_FreezeBalanceContract;
        FreezeBalanceContract contract;

        // set owner address
        contract.owner_address.size = sizeof(owner_address_decoded);
        memcpy(contract.owner_address.bytes, owner_address_decoded, sizeof(owner_address_decoded)); // set owner address

        // set to address
        if (msg->contract.freeze_balance_contract.has_receiver_address)
        {
            if (!tron_setContractAddress(contract.receiver_address.bytes, &contract.receiver_address.size,
                                         msg->contract.freeze_balance_contract.receiver_address)) // set to address
            {
                return tron_signingAbort("Cannot decode to address");
            }
        }

        // set frozen balance and frozen duration
        contract.frozen_balance = msg->contract.freeze_balance_contract.frozen_balance;
        contract.frozen_duration = msg->contract.freeze_balance_contract.frozen_duration;

        // set resource code
        contract.resource = msg->contract.freeze_balance_contract.resource;

        // encode the contract
        if (!tron_encodeContract(&tx, "type.googleapis.com/protocol.FreezeBalanceContract",
                                 FreezeBalanceContract_fields, &contract))
        {
            return tron_signingAbort("Failed to encode FreezeBalanceContract");
        }
    }

    // set timestamp
    tx.raw_data.timestamp = msg->timestamp;
    tx.raw_data.expiration = msg->has_expiration ? msg->expiration : msg->timestamp + 10 * 60 * 60 * 1000; // 10 hours
    tx.raw_data.fee_limit = msg->fee_limit;

    // set block refrence
    if (!tron_setBlockReference(msg, &tx))
        return false;

    // now serialize transaction
    pb_ostream_t stream = pb_ostream_from_buffer(resp->serialized_tx.bytes,
                                                 sizeof(resp->serialized_tx.bytes));
    if (!pb_encode(&stream, rawTr_fields, &tx.raw_data))
    {
        return tron_signingAbort("Failed to encode Transaction");
    }

    // hash the serialized transaction
    uint8_t hash[32];
    sha256_Raw(resp->serialized_tx.bytes, stream.bytes_written, hash);

    // Now sign the hash
    if (ecdsa_sign_digest(&secp256k1, node->private_key, hash, tx.signature[0].bytes,
                          tx.signature[0].bytes + 64, NULL) != 0)
    {
        return tron_signingAbort("Signing failed");
    }
    tx.signature_count = 1;
    tx.signature[0].size = 65;

    // reset stream
    stream = pb_ostream_from_buffer(resp->serialized_tx.bytes,
                                    sizeof(resp->serialized_tx.bytes));
    if (!pb_encode(&stream, Transaction_fields, &tx))
    {
        return tron_signingAbort("Failed to encode signed Transaction");
    }
    resp->has_serialized_tx = true;
    resp->serialized_tx.size = stream.bytes_written;

    return true;
}

bool tron_signingAbort(const char *reason)
{
    if (!reason)
    {
        reason = _("Unknown error");
    }

    fsm_sendFailure(FailureType_Failure_ProcessError, reason);
    return false;
}
