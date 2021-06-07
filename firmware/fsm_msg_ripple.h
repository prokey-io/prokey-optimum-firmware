void fsm_msgRippleGetAddress(RippleGetAddress* msg)
{
    CHECK_INITIALIZED
    CHECK_PIN

    RESP_INIT(RippleAddress);

    HDNode *node = node = fsm_getDerivedNode(SECP256K1_NAME, msg->address_n,
                                             msg->address_n_count, NULL);
    if (!node) return;
    hdnode_fill_public_key(node);

    resp->has_address = true;
    hdnode_get_ripple_address(node, resp->address);

    if (msg->has_show_display && msg->show_display) 
    {
        char desc[16];
        strlcpy(desc, "Address:", sizeof(desc));

        if (!fsm_layoutAddress(resp->address, desc, false, 0, msg->address_n,
                            msg->address_n_count, true)) {
        return;
        }
    }

    msg_write(MessageType_MessageType_RippleAddress, resp);
    layoutHome();
}

void fsm_msgRippleSignTx(RippleSignTx* msg)
{

}

void fsm_msgRippleSignedTx(RippleSignedTx* msg)
{

}
