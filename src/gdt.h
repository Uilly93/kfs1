#ifndef GDT_H
#define GDT_H


#include <stdint.h>
#define GDT_ADRESS 0x00000800
#define GDT_SIZE 5

//https://wiki.osdev.org/GDT_Tutorial
//https://wiki.osdev.org/Global_Descriptor_Table#Segment_Descriptor
typedef struct __attribute__((packed))gdt_entry {
	uint16_t low_limit;
	uint16_t low_base;
	uint8_t medium_base;
	uint8_t access_byte;
	uint8_t high_limit:4;
	uint8_t flag:4;
	uint8_t high_base;
} gdt_entry;

typedef struct __attribute__((packed))gdt_register {
	unsigned short size;
	unsigned int address;
} gdt_register;

extern gdt_entry *gdt_array;

void load_gdt(uint32_t gdt_ptr_address);
void init_gdt(void);

#endif
