#include "gdt.h"
#include "kernel.h"


gdt_entry *gdt = (gdt_entry *)GDT_ADRESS;

void init_gdt()
{
	//null descriptor
	gdt[0].low_limit = 0x0;
	gdt[0].low_base = 0x0;
	gdt[0].medium_base = 0x0;
	gdt[0].access_byte = 0x0;
	gdt[0].high_limit = 0x0;
	gdt[0].flag = 0x0;
	gdt[0].high_base = 0x0;
	//kernel mode code segment
	gdt[1].low_limit = 0xFFFF;
	gdt[1].low_base = 0x0;
	gdt[1].medium_base = 0x0;
	gdt[1].access_byte = 0x9A;
	gdt[1].high_limit = 0xF;
	gdt[1].flag = 0xC;
	gdt[1].high_base = 0x0;
	//kernel mode data segment 
	gdt[2].low_limit = 0xFFFF;
	gdt[2].low_base = 0x0;
	gdt[2].medium_base = 0x0;
	gdt[2].access_byte = 0x92;
	gdt[2].high_limit = 0xF;
	gdt[2].flag = 0xC;
	gdt[2].high_base = 0x0;
	//user mode code segment 
	gdt[3].low_limit = 0xFFFF;
	gdt[3].low_base = 0x0;
	gdt[3].medium_base = 0x0;
	gdt[3].access_byte = 0xFA;
	gdt[3].high_limit = 0xF;
	gdt[3].flag = 0xC;
	gdt[3].high_base = 0x0;
	//user mode data segment
	gdt[4].low_limit = 0xFFFF;
	gdt[4].low_base = 0x0;
	gdt[4].medium_base = 0x0;
	gdt[4].access_byte = 0xF2;
	gdt[4].high_limit = 0xF;
	gdt[4].flag = 0xC;
	gdt[4].high_base = 0x0;
}
