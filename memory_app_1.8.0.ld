/* STM32F205RG - 1024K Flash, 128K RAM */
/* program starts at 0x08020000 */
MEMORY
{
	rom (rx) : ORIGIN = 0x08020000, LENGTH = 1024K - 128K
	ram (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
}

SECTIONS
{
	.confidential (NOLOAD) : {
		*(confidential)
		ASSERT ((SIZEOF(.confidential) <= 32K), "Error: Confidential section too big!");
	} >ram
}

INCLUDE cortex-m-generic.ld

_codelen = SIZEOF(.text) + SIZEOF(.data) + SIZEOF(.ARM.exidx);

_ram_start = ORIGIN(ram);
_ram_end = ORIGIN(ram) + LENGTH(ram);
_stack = _ram_end - 8;
__stack_chk_guard = _ram_end - 8;
system_millis = _ram_end - 4;

_data_size = SIZEOF(.data);
