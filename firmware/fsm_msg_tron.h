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

void fsm_msgTronGetAddress(TronGetAddress* msg)
{
    CHECK_INITIALIZED
    CHECK_PIN

    RESP_INIT(TronAddress);
    resp->has_address = true;
    tron_getAddress(msg->address_n, msg->address_n_count, resp->address, sizeof(resp->address));

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
    CHECK_INITIALIZED
    CHECK_PIN

    RESP_INIT(TronSignedTx);

    if (!tron_signTransaction(msg, resp))
    {
        return;
    }

    msg_write(MessageType_MessageType_TronSignedTx, resp);
    layoutHome();
}
