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

static const uint64_t MIN_RIPPLE_FEE = 10;      // 10 drops
static const uint64_t MAX_RIPPLE_FEE = 1000000; // = 1 XRP
// https://xrpl.org/basic-data-types.html#specifying-currency-amounts
// the value in docs is in XRP, we declare it here in drops
static const uint64_t MAX_RIPPLE_AMOUNT = 100000000000000000UL;
static const uint32_t FLAG_FULLY_CANONICAL = 0x80000000;
static const uint32_t HASH_TX_SIGN = 0x53545800; // 'STX'
static bool ripple_sign_failed = false;
static char* ripple_failed_msg = NULL;

void fsm_msgRippleGetAddress(RippleGetAddress *msg)
{
    CHECK_INITIALIZED
    CHECK_PIN

    RESP_INIT(RippleAddress);

    HDNode *node = fsm_getDerivedNode(SECP256K1_NAME, msg->address_n,
                                      msg->address_n_count, NULL);
    if (!node)
        return;
    hdnode_fill_public_key(node);

    resp->has_address = true;
    hdnode_get_ripple_address(node, resp->address, sizeof(resp->address));

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

    msg_write(MessageType_MessageType_RippleAddress, resp);
    layoutHome();
}

void set_canonical_flag(RippleSignTx *msg)
{
    /*
    Our ECDSA implementation already returns fully-canonical signatures,
    so we're enforcing it in the transaction using the designated flag
    - see https://wiki.ripple.com/Transaction_Malleability#Using_Fully-Canonical_Signatures
    - see https://github.com/trezor/trezor-crypto/blob/3e8974ff8871263a70b7fbb9a27a1da5b0d810f7/ecdsa.c#L791
    */
    if (!msg->has_flags)
        msg->flags = 0;
    msg->flags |= FLAG_FULLY_CANONICAL;
}

enum Ripple_Transaction_Type
{
    Ripple_Transaction_Type_Payment = 0
};

enum Ripple_Field_Type
{
    Ripple_Field_Type_Int16 = 1,
    Ripple_Field_Type_Int32 = 2,
    Ripple_Field_Type_Amount = 6,
    Ripple_Field_Type_VL = 7,
    Ripple_Field_Type_Account = 8
};

static uint8_t *write_type(uint8_t *buf, uint8_t field_type, uint8_t key)
{
    if (key <= 0xF)
    {
        buf[0] = (field_type << 4) | key;
        return buf + 1;
    }
    // we need to write two bytes
    buf[0] = field_type << 4;
    buf[1] = key;
    return buf + 2;
}

static uint8_t *write_amount(uint8_t *buf, uint64_t amount)
{
    write_be_64(buf, amount);
    buf[0] &= 0x7F; // clear first bit to indicate XRP
    buf[0] |= 0x40; // set second bit to indicate positive number
    return buf + 8;
}

static uint8_t *write_var_len(uint8_t *buf, uint32_t size)
{
    // Implements variable-length int encoding from Ripple.
    // See: https://xrpl.org/serialization.html#length-prefixing
    if (size <= 192)
    {
        buf[0] = size;
        return buf + 1;
    }
    if (size <= 12480)
    {
        size -= 193;
        buf[0] = (193 + size) >> 8;
        buf[1] = size;
        return buf + 2;
    }
    if (size <= 918744)
    {
        size -= 12481;
        buf[0] = (241 + size) >> 16;
        buf[1] = size >> 8;
        buf[2] = size;
        return buf + 3;
    }
    ripple_sign_failed = true;
    ripple_failed_msg = "Value is too large";
    return buf;
}

static uint8_t *write_var(uint8_t *buf, uint8_t *val, uint32_t size)
{
    buf = write_var_len(buf, size);
    memcpy(buf, val, size);
    return buf + size;
}

static uint8_t* write_account(uint8_t *buf, char* account)
{
    uint8_t add[21];
    if (base58_decode_check_ripple(account, add, sizeof(add)) == 0)
    {
        ripple_sign_failed = true;
        ripple_failed_msg = "Account decode failed";
    }
    return write_var(buf, add + 1, 20);
}

static uint32_t serialize_ripple_tx(uint8_t *buf, RippleSignTx *msg, char *source_address,
                                    uint8_t *pubkey, uint32_t pubkey_size,
                                    uint8_t *signature, uint32_t signature_size)
{
    uint8_t *buf_start = buf;

    // must be sorted numerically first by type and then by name
    // write transaction type
    buf = write_type(buf, (uint8_t)Ripple_Field_Type_Int16, 2);
    write_be_16(buf, (uint16_t)Ripple_Transaction_Type_Payment);
    buf += 2;
    // write flags
    buf = write_type(buf, (uint8_t)Ripple_Field_Type_Int32, 2);
    write_be(buf, msg->flags);
    buf += 4;
    // write sequence
    buf = write_type(buf, (uint8_t)Ripple_Field_Type_Int32, 4);
    write_be(buf, msg->sequence);
    buf += 4;
    // write destinationTag
    if (msg->payment.has_destination_tag)
    {
        buf = write_type(buf, Ripple_Field_Type_Int32, 14);
        write_be(buf, msg->payment.destination_tag);
        buf += 4;
    }
    // write lastLedgerSequence
    if (msg->has_last_ledger_sequence)
    {
        buf = write_type(buf, Ripple_Field_Type_Int32, 27);
        write_be(buf, msg->last_ledger_sequence);
        buf += 4;
    }
    // write amount
    buf = write_type(buf, Ripple_Field_Type_Amount, 1);
    buf = write_amount(buf, msg->payment.amount);
    // write fee
    buf = write_type(buf, Ripple_Field_Type_Amount, 8);
    buf = write_amount(buf, msg->fee);
    // write signingPubKey
    if (pubkey)
    {
        buf = write_type(buf, Ripple_Field_Type_VL, 3);
        buf = write_var(buf, pubkey, pubkey_size);
    }
    // write txnSignature
    if (signature)
    {
        buf = write_type(buf, Ripple_Field_Type_VL, 4);
        buf = write_var(buf, signature, signature_size);
    }
    // write account
    if (source_address)
    {
        buf = write_type(buf, Ripple_Field_Type_Account, 1);
        buf = write_account(buf, source_address);
    }
    // write destination
    if (msg->payment.has_destination)
    {
        buf = write_type(buf, Ripple_Field_Type_Account, 3);
        buf = write_account(buf, msg->payment.destination);
    }

    return buf - buf_start;
}

void fsm_msgRippleSignTx(RippleSignTx *msg)
{
    CHECK_INITIALIZED
    CHECK_PIN
    ripple_sign_failed = false;

    CHECK_PARAM(msg->has_fee,
                "Transaction fee is missing");
    CHECK_PARAM(msg->fee >= MIN_RIPPLE_FEE,
                "Transaction fee is too low must be more than 10 drops");
    CHECK_PARAM(msg->fee <= MAX_RIPPLE_FEE,
                "Transaction fee is too high must be less than 1 XRP");
    CHECK_PARAM(msg->has_sequence,
                "Transaction squence is missing");
    CHECK_PARAM(msg->has_payment,
                "Transaction payment is missing");
    CHECK_PARAM(msg->payment.has_amount,
                "Payment amount is missing");
    CHECK_PARAM(msg->payment.amount > 0,
                "Payment amount must be greater than 0");
    CHECK_PARAM(msg->payment.amount <= MAX_RIPPLE_AMOUNT,
                "Payment amount is too high.");
    CHECK_PARAM(msg->payment.has_destination,
                "Payment destination is missing");
    CHECK_PARAM(strlen(msg->payment.destination) > 24 
                && strlen(msg->payment.destination) < 36,
                "Payment destination is mismatch");

    RESP_INIT(RippleSignedTx);

    // get the node and source address
    HDNode *node = fsm_getDerivedNode(SECP256K1_NAME, msg->address_n,
                                      msg->address_n_count, NULL);
    if (!node)
        return;
    hdnode_fill_public_key(node);

    char source_address[36];
    hdnode_get_ripple_address(node, source_address, sizeof(source_address));

    set_canonical_flag(msg);

    // serialize the tx to sign.
    uint8_t *buf = resp->serialized_tx.bytes;
    write_be(buf, HASH_TX_SIGN);
    uint32_t buf_size = 4;
    buf_size += serialize_ripple_tx(buf + 4, msg, source_address,
                                    node->public_key, sizeof(node->public_key), NULL, 0);

    if (ripple_sign_failed)
    {
        fsm_sendFailure(FailureType_Failure_ProcessError, ripple_failed_msg);
        return;
    }

    // Get confirmation from user
    if (msg->payment.has_destination_tag)
    {
        layoutRippleConfirmDestinationTag(msg->payment.destination_tag);
        if (!protectButton(ButtonRequestType_ButtonRequest_ConfirmWord, false))
        {
            fsm_sendFailure(FailureType_Failure_ActionCancelled, NULL);
            layoutHome();
            return;
        }
    }
    layoutRippleConfirmFee(msg->fee);
    if (!protectButton(ButtonRequestType_ButtonRequest_ConfirmOutput, false))
    {
        fsm_sendFailure(FailureType_Failure_ActionCancelled, NULL);
        layoutHome();
        return;
    }
    layoutRippleConfirmTx(msg->payment.amount, msg->payment.destination);
    if (!protectButton(ButtonRequestType_ButtonRequest_ConfirmOutput, false))
    {
        fsm_sendFailure(FailureType_Failure_ActionCancelled, NULL);
        layoutHome();
        return;
    }

    // sign the tx
    // We need first half of SHA512 as digest
    uint8_t sha[64];
    sha512_Raw(buf, buf_size, sha);

    // Sign and encode signature to DER format
    uint8_t sig[64];
    if (ecdsa_sign_digest(&secp256k1, node->private_key, sha, sig,
                          NULL, NULL) != 0)
    {
        fsm_sendFailure(FailureType_Failure_ProcessError, _("Signing failed"));
        layoutHome();
        return;
    }
    resp->has_signature = true;
    resp->signature.size = ecdsa_sig_to_der(sig, resp->signature.bytes);

    resp->has_serialized_tx = true;
    resp->serialized_tx.size = serialize_ripple_tx(buf, msg, source_address,
                                                   node->public_key, sizeof(node->public_key),
                                                   resp->signature.bytes, resp->signature.size);

    if (ripple_sign_failed)
    {
        fsm_sendFailure(FailureType_Failure_ProcessError, ripple_failed_msg);
        layoutHome();
        return;
    }

    msg_write(MessageType_MessageType_RippleSignedTx, resp);
    layoutHome();
}
