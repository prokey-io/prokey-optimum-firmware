/*
 * PROKEY HARDWARE WALLET
 * Copyright (C) Prokey.io

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

#include "auth.h"
#include <string.h>
#include "rng.h"
//#include "Challenge.h"
#include "serialno.h"
#include "util.h"
#include "sha2.h"
#include "aes/aes.h"
#include <stdbool.h>

static sAuth auth;
//********************************
//
//********************************
void            AuthInit        ( void )
{
    auth.isGetRandomBefore = false;
    auth.isChallengingOkay = false;
}
//********************************
//
// Protobuf Schema for reference
// message MsgChallenge
// {
//     required MsgEnumChallengeReq	    RequestType			= 1;  -> Filed: 0x08
//     optional bytes 					SerialNumber		= 2;  -> Field: 0x10
//     optional bytes					Random				= 3;  -> Field: 0x18
//     optional bytes					Challenge			= 4;  -> Field: 0x20
//     optional bytes					CheckHash			= 5;  -> Field: 0x28
// }
//********************************
bool  AuthNext        ( unsigned char* buf, unsigned char fistByteIndex, sAuthResponse* res, unsigned char* err )
{
    unsigned char n = fistByteIndex;

    //! The first byte is Protobuf Field Number + type
    //! for reqType, the first byte should be 0x08 which means Field: 1, type: varint
    if( buf[n++] != 0x08 )
    {
        *err = 0x40;
        return false;
    }

    unsigned char reqType = buf[n++]; 
    if (reqType < 1 || reqType > 4) 
    {
        *err = 0x41;
        return false;
    }

    //! Challenging will be started by getting SN + RNDD(Random Device)
    //! The Server after getting the SN+RNDD should encrypt it and send Enc(RNDD) + RNDS
    //! The device will check its own RNDD with Enc(RNDD) which got from server, if it was right, Device will send Enc(RNDS) 
    //! In Check Session, both side with make a session key my mixing HASH( HASH(RNDD + RNDS + Enc(RNDD) + Enc(RNDS) + PRV(N)) )
    //! First step is request Random
    if( reqType == 1 ) 
    {
        AuthInit();

        for( int i=0; i<16; i++ )
            auth.devRand[i] = random32();
        
        SerialNumberGet32(auth.cmdMySerial);


        //! Protobuf section
        //! Varint, Field 1
        res->response[0] = 0x08; 

        //! ReqType
        res->response[1] = reqType;

        //! Length-delimited[32], Field 2
        res->response[2] = 0x12;
        res->response[3] = 32;
        memcpy(&res->response[4], auth.cmdMySerial, 32);

        //! Length-delimited[24], Filed 3
        res->response[36] = 0x1A;
        res->response[37] = 16;
        memcpy(&res->response[38], auth.devRand, 16);

        res->isErr = false;
        res->len = 6+16+32;
        
        auth.isGetRandomBefore = true;

        return true;
    }

    //! Second step is challenge
    if( reqType == 2 )
    {
        if( auth.isGetRandomBefore == false )
        {
            *err = 0x42;
            return false;
        }

        //! Next time, new RNDD
        auth.isGetRandomBefore = false;

        uint8_t typeAndField;
        uint8_t size;
        uint8_t challenge[16];

        for( int j=0; j<2; j++ )
        {
            typeAndField = buf[n++];
            size = buf[n++];

            //! Check size and type
            if( size != 16 || (typeAndField & 0x07) != 0x02 )
            {
                *err = 0x43;
                return false;
            }

            //! Ignoring type
            //! now we only have TAG
            typeAndField >>=3;
            
            uint8_t *dp = 0;
            if( typeAndField == 3 )
                dp = auth.serRand;
            else if( typeAndField == 4 )
                dp = challenge;
            else
            {
                *err = 0x44;
                return false;
            }

            for( int i=0; i<16; i++ )
                *dp++ = buf[n++];
        }

        // THIS KEY HERE IS ONLY FOR TEST AND THIS IS NOT THE KEY OFFICIAL PROKEY DEVICE IS USING FOR SURE.
        // OFFICIAL PROKEY USES AN ALGORITHM TO GENERATE THIS KEY AND DOES NOT USE THE SAME KEY FOR EACH SESSION.
        // YOU CAN CHANGE THIS KEY FOR YOUR OWN TEST
        uint8_t key[32] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32};

        uint8_t aesMyRandom[16];

        //! Encrypt RNDD
        aes_encrypt_ctx ctx;
        aes_init();
        aes_encrypt_key256( key, &ctx );
        aes_encrypt( auth.devRand, aesMyRandom, &ctx );

        for( int i=0; i<16; i++ )
        {
            if(challenge[i] != aesMyRandom[i])
            {
                *err = 0x45;
                return false;
            }
        }
        
        uint8_t aesSerRnd[16];
        aes_init();
        aes_encrypt_key256( key, &ctx );
        aes_encrypt( auth.serRand, aesSerRnd, &ctx );

        
        res->response[0] = 0x08;
        res->response[1] = 2;

        //! Length-delimited[32], Field 2
        res->response[2] = 0x12;
        res->response[3] = 32;
        memcpy(&res->response[4], auth.cmdMySerial, 32);

        res->response[36] = 0x22;
        res->response[37] = 16;
        for( int i=0; i<16; i++ ) 
            res->response[i+38] = aesSerRnd[i];
        
        //SendPacketToUsb( dev, 0xFFF1, response, 16+4+34);
        res->isErr = false;
        res->len = 16+4+34;

        return true;
    }

    //! 3rd step is Check Session
    if( reqType == 3 )
    {
        uint8_t typeAndField = buf[n++];
        uint8_t size = buf[n++];
        uint8_t checkhash[32];

        if( size != 32 || typeAndField != 0x2A)
        {
            *err = 0x46;
            return false;
        }

        for( int i=0; i<32; i++ )
            checkhash[i] = buf[n++];

        uint8_t sessionKeyHash2[32];
        uint8_t sessionKey[32];
        uint8_t z=0;
        uint8_t m=0;
        uint8_t j=0;

        // THIS ALGORITHM HERE IS ONLY FOR TEST AND THIS IS NOT THE ALGORITHM OFFICIAL PROKEY DEVICE IS USING.
        // YOU CAN CHANGE THIS KEY FOR YOUR OWN TEST

        sessionKey[z++] = auth.devRand[m++]; // D
        sessionKey[z++] = auth.serRand[j++]; // S
        sessionKey[z++] = auth.devRand[m++]; // D
        sessionKey[z++] = auth.serRand[j++]; // S
        sessionKey[z++] = auth.devRand[m++]; // D
        sessionKey[z++] = auth.serRand[j++]; // S
        sessionKey[z++] = auth.serRand[j++]; // S
        sessionKey[z++] = auth.serRand[j++]; // S
        sessionKey[z++] = auth.serRand[m++]; // S
        sessionKey[z++] = auth.devRand[m++]; // D
        sessionKey[z++] = auth.serRand[j++]; // S
        sessionKey[z++] = auth.devRand[m++]; // D
        sessionKey[z++] = auth.devRand[m++]; // D
        sessionKey[z++] = auth.serRand[j++]; // S
        sessionKey[z++] = auth.devRand[m++]; // D
        sessionKey[z++] = auth.serRand[m++]; // S
        sessionKey[z++] = auth.serRand[j++]; // S
        sessionKey[z++] = auth.serRand[j++]; // S
        sessionKey[z++] = auth.devRand[m++]; // D
        sessionKey[z++] = auth.serRand[j++]; // S
        sessionKey[z++] = auth.serRand[j++]; // S
        sessionKey[z++] = auth.serRand[j++]; // S
        sessionKey[z++] = auth.devRand[m++]; // D
        sessionKey[z++] = auth.devRand[m++]; // D
        sessionKey[z++] = auth.serRand[j++]; // S
        sessionKey[z++] = auth.devRand[m++]; // D
        sessionKey[z++] = auth.devRand[m++]; // D
        sessionKey[z++] = auth.serRand[j++]; // S
        sessionKey[z++] = auth.devRand[m++]; // D
        sessionKey[z++] = auth.serRand[m++]; // S
        sessionKey[z++] = auth.serRand[j++]; // S
        sessionKey[z++] = auth.serRand[j++]; // S

        sha256_Raw( sessionKey, 32, auth.sessionKeyHash );
        sha256_Raw( auth.sessionKeyHash, 32, sessionKeyHash2 );

        for( int i=0; i<32; i++ )
        {
            if( sessionKeyHash2[i] != checkhash[i] )
            {
                *err = 0x47;
                return false;
            }
        }

        auth.isChallengingOkay = true;

        //! Varint, Field 1
        res->response[0] = 0x08;
        //! ReqType
        res->response[1] = 0x04;

        //! Length-delimited[32], Field 2
        res->response[2] = 0x12;
        //! Lenght
        res->response[3] = 32;
        memcpy(&res->response[4], auth.cmdMySerial, 32);
        
        res->isErr = false;
        res->len = 36;

        return true;
    }

    *err = 0x48;
    return false;
}
//********************************
//
//********************************
bool            AuthIsOkay      ( void )
{
    return auth.isChallengingOkay;
}
//********************************
//
//********************************
sAuth*          AuthGet         ( void )
{
    return &auth;
}