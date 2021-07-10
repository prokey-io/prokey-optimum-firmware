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

static const int8_t tron_prefix = 0x41; // Tron addresses must start with T

void fsm_msgTronGetAddress(TronGetAddress* msg)
{
    CHECK_INITIALIZED
    CHECK_PIN

    HDNode *node = fsm_getDerivedNode(SECP256K1_NAME, msg->address_n,
                                      msg->address_n_count, NULL);
    if (!node)
        return;
    hdnode_fill_public_key(node);

    RESP_INIT(TronAddress);
    resp->has_address = true;
    uint8_t hash[SHA3_256_DIGEST_LENGTH];
    keccak_256(node->public_key, sizeof(node->public_key), hash);
    uint8_t key[21];
    key[0] = tron_prefix;  
    // copy last 20 bytes of hash
    memcpy(key + 1, hash + (SHA3_256_DIGEST_LENGTH - 21), 20);
    base58_encode_check(key, sizeof(key), HASHER_SHA2D, resp->address, sizeof(resp->address));

    if (msg->has_show_display && msg->show_display)
    {
        char desc[16];
        strlcpy(desc, "Address:", sizeof(desc));

        if (!fsm_layoutAddress(resp->address, desc, false, 0, msg->address_n,
                               msg->address_n_count, false))
        {
            return;
        }
    }

    msg_write(MessageType_MessageType_TronAddress, resp);
    layoutHome();
}

void fsm_msgTronSignTx(TronSignTx* msg)
{

}
