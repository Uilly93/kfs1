#ifndef KERNEL_H
#define KERNEL_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdarg.h>
#include "vga.h"


#define MAX_SCREEN 3

typedef struct {
	u_int16_t buffer[VGA_HEIGHT * VGA_WIDTH * sizeof(u_int16_t)];
	u_int8_t term_row;
	u_int8_t term_column;
	u_int8_t term_color;
	u_int8_t cursor_color;
	u_int16_t cursor_pos;
} screen_t;

extern u_int16_t* terminal_buffer;
extern u_int8_t current_screen;
extern screen_t screens[MAX_SCREEN];

static const char scancode_to_ascii[] = {
    0,   27,  '1', '2', '3',  '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r',  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
0,    'a', 's', 'd', 'f',  'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0,    '\\','z', 'x', 'c',  'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*',  0,   ' '
};

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void outb(unsigned short port, unsigned char val) {
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

int		ft_printf(const char *s, ...);
void	*memset(void *s, int c, size_t n);
void	*ft_memmove(void *dest, const void *src, size_t n);
void	*memcpy(void *dest, const void *src, size_t n);
size_t	strlen(const char *s);
void	enable_cursor(uint8_t cursor_start, uint8_t cursor_end);
void	disable_cursor();
void	update_cursor(uint8_t x, uint8_t y);
void	switch_screen(u_int8_t index);
void	screen_init(void);
void	set_terminal_color(uint8_t screen_index, vga_color bg, vga_color fg);
void	set_cursor_color(uint8_t screen_index, vga_color bg, vga_color fg);
void	backspace();
void	enter();
void	screen_putchar(char c);
void	cursor_up();
void	cursor_down();
void	cursor_left();
void	cursor_right();


#endif 
