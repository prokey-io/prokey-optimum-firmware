#ifndef AUTH_H
#define AUTH_H

#include <stdbool.h>

#define AUTH_KEY_VERSION 0x01

#define AUTH_FALSE                  0x55
#define AUTH_TRUE                   0xAA

#define AUTH_ERR_PROTOBUF           0x40
#define AUTH_ERR_REQ_TYPE           0x41
#define AUTH_ERR_RNDD_NOT_SET       0x42
#define AUTH_ERR_DATA_SIZE          0x43
#define AUTH_ERR_TYPE               0x44
#define AUTH_ERR_CHALLENGE_FAILED   0x45
#define AUTH_ERR_SESSION_ERR        0x47
#define AUTH_ERR_CHALLENGE_UNKNOWN  0x48

#define AUTH_ERR_KEY_ALREADY_SET    0x50
#define AUTH_ERR_KEY_NOT_SET        0x51
#define AUTH_ERR_KEY_HASH_WRONG     0x52
#define AUTH_ERR_KEY_WRITE_ERR      0x53


typedef struct _sAuth {
    unsigned char   devRand[16];
    unsigned char   serRand[16];
    unsigned char   prvMagic[4];
    unsigned char   sessionKeyHash[32];
    unsigned char   cmdMySerial[32];
    bool            isGetRandomBefore;
    bool            isChallengingOkay;
} sAuth;

typedef struct _sAuthResponse {
    unsigned char   response[128];
    unsigned char   len;
} sAuthResponse;

void            AuthInit                ( void );
bool            AuthNext                ( unsigned char* buf, unsigned char fistByteIndex, sAuthResponse* res );
bool            AuthIsOkay              ( void );
sAuth*          AuthGet                 ( void );
void            AuthStatus              ( sAuthResponse* res );
bool            AuthSetKey              ( sAuthResponse* res );
bool            AuthWriteAuthKeyToOpt   ( unsigned char* buf, unsigned char fistByteIndex, sAuthResponse* res );

#endif //! AUTH_H