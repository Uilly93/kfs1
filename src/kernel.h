#ifndef KERNEL_H
#define KERNEL_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include "vga.h"
#include "gdt.h"
#define COORD(x, y) ((y) * 80 + (x))


#define MAX_SCREEN 3

typedef struct {
	uint16_t buffer[VGA_HEIGHT * VGA_WIDTH * sizeof(uint16_t)];
	uint8_t term_row;
	uint8_t term_column;
	uint8_t term_color;
    uint8_t prompt_color;
	uint8_t cursor_color;
	uint16_t cursor_pos;
} screen_t;

extern uint16_t* terminal_buffer;
extern uint8_t current_screen;
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

int		ft_printf(int16_t, const char *s, ...);
void	print_stack(uint8_t size);
void	*memset(void *s, int c, size_t n);
int     ft_strcmp(char *s1, char *s2);
int     ft_strncmp(const char *s1, const char *s2, size_t n);
void	*ft_memmove(void *dest, const void *src, size_t n);
void	*memcpy(void *dest, const void *src, size_t n);
size_t	ft_strlen(const char *s);
int	    ft_atoi(const char *nptr);
void	enable_cursor(uint8_t cursor_start, uint8_t cursor_end);
void	disable_cursor();
void	update_cursor(uint8_t x, uint8_t y);
void	switch_screen(uint8_t index);
void    screen_multi_init(void);
void	set_terminal_color(uint8_t screen_index, vga_color bg, vga_color fg, vga_color prompt);
void	set_cursor_color(uint8_t screen_index, vga_color bg, vga_color fg);
void	backspace();
void	enter();
void	screen_putchar(char c, int16_t *idx);
void	cursor_up();
void	cursor_down();
void	cursor_left();
void	cursor_right();
void    print_kfs_logo();
void    clear();
void    reboot();
int     isolate_cmd();
int     ft_strwcmp(char *s1, char *s2);
uint8_t fill_color(char *cmd);
void    change_color(char *cmd);
void    print_stack_cmd(char *cmd);
void    exec_cmd(char *cmd);

#endif 
