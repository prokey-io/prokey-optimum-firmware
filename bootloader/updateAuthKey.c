#include "updateAuthKey.h"
#include "memory.h"
#include <otp.h>
#include <string.h>

//**********************************
// Update authentication key for the
// old version devices. A unique key
// was generated and added into the 
// firmware.
//**********************************
unsigned char UpdateAuthKey  ( void )
{
    //! Check if OTP already set
    if(flash_otp_is_locked(FLASH_OTP_MA_KEY_BLOCK))
    {
        return 0;
    }

    
    for( unsigned int keyIndex = FLASH_APP_START; keyIndex<FLASH_APP_START + FLASH_APP_LEN - 40; keyIndex += 4 )
    {
        if(*(volatile unsigned int *)(keyIndex) != 0x7541754D) // 0x4D754175 = MuAu 
            continue;
        
        if(*(volatile unsigned int *)(keyIndex+4) != 0x3179654B) // 0x4B657931 = Key1
            continue;

        if(*(volatile unsigned char *)(keyIndex+8) != 32) // 32 is len
            continue;
        
        //! Check if key set correctly
        if(*(volatile unsigned int *)(keyIndex+9) == 0 && *(volatile unsigned int *)(keyIndex+13) == 0)
            return 1;
        
        unsigned char key[32];
        for(int i=0; i<32; i++) 
        {
            key[i] = *(volatile unsigned char *)(keyIndex+9+i);
        }

        flash_otp_write(FLASH_OTP_MA_KEY_BLOCK, 0, key, 32);
        flash_otp_lock(FLASH_OTP_MA_KEY_BLOCK);
        return 0;
    }

    return 2;
}