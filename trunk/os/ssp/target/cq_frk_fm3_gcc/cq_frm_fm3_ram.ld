MEMORY
{
    FLASH (rx) : ORIGIN = 0x00000000 , LENGTH = 1M
    SRAM0 (rwx) : ORIGIN = 0x1fff0000 , LENGTH = 64K
    SRAM1 (rwx) : ORIGIN = 0x20000000 , LENGTH = 64K
}

PROVIDE(hardware_init_hook = 0);
PROVIDE(software_init_hook = 0);
PROVIDE(software_term_hook = 0);
STARTUP(start.o)

SECTIONS
{
    .text :
    {
        __text = .;
		KEEP(*(.vector))
        *(.text)
        *(.text.*)
    } > SRAM0
	_etext = .;
	PROVIDE(etext = .);

	.rodata :
	{
        *(.rodata)
        *(.rodata.*)
    } > SRAM0

	.ARM.extab   : { *(.ARM.extab* .gnu.linkonce.armextab.*) }
	__exidx_start = .;
	.ARM.exidx   : { *(.ARM.exidx* .gnu.linkonce.armexidx.*) }
	__exidx_end = .;

	__idata_start = .;
    .data : AT (__idata_start)
    {
		__data_start = .;
        *(vtable)
        *(.data)
        *(.data.*)
    } > SRAM0
	__idata_end = __idata_start + SIZEOF(.data);
	_edata = .;
	PROVIDE(edata = .);

	. = ALIGN(4);

    .bss :
    {
        __bss_start  = .;
        *(.bss)
        *(COMMON)
        . = ALIGN(4);
    } > SRAM0
	. = ALIGN(4);
	_end = .;
	__bss_end = .;
	PROVIDE(end = .);
}
