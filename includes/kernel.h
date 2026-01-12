#ifndef KERNEL_H
#define KERNEL_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000

static inline unsigned char inb(unsigned short port)
{
	unsigned char value;
	asm volatile("inb %1, %0" : "=a"(value) : "Nd"(port));
	return value;
}
static inline void outb(unsigned short port, unsigned char val) {
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}
void *ft_memcpy(void *dest, const void *src, size_t n);
void *ft_memmove(void *dest, const void *src, size_t n);
size_t ft_strlen(const char *str);
int isprintable(char c);

#endif