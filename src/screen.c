#include "kernel.h"

void screen_init(void) {
	screens[0].term_row = 0;	
	screens[0].term_column = 0;
	set_terminal_color(0, BLACK, LIGHT_GREY);
	set_cursor_color(0, LIGHT_GREY, BLACK);
	screens[0].cursor_pos = 0;
	for (uint16_t j = 0; j < VGA_LEN; j++)
	{
		screens[0].buffer[j] = vga_entry(' ', screens[0].term_color);
	}
	
	screens[1].term_row = 0;	
	screens[1].term_column = 0;
	set_terminal_color(1, LIGHT_GREY, BLACK);
	set_cursor_color(1, BLACK, LIGHT_GREY);
	screens[1].cursor_pos = 0;
	for (uint16_t j = 0; j < VGA_LEN; j++)
	{
		screens[1].buffer[j] = vga_entry(' ', screens[1].term_color);
	}

	screens[2].term_row = 0;	
	screens[2].term_column = 0;
	set_terminal_color(2, BLUE, LIGHT_GREY);
	set_cursor_color(2, LIGHT_GREY, BLUE);
	screens[2].cursor_pos = 0;
	for (uint16_t j = 0; j < VGA_LEN; j++)
	{
		screens[2].buffer[j] = vga_entry(' ', screens[2].term_color);
	}
}

void set_terminal_color(uint8_t screen_index, vga_color bg, vga_color fg) 
{
	screens[screen_index].term_color = (bg << 4) | fg;
}

void switch_screen(u_int8_t index) {
 	current_screen = index;
 	memset(terminal_buffer, ' ', VGA_LEN);
 	memcpy(terminal_buffer, &screens[current_screen].buffer, VGA_LEN);
 	update_cursor(screens[current_screen].term_column, screens[current_screen].term_row);
}

void screen_putchar(char c) 
{
	if (c == '\n')
	{
		enter();
		return;
	}
	const size_t index = screens[current_screen].term_row * VGA_WIDTH + screens[current_screen].term_column;
	screens[current_screen].buffer[index] = vga_entry(c, screens[current_screen].term_color);
	if (++screens[current_screen].term_column == VGA_WIDTH || c == '\n')
	{
		screens[current_screen].term_column = 0;
		if (++screens[current_screen].term_row == VGA_HEIGHT)
		{
			ft_memmove(screens[current_screen].buffer, screens[current_screen].buffer + 80, VGA_SCROLL);
			for (int x = 0; x < VGA_WIDTH; x++) {
				screens[current_screen].buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = (uint16_t)' ';
            }
			screens[current_screen].term_row = VGA_HEIGHT -1;
		}
	}
	update_cursor(screens[current_screen].term_column, screens[current_screen].term_row);
}
