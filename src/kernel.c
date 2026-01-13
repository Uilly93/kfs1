#include "kernel.h"
#include "vga.h"
#include <stddef.h>
#include <stdint.h>


uint8_t current_screen = 0;
screen_t screens[MAX_SCREEN];
uint16_t* terminal_buffer = (uint16_t*)VGA_MEMORY;

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
				cursor_up();
				break;
			case 0x50:
				cursor_down();
				break; 
			case 0x4B:
				cursor_left();
				break;
			case 0x4D:
				cursor_right();
				break; 
		}
		// update_cursor(terminal_column, terminal_row);
	}
}

void kernel_main(void) 
{
	/* Initialize terminal interface */
	screen_init();

	terminal_write("Hello, kernel World!");
	while(0xcafe) {
		if (inb(0x64) & 1)
		{
			uint8_t scancode = inb(0x60);
			
			if (scancode & 0x80)
				continue;
			//handle_cursor(scancode);

			switch (scancode) {
				case 0x3B: //f1
					switch_screen(0);
					break;
				case 0x3C: //f2
					switch_screen(1);
					break;
				case 0x3D: //f3
					switch_screen(2);
					break;
			}
			if (scancode < sizeof(scancode_to_ascii))
			{
				uint8_t c = scancode_to_ascii[scancode];
				switch (c) {
					case 0: continue; 
					case 27: continue;
					case 8:
						backspace();
						memcpy(terminal_buffer, &screens[current_screen].buffer, VGA_LEN);
						continue; // backspace
					case 10: 
						enter();
						memcpy(terminal_buffer, &screens[current_screen].buffer, VGA_LEN);
						continue; // carriage return
					default: 
						screen_putchar(c);
						memcpy(terminal_buffer, &screens[current_screen].buffer, VGA_LEN);
				}
			}
		}
	}
}
