#ifndef KERNEL_H
#define KERNEL_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>
#include "vga.h"


#define MAX_SCREEN 3

typedef struct {
	u_int16_t buffer[VGA_HEIGHT * VGA_WIDTH];
	u_int8_t term_row;
	u_int8_t term_column;
	u_int8_t term_color;
	u_int8_t cursor_color;
	u_int16_t cursor_pos;
} screen_t;

extern u_int16_t* terminal_buffer;
extern u_int8_t current_screen;
extern screen_t screens[MAX_SCREEN];

void	*memset(void *s, int c, size_t n);
void	*ft_memmove(void *dest, const void *src, size_t n);
void	*memcpy(void *dest, const void *src, size_t n);
size_t	strlen(const char *s);
void	set_terminal_color(uint8_t screen_index, vga_color bg, vga_color fg);
void	set_cursor_color(uint8_t screen_index, vga_color bg, vga_color fg);

#endif 
