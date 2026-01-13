#include "kernel.h"
#include "vga.h"
#include <stdint.h>
#include <sys/types.h>

void set_cursor_color(uint8_t screen_index, vga_color bg, vga_color fg) 
{
	screens[screen_index].cursor_color = (bg << 4) | fg;
}

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end)
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);

	outb(0x3D4, 0x0B);
	outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}

void disable_cursor()
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
}

void update_cursor(uint8_t x, uint8_t y)
{
	u_int16_t prev_pos = screens[current_screen].cursor_pos;
	u_int16_t pos = y * VGA_WIDTH + x;
	screens[current_screen].cursor_pos = pos;

	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (screens[current_screen].cursor_pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((screens[current_screen].cursor_pos >> 8) & 0xFF));

	if (prev_pos != pos) {
		char prev_char = screens[current_screen].buffer[prev_pos] & 0xFF;  
		screens[current_screen].buffer[prev_pos] = vga_entry(prev_char, screens[current_screen].term_color);
	}
	char new_char = screens[current_screen].buffer[pos] & 0xFF;  
	screens[current_screen].buffer[pos] = vga_entry(new_char, screens[current_screen].cursor_color);
}

void cursor_up(){
	if (screens[current_screen].term_row > 0){
		if(screens[current_screen].term_column > 0 && (terminal_buffer[(screens[current_screen].term_row - 1) * 80 + screens[current_screen].term_column] & 0xFF) != ' '){
			screens[current_screen].term_row--;
		}
		else {
			screens[current_screen].term_row--;
			screens[current_screen].term_column = 79;
			while (screens[current_screen].term_column > 0 && (terminal_buffer[screens[current_screen].term_row * 80 + screens[current_screen].term_column] & 0xFF) == ' '){
				screens[current_screen].term_column--;
			}
			if((terminal_buffer[screens[current_screen].term_row * 80 + screens[current_screen].term_column] & 0xFF) != ' ')
				screens[current_screen].term_column++;
		}
	}
}

void cursor_down() {
	if (screens[current_screen].term_row < 24) {
		if(screens[current_screen].term_column > 0 && (terminal_buffer[(screens[current_screen].term_row + 1) * 80 + screens[current_screen].term_column] & 0xFF) != ' '){
			screens[current_screen].term_row++;
		}
		else{
			screens[current_screen].term_row++;
			screens[current_screen].term_column = 79;
			while (screens[current_screen].term_column > 0 && (terminal_buffer[screens[current_screen].term_row * 80 + screens[current_screen].term_column] & 0xFF) == ' '){
				screens[current_screen].term_column--;
			}
			if((terminal_buffer[screens[current_screen].term_row * 80 + screens[current_screen].term_column] & 0xFF) != ' ')
				screens[current_screen].term_column++;
		}
	}
}

void cursor_right() {
		size_t boundary = 79;
		while (boundary > 0 && (terminal_buffer[screens[current_screen].term_row * 80 + boundary] & 0xFF) == ' ') {
				boundary--;
		}
		if((terminal_buffer[screens[current_screen].term_row * 80 + boundary] & 0xFF) != ' ')
			boundary++;
		if (screens[current_screen].term_column < boundary && screens[current_screen].term_column < 79) {
				screens[current_screen].term_column++;
		}
}

void cursor_left() {
		if (screens[current_screen].term_column == 0 && screens[current_screen].term_row == 0)
			return;
		if(screens[current_screen].term_column == 0 && screens[current_screen].term_row > 0){
			screens[current_screen].term_row--;
			screens[current_screen].term_column = 79;
			while (screens[current_screen].term_column > 0 && (terminal_buffer[screens[current_screen].term_row * 80 + screens[current_screen].term_column] & 0xFF) == ' ') {
				screens[current_screen].term_column--;
			}
			screens[current_screen].term_column++;
		}
		else
			screens[current_screen].term_column--;
}

void backspace() {
	const size_t index = screens[current_screen].term_row * VGA_WIDTH + screens[current_screen].term_column -1;
	if (index < VGA_LEN)
		screens[current_screen].buffer[index] = vga_entry((uint16_t)' ', screens[current_screen].term_color);
	if (screens[current_screen].term_column > 0 )
	{
		screens[current_screen].term_column--;
	}
	else if (screens[current_screen].term_row > 0) {
		screens[current_screen].term_row--;
		screens[current_screen].term_column = VGA_WIDTH - 1;
		while (screens[current_screen].term_column > 0 && 
          (screens[current_screen].buffer[screens[current_screen].term_row * 80 + screens[current_screen].term_column] & 0xFF) == ' '){
              screens[current_screen].term_column--;
          }
		screens[current_screen].term_column++;
	}
	update_cursor(screens[current_screen].term_column, screens[current_screen].term_row);
}

void enter() 
{
    uint8_t next_row = screens[current_screen].term_row + 1;
    
    if (next_row >= VGA_HEIGHT) {
		uint16_t prev_pos = screens[current_screen].cursor_pos;
        char prev_char = screens[current_screen].buffer[prev_pos] & 0xFF;
        screens[current_screen].buffer[prev_pos] = vga_entry(prev_char, screens[current_screen].term_color);
        ft_memmove(screens[current_screen].buffer, screens[current_screen].buffer + VGA_WIDTH, VGA_SCROLL);
        
        for (int x = 0; x < VGA_WIDTH; x++) {
            screens[current_screen].buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = 
                vga_entry(' ', screens[current_screen].term_color);
        }
        next_row = VGA_HEIGHT - 1;
    }
    update_cursor(0, next_row);
    screens[current_screen].term_column = 0;
    screens[current_screen].term_row = next_row;
}
