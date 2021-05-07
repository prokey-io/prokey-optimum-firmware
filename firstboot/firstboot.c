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

#include "stdbool.h"
#include <libopencm3/cm3/scb.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/vector.h>
#include <libopencm3/cm3/mpu.h>

#define LED_PORT         GPIOA
#define LED_PIN          GPIO8
#define KEY_UP_PORT      GPIOC
#define KEY_UP_PIN       GPIO8

#define MPU_RASR_SIZE_16KB (0x0DUL << MPU_RASR_SIZE_LSB)
#define MPU_RASR_SIZE_128KB (0x10UL << MPU_RASR_SIZE_LSB)
#define MPU_RASR_SIZE_512KB (0x12UL << MPU_RASR_SIZE_LSB)
#define MPU_RASR_SIZE_4GB (0x1FUL << MPU_RASR_SIZE_LSB)

// http://infocenter.arm.com/help/topic/com.arm.doc.dui0552a/BABDJJGF.html
#define MPU_RASR_ATTR_FLASH (MPU_RASR_ATTR_C)
#define MPU_RASR_ATTR_SRAM (MPU_RASR_ATTR_C | MPU_RASR_ATTR_S)
#define MPU_RASR_ATTR_PERIPH (MPU_RASR_ATTR_B | MPU_RASR_ATTR_S)

#define MEM_MAP_BOOTLOADER_ADDRESS      0x08010000
#define MEM_MAP_APPLICATION_ADDRESS     0x08020000

#define FLASH_BASE (0x08000000U)
#define SRAM_BASE (0x20000000U)

int             main                    (void);
bool            UpButtonRead            (void);
bool            IsBootloaderAvailable   (void);
bool            IsFirmwareAvailable     (void);
static void     JumpToApplication       (bool isFirmware);
void            MpuConfigFirstboot      (void);
void            MpuDisable              (void);

int main()
{
    MpuConfigFirstboot();

    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOC);

    gpio_mode_setup(LED_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_PIN);
    gpio_clear(LED_PORT, LED_PIN);

    gpio_mode_setup(KEY_UP_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, KEY_UP_PIN);

    if(IsFirmwareAvailable() == false || UpButtonRead())
    {
        if(IsBootloaderAvailable())
        {
            JumpToApplication(false);
        }
        else
        {
            while(1)
            {
                gpio_toggle(LED_PORT, LED_PIN);
                for(volatile int i=0; i<500000; i++);
            }
        }
        
    }
    else
    {
        JumpToApplication(true);
    }
    
}
//**************************************
//
//**************************************
bool    UpButtonRead    (void)
{
    if(!gpio_get(KEY_UP_PORT, KEY_UP_PIN))
    {
        int d=0;
        while(1)
        {
            if(!gpio_get(KEY_UP_PORT, KEY_UP_PIN))
            {
                if(d++ > 1000)
                {
                    return true;
                }
            }
            else
            {
                return false;
            }
        }

    }

    return false;
}
//**************************************
//
//**************************************
bool IsBootloaderAvailable()
{
    return (((*(volatile unsigned int*)MEM_MAP_BOOTLOADER_ADDRESS) & 0x2FFE0000 ) == 0x20000000);
}
//**************************************
//
//**************************************
bool IsFirmwareAvailable()
{
    return (((*(volatile unsigned int*)MEM_MAP_APPLICATION_ADDRESS) & 0x2FFE0000 ) == 0x20000000);
}
//**************************************
//
//**************************************
static inline void __attribute__((noreturn))
JumpToApplication ( bool isFirmware )
{
    volatile unsigned int address = MEM_MAP_BOOTLOADER_ADDRESS;

    if( isFirmware )
        address = MEM_MAP_APPLICATION_ADDRESS;
    
    MpuDisable();
    
    const vector_table_t *ivt = (vector_table_t*)address;
    SCB_VTOR = (uint32_t)ivt;  // Set vector table
    __asm__ volatile("msr msp, %0" ::"r"(ivt->initial_sp_value));
    
    // Jump to address
    ivt->reset();
    while(1);
}

//**************************************
//
//**************************************
void MpuConfigFirstboot(void) 
{
  // Disable MPU
  MPU_CTRL = 0;

  // Everything (0x00000000 - 0xFFFFFFFF, 4 GiB, read-only)
  MPU_RBAR = 0 | MPU_RBAR_VALID | (0 << MPU_RBAR_REGION_LSB);
  MPU_RASR = MPU_RASR_ENABLE | MPU_RASR_ATTR_FLASH | MPU_RASR_SIZE_4GB |
             MPU_RASR_ATTR_AP_PRO_URO;

  // SRAM (0x20000000 - 0x2001FFFF, read-write, execute never)
  MPU_RBAR = SRAM_BASE | MPU_RBAR_VALID | (2 << MPU_RBAR_REGION_LSB);
  MPU_RASR = MPU_RASR_ENABLE | MPU_RASR_ATTR_SRAM | MPU_RASR_SIZE_128KB |
             MPU_RASR_ATTR_AP_PRW_URW | MPU_RASR_ATTR_XN;

  // Peripherals (0x40000000 - 0x4001FFFF, read-write, execute never)
  MPU_RBAR = PERIPH_BASE | MPU_RBAR_VALID | (3 << MPU_RBAR_REGION_LSB);
  MPU_RASR = MPU_RASR_ENABLE | MPU_RASR_ATTR_PERIPH | MPU_RASR_SIZE_128KB |
             MPU_RASR_ATTR_AP_PRW_URW | MPU_RASR_ATTR_XN;

  // Peripherals (0x40020000 - 0x40023FFF, read-write, execute never)
  MPU_RBAR = 0x40020000 | MPU_RBAR_VALID | (4 << MPU_RBAR_REGION_LSB);
  MPU_RASR = MPU_RASR_ENABLE | MPU_RASR_ATTR_PERIPH | MPU_RASR_SIZE_16KB |
             MPU_RASR_ATTR_AP_PRW_URW | MPU_RASR_ATTR_XN;

  // Don't enable DMA controller access
  // Peripherals (0x50000000 - 0x5007ffff, read-write, execute never)
  MPU_RBAR = 0x50000000 | MPU_RBAR_VALID | (5 << MPU_RBAR_REGION_LSB);
  MPU_RASR = MPU_RASR_ENABLE | MPU_RASR_ATTR_PERIPH | MPU_RASR_SIZE_512KB |
             MPU_RASR_ATTR_AP_PRW_URW | MPU_RASR_ATTR_XN;

  // Enable MPU
  MPU_CTRL = MPU_CTRL_ENABLE | MPU_CTRL_HFNMIENA;

  // Enable memory fault handler
  SCB_SHCSR |= SCB_SHCSR_MEMFAULTENA;

  __asm__ volatile("dsb");
  __asm__ volatile("isb");
}
//**************************************
//
//**************************************
void MpuDisable ( void )
{
    // Disable MPU
    MPU_CTRL = 0;

    __asm__ volatile("dsb");
    __asm__ volatile("isb");
}