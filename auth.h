#ifndef AUTH_H
#define AUTH_H

#include <stdbool.h>

typedef struct _sAuth {
    unsigned char   devRand[16];
    unsigned char   serRand[16];
    unsigned char   prvMagic[4];
    unsigned char   sessionKeyHash[32];
    char            cmdMySerial[32];
    bool            isGetRandomBefore;
    bool            isChallengingOkay;
} sAuth;

typedef struct _sAuthResponse {
    bool            isErr;
    unsigned char   response[128];
    unsigned char   len;
} sAuthResponse;

void            AuthInit        ( void );
bool            AuthNext        ( unsigned char* buf, unsigned char fistByteIndex, sAuthResponse* res, unsigned char* err );
bool            AuthIsOkay      ( void );
sAuth*          AuthGet         ( void );         

#endif //! AUTH_H