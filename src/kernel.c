#include "kernel.h"
#include "vga.h"
#include <stddef.h>
#include <stdint.h>


#define TO_CURSOR -1
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
			// case 0x48: 
			// 	cursor_up();
			// 	break;
			// case 0x50:
			// 	cursor_down();
			// 	break;
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

void print_kfs_logo() {
	// int16_t start = 80;
	// for(; start <= 800; start++)
	// 	screens[current_screen].buffer[start] = vga_entry(' ', screens[current_screen].term_color);
	ft_printf(-1,       "42  42  42424242  42424242\n");
	ft_printf(-1,       "42 42   42        42      \n");
	ft_printf(-1,       "4242     424242    424242 \n");
	ft_printf(-1,       "42 42   42              42\n");
	ft_printf(-1,       "42  42  42        42424242\n");
	ft_printf(-1, "System Kernel Loaded Successfully!\n");
}

// void clear()
// {
// 	// uint16_t start = 80;
// 	// for(; start <= COORD(80, 24); start++)
// 	// screens[current_screen].buffer[start] = vga_entry(' ', screens[current_screen].term_color);
// }

void kernel_main(void)
{
	/* Initialize terminal interface */
	screen_multi_init();

	//ft_printf("%c\ndjhfjdhf\n", 'o');
	//ft_printf("%d\n", 42);
	//ft_printf("%x\n", 42);
	//ft_printf("%X\n", 42);
	//ft_printf("%p\n", VGA_MEMORY);
	// print_kfs_logo();
	while(0xcafe) {
		memcpy(terminal_buffer, &screens[current_screen].buffer, VGA_LEN);
		if (inb(0x64) & 1)
		{
			uint8_t scancode = inb(0x60);
			
			handle_cursor(scancode);
			switch (scancode) {
				case 0x3B: //f1
					switch_screen(0);
					// ft_printf(0, "%d Kfs | screen: %d", 42, current_screen + 1);
	//				ft_printf("Hello screen %d \n", current_screen + 1);
					break;
				case 0x3C: //f2
					switch_screen(1);
					// ft_printf(0, "%d Kfs | screen: %d", 42, current_screen + 1);
	//				ft_printf("Hello screen %d \n", current_screen + 1);
					break;
				case 0x3D: //f3
					switch_screen(2);
					// ft_printf(0, "%d Kfs | screen: %d", 42, current_screen + 1);
	//				ft_printf("Hello screen %d \n", current_screen + 1);
					break;
			}
			if (scancode < sizeof(scancode_to_ascii))
			{
				int16_t to_cursor = -1;
				uint8_t c = scancode_to_ascii[scancode];
				switch (c) {
					case 0: continue; 
					case 27: continue;
					case 8:
						backspace();
						continue; // backspace
					case 10: 
						enter();
						continue; // carriage return
					default: 
						screen_putchar(c, &to_cursor);
				}
			}
		}
	}
}
