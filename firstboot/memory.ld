/* STM32F205RG - 1024K Flash, 128K RAM */
/* program starts at 0x08000000 */
MEMORY
{
	rom (rx) : ORIGIN = 0x08000000, LENGTH = 16K
	ram (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
}

INCLUDE cortex-m-generic.ld
