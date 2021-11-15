/*
 * PROKEY HARDWARE WALLET
 * Copyright (C) Prokey.io
 * Hadi Robati <hadi@prokey.io>

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

 * ----------- MUTUAL AUTHENTICATION -----------
 * It is a security process in which Prokey Device and Server authenticate each other
 * before secure communication occurs. This authentication is mandatory for updating or installing
 * the device firmware. After authentication, sending the firmware raw bin file from server
 * will be encrypted using a unique generated session key. This secure communication does not use
 * in normal wallet operation.
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
#include <otp.h>

static sAuth auth;
static unsigned char _tmpAuthKey[32];
static unsigned char _isTmpAuthKeySet = AUTH_FALSE;
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
bool  AuthNext        ( unsigned char* buf, unsigned char fistByteIndex, sAuthResponse* res)
{
    unsigned char n = fistByteIndex;

    //! The first byte is Protobuf Field Number + type
    //! for reqType, the first byte should be 0x08 which means Field: 1, type: varint
    if( buf[n++] != 0x08 )
    {
        res->response[0] = AUTH_ERR_PROTOBUF;
        return false;
    }

    unsigned char reqType = buf[n++]; 
    if (reqType < 1 || reqType > 4) 
    {
        res->response[0] = AUTH_ERR_REQ_TYPE;
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

        res->len = 6+16+32;
        
        auth.isGetRandomBefore = true;

        return true;
    }

    //! Second step is challenge
    if( reqType == 2 )
    {
        if( auth.isGetRandomBefore == false )
        {
            res->response[0] = AUTH_ERR_RNDD_NOT_SET;
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
                res->response[0] = AUTH_ERR_DATA_SIZE;
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
                res->response[0] = AUTH_ERR_TYPE;
                return false;
            }

            for( int i=0; i<16; i++ )
                *dp++ = buf[n++];
        }

        //! Is Key set?
        if(flash_otp_is_locked(FLASH_OTP_MA_KEY_BLOCK) == false)
        {
            //! Error code 0x51: Key not set 
            res->response[0] = AUTH_ERR_KEY_NOT_SET;
            return false;
        }

        uint8_t key[32] = {0};
        if( flash_otp_read(FLASH_OTP_MA_KEY_BLOCK, 0, key, 32) == false )
        {
            //! Error code 0x54: Can not read the key
            res->response[0] = AUTH_ERR_KEY_READ_ERR;
            return false;
        }

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
                res->response[0] = AUTH_ERR_CHALLENGE_FAILED;
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
            res->response[0] = AUTH_ERR_DATA_SIZE;
            return false;
        }

        for( int i=0; i<32; i++ )
            checkhash[i] = buf[n++];

        uint8_t sessionKey[32];
        uint8_t sessionKeyHash2[32];

        //! Read Key to generate the session key
        uint8_t key[32] = {0};
        if( flash_otp_read(FLASH_OTP_MA_KEY_BLOCK, 0, key, 32) == false )
        {
            //! Error code 0x54: Can not read the key
            res->response[0] = AUTH_ERR_KEY_READ_ERR;
            return false;
        }

        //! Hash the Key for 3 times to make the session key
        sha256_Raw( key, 32, sessionKey );
        sha256_Raw( sessionKey, 32, key );
        sha256_Raw( key, 32, sessionKey );

        //! Hash the sessionKey once more to make the sessionKeyHash and compare it to what server made   
        sha256_Raw( sessionKey, 32, sessionKeyHash2 );

        //! This step is necessary to make sure all steps are done correctly
        for( int i=0; i<32; i++ )
        {
            if( sessionKeyHash2[i] != checkhash[i] )
            {
                res->response[0] = AUTH_ERR_SESSION_ERR;
                return false;
            }
        }

        //! The sessionKeyHash will be used for encrypting device firmware
        //! Although the source code of firmware(this source code) is open, The reason we encrypt the firmware 
        //! is that to make sure there is no man in the middle/proxy who try to poison the 
        //! device firmware while updating the it.
        for( int i=0; i<32; i++ )
        {
            auth.sessionKeyHash[i] = sessionKey[i];
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
        
        res->len = 36;

        return true;
    }

    res->response[0] = AUTH_ERR_CHALLENGE_UNKNOWN;
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
//********************************
// This function returns the status
// Protobuf Schema for reference
// message MsgAuthKey
// {
//     required uint32 	    AuthVersion	= 1;
//     required bytes 		SerialNumber = 2;
//     optional bool		IsOtpSet = 3;
// }
//********************************
void AuthStatus ( sAuthResponse* res )
{
    //! Protobuf section
    //! Varint, Field 1
    res->response[0] = 0x08;

    //! Version 1
    res->response[1] = AUTH_KEY_VERSION;

    //! Length-delimited[32], Field 2
    res->response[2] = 0x12;
    //! Lenght
    res->response[3] = 32;
    SerialNumberGet32(&res->response[4]);

    //! Varint, Field 3
    res->response[36] = 0x18;
    res->response[37] = flash_otp_is_locked(FLASH_OTP_MA_KEY_BLOCK) ? 0x01 : 0x00;

    res->len = 38;
}
//********************************
// This function sets the AuthKey for the first time in Prokey Production Line but won't 
// write it into OTP until receiving next command to make sure server store it
// Protobuf Schema for reference
// message MsgAuthKey
// {
//     required uint32 	    AuthVersion	= 1;
//     required bytes 		SerialNumber = 2;
//     optional bytes		AuthKey = 4;
// }
//********************************
bool            AuthSetKey      ( sAuthResponse* res )
{
    //! Key can only be set once
    if(flash_otp_is_locked(FLASH_OTP_MA_KEY_BLOCK))
    {
        //! Error code 0x50: Key already set 
        res->response[0] = AUTH_ERR_KEY_ALREADY_SET;
        return false;
    }

    //! Protobuf section
    //! Varint, Field 1
    res->response[0] = 0x08;

    //! Version 1
    res->response[1] = AUTH_KEY_VERSION;

    //! Length-delimited[32], Field 2
    res->response[2] = 0x12;
    //! Lenght
    res->response[3] = 32;
    SerialNumberGet32(&res->response[4]);

    //! Length-delimited[32], Field 3
    res->response[36] = 0x22;
    //! Lenght
    res->response[37] = 32;

    //! Generate random numbers
    for( int i=0; i<32; i++ )
        res->response[i+38] = random32();
    
    //! Hash the numbers 3 times
    for( int h=0; h<3; h++)
    {
        sha256_Raw(&res->response[38], 32, _tmpAuthKey);
        memcpy(&res->response[38], _tmpAuthKey, 32);
    }

    _isTmpAuthKeySet = AUTH_TRUE;
    res->len = 70;

    return true;
}
//********************************
// This function writes the AuthKey to OPT Block 15
// Protobuf Schema for reference
// message MsgWriteReq
// {
//     required bytes 		Hash[AuthKey] = 2;
// }
//
// message MsgAuthKey
// {
//     required uint32 	    AuthVersion	= 1;
//     required bytes 		SerialNumber = 2;
// }
//********************************
bool AuthWriteAuthKeyToOpt(unsigned char* buf, unsigned char fistByteIndex, sAuthResponse* res)
{
    //! Check if _tmpAuthKey is set
    if(_isTmpAuthKeySet != AUTH_TRUE)
    {
        res->response[0] = AUTH_ERR_KEY_NOT_SET;
        return false;
    }

    // Field 1, Length-delimited
    if(buf[fistByteIndex++] != 0x0A) 
    {
        res->response[0] = AUTH_ERR_PROTOBUF;
        return false;
    }

    // Length should be 32
    if(buf[fistByteIndex++] != 32) 
    {
        res->response[0] = AUTH_ERR_DATA_SIZE;
        return false;
    }

    unsigned char hashAuthKey[32];
    sha256_Raw(_tmpAuthKey, 32, hashAuthKey);
    for(int i=0; i<32; i++)
    {
        if(hashAuthKey[i] != buf[fistByteIndex+i])
        {
            res->response[0] = AUTH_ERR_KEY_HASH_WRONG;
            return false;
        }
    }

    if(flash_otp_write(FLASH_OTP_MA_KEY_BLOCK, 0, _tmpAuthKey, 32) == false)
    {
        res->response[0] = AUTH_ERR_KEY_WRITE_ERR;
        return false;
    }

    //! Protobuf section
    //! Varint, Field 1
    res->response[0] = 0x08;

    //! Version 1
    res->response[1] = AUTH_KEY_VERSION;

    //! Length-delimited[32], Field 2
    res->response[2] = 0x12;
    //! Lenght
    res->response[3] = 32;
    SerialNumberGet32(&res->response[4]);

    //! Varint, Field 3
    res->response[36] = 0x18;
    res->response[37] = flash_otp_is_locked(FLASH_OTP_MA_KEY_BLOCK) ? 0x01 : 0x00;

    res->len = 38;
    return true;
}