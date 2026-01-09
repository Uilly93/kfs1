OUTPUT_FORMAT(elf32-i386)
ENTRY(_start)

SECTIONS
{
	. = 1M; /* equivaut a 0x100000 reserve the first 1MB for VGA , BIOS, GRUB*/

	.text BLOCK(4K) : ALIGN(4K) /* reserve 4Kbyte for the section (4K is std for x86 pagination)*/
	{
		*(.multiboot) /* multiboot has to be first*/
		*(.text)
	}
	
	.rodata : {	*(.rodata) } /*section for const value*/
	
	.data :	{ *(.data) } /* section for initialized global value*/
	
	.bss : {			 /* section for uninitialized global value*/
		*(COMMON) /*where go the uninitialized global value created without static (compilator dependent and can be stop with -fno-common then all go in bss)*/
		*(.bss)
	}
}
