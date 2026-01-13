#include "../includes/kernel.h"
#include <stddef.h>

/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
// #error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
// #error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

/* Hardware text mode color constants. */
enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

enum t_dir {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) 
{
	return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) 
{
	return (uint16_t) uc | (uint16_t) color << 8;
}

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer = (uint16_t*)VGA_MEMORY;
const unsigned char kbd_qwerty[] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0,
    ' ', 0
};

void terminal_initialize(void) 
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
	
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

void terminal_setcolor(uint8_t color) 
{
	terminal_color = color;
}

void update_cursor(int x, int y) {
    uint16_t pos = y * 80 + x;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) 
{
	const size_t index = y * VGA_WIDTH + x;
	if(isprintable(c)){
		terminal_buffer[index] = vga_entry(c, color);
		terminal_column++;
	}
}

void terminal_scroll(void){
	ft_memmove(terminal_buffer, terminal_buffer + 80, ft_strlen((char *)terminal_buffer + 80));
	unsigned short empty_char = (0x07 << 8) | ' ';
	for (int x = 0; x < VGA_WIDTH; x++) {
		terminal_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = empty_char;
	}
	terminal_row = VGA_HEIGHT - 1;
}

void terminal_putchar(char c) 
{
	terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
	if (terminal_column == VGA_WIDTH || c == '\n') {
		terminal_column = 0;
		terminal_row++;
		if (terminal_row == VGA_HEIGHT)
			terminal_scroll();
	}
	update_cursor(terminal_column, terminal_row);
}

void terminal_write(const char* data, size_t size) 
{
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

void terminal_writestring(const char* data) 
{
	terminal_write(data, ft_strlen(data));
}

void directionnal_cross_boundary(enum t_dir dir){
	if(dir == UP && terminal_row > 0){
		if(terminal_column > 0 && (terminal_buffer[(terminal_row - 1) * 80 + terminal_column] & 0xFF) != ' '){
			terminal_row--;
		}
		else{
			terminal_row--;
			terminal_column = 79;
			while (terminal_column > 0 && (terminal_buffer[terminal_row * 80 + terminal_column] & 0xFF) == ' '){
				terminal_column--;
			}
			if((terminal_buffer[terminal_row * 80 + terminal_column] & 0xFF) != ' ')
				terminal_column++;
		}
	}
	else if(dir == DOWN && terminal_row < 24){
		if(terminal_column > 0 && (terminal_buffer[(terminal_row + 1) * 80 + terminal_column] & 0xFF) != ' '){
			terminal_row++;
		}
		else{
			terminal_row++;
			terminal_column = 79;
			while (terminal_column > 0 && (terminal_buffer[terminal_row * 80 + terminal_column] & 0xFF) == ' '){
				terminal_column--;
			}
			if((terminal_buffer[terminal_row * 80 + terminal_column] & 0xFF) != ' ')
				terminal_column++;
		}
	}
	else if (dir == RIGHT){
		size_t boundary = 79;
		while (boundary > 0 && (terminal_buffer[terminal_row * 80 + boundary] & 0xFF) == ' ') {
				boundary--;
		}
		if (terminal_column <= boundary && terminal_column < 79) {
				terminal_column++;
		}
	}
	else if (dir == LEFT){
		if (terminal_column == 0 && terminal_row == 0)
			return;
		if(terminal_column == 0 && terminal_row > 0){
			terminal_row--;
			terminal_column = 79;
			while (terminal_column > 0 && (terminal_buffer[terminal_row * 80 + terminal_column] & 0xFF) == ' '){
				terminal_column--;
			}
			terminal_column++;
		}
		else
			terminal_column--;
	}
}

void escape_whitespaces(){
	if (terminal_column == 0 && terminal_row == 0)
		return;
	else if(terminal_column > 0)
		terminal_column -= 1;
	else if (terminal_column == 0 && terminal_row > 0){
		terminal_row -= 1;
		terminal_column = 79;
		while (terminal_column > 0 && (terminal_buffer[terminal_row * 80 + terminal_column] & 0xFF) == ' '){
			terminal_column--;
		}
		terminal_column++;
	}
}

void handle_backscape(unsigned char c)
{
	if(c == 0x0E)
	{
		escape_whitespaces();
		uint16_t *location = terminal_buffer + (terminal_row * 80 + terminal_column);
		*location = (uint16_t) (0x07 << 8) | ' ';
		update_cursor(terminal_column, terminal_row);
	}
}

static int is_extended = 0;

void handle_cursor(unsigned char c){
	if(c == 0xE0){
		is_extended = 1;
		return;
	}
	if(is_extended)
	{
		is_extended = 0;
		switch (c) {
			case 0x48: 
				directionnal_cross_boundary(UP);
				break; // up
			case 0x50:
				directionnal_cross_boundary(DOWN);
				break; // down
			case 0x4B:
				directionnal_cross_boundary(LEFT);
				break; // left
			case 0x4D:
				directionnal_cross_boundary(RIGHT);
				break; // right
		}
		update_cursor(terminal_column, terminal_row);
	}
}

void handle_typing(unsigned char c){
	if(!(c & 0x80) && c < sizeof(kbd_qwerty))
		terminal_putchar(kbd_qwerty[c]);
}

void terminal_write_inputs(void){
	if(inb(0x64) & 1){
		const unsigned char c = inb(0x60);
		handle_backscape(c);
		handle_cursor(c);
		handle_typing(c);
	}
}

void kernel_main(void) 
{
	/* Initialize terminal interface */
	terminal_initialize();
	/* Newline support is left as an exercise. */
	terminal_writestring("42\n");
	while(1){
		terminal_write_inputs();
	}
}