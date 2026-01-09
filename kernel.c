#include "kernel.h"
#include "vga.h"
#include <stddef.h>
#include <stdint.h>


uint8_t current_screen = 0;
screen_t screens[MAX_SCREEN];
uint16_t* terminal_buffer = (uint16_t*)VGA_MEMORY;



void screen_init(void) {
	for (uint8_t i = 0; i < MAX_SCREEN; i++) {
		screens[i].term_row = 0;	
		screens[i].term_column = 0;
		set_terminal_color(i, BLACK, LIGHT_GREY);
		set_cursor_color(i, LIGHT_GREY, BLACK);
		screens[i].cursor_pos = 0;
		for (uint16_t j = 0; j < VGA_WIDTH * VGA_HEIGHT; j++)
		{
			screens[i].buffer[j] = vga_entry(' ', screens[i].term_color);
		}
		
	}
}

void set_cursor_color(uint8_t screen_index, vga_color bg, vga_color fg) 
{
	screens[screen_index].cursor_color = (bg << 4) | fg;
}

void set_terminal_color(uint8_t screen_index, vga_color bg, vga_color fg) 
{
	screens[screen_index].term_color = (bg << 4) | fg;
}

void screen_putchar(uint8_t scren_index, char c) 
{
	const size_t index = screens[scren_index].term_row * VGA_WIDTH + screens[scren_index].term_column;
	screens[scren_index].buffer[index] = vga_entry(c, screens[scren_index].term_color);
	if (++screens[scren_index].term_column == VGA_WIDTH) {
		screens[scren_index].term_column = 0;
		if (++screens[scren_index].term_row == VGA_HEIGHT)
			screens[scren_index].term_row = 0;
	}
}

void terminal_write(const char* data) 
{
	size_t len = strlen(data);
	for (size_t i = 0; i < len; i++)
		screen_putchar(current_screen, data[i]);
	memset(terminal_buffer, ' ', VGA_HEIGHT * VGA_WIDTH);
	memcpy(terminal_buffer, &screens[current_screen].buffer, VGA_HEIGHT * VGA_WIDTH);
}

void kernel_main(void) 
{
	/* Initialize terminal interface */
	screen_init();

	/* Newline support is left as an exercise. */
	terminal_write("Hello, kernel World!\n");
	current_screen++;
	set_terminal_color(current_screen,  WHITE, BLACK);
	terminal_write("Hello, screen 2\n");
	current_screen++;
	set_terminal_color(current_screen,  BLUE, RED);
	terminal_write("Hello, screen 3\n");
}
