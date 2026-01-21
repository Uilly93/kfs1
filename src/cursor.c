#include "kernel.h"
#include "vga.h"
#include <stddef.h>
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

	char prev_char = screens[current_screen].buffer[prev_pos] & 0xFF;  
	screens[current_screen].buffer[prev_pos] = vga_entry(prev_char, screens[current_screen].term_color);
	char new_char = screens[current_screen].buffer[pos] & 0xFF;  
	screens[current_screen].buffer[pos] = vga_entry(new_char, screens[current_screen].cursor_color);
}

void cursor_up(){ // vim mode
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
		update_cursor(screens[current_screen].term_column, screens[current_screen].term_row);
	}
}

void cursor_down() { // vim mode
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
		update_cursor(screens[current_screen].term_column, screens[current_screen].term_row);
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
		update_cursor(screens[current_screen].term_column, screens[current_screen].term_row);
}

void cursor_left() {
		if ((screens[current_screen].term_column == 0 && screens[current_screen].term_row == 0) || screens[current_screen].term_column == 7) // vim mode
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
		update_cursor(screens[current_screen].term_column, screens[current_screen].term_row);
}

void backspace() {
	const size_t index = screens[current_screen].term_row * VGA_WIDTH + screens[current_screen].term_column -1;
	if (index < VGA_LEN)
		screens[current_screen].buffer[index] = vga_entry((uint16_t)' ', screens[current_screen].term_color);
	if (screens[current_screen].term_column > 7 )
	{
		screens[current_screen].term_column--;
	}
	// VIM mode
	// else if (screens[current_screen].term_row > 7) {
	// 	screens[current_screen].term_row--;
	// 	screens[current_screen].term_column = VGA_WIDTH - 1;
	// 	while (screens[current_screen].term_column > 0 && 
    //       (screens[current_screen].buffer[screens[current_screen].term_row * 80 + screens[current_screen].term_column] & 0xFF) == ' '){
    //           screens[current_screen].term_column--;
    //       }
	// 	screens[current_screen].term_column++;
	// }
	// VIM mode
	update_cursor(screens[current_screen].term_column, screens[current_screen].term_row);
}

int isolate_cmd()
{
	// size_t start = COORD(7, 24);
	// size_t end = COORD(80, 24);
	int16_t start = (VGA_HEIGHT - 1) * VGA_WIDTH + 7;
    int16_t end = (VGA_HEIGHT - 1) * VGA_WIDTH + (VGA_WIDTH - 1);
    while (end >= start && (screens[current_screen].buffer[end] & 0xFF) == ' ') {
        end--;
    }
	if (end < start)
		return 0;
	return (end - start + 1);
}

void clear()
{
	int start = COORD(0, 1);
	int end = COORD(80, 23);
	while(start < end)
	{
		screens[current_screen].buffer[start] = vga_entry(' ', screens[current_screen].term_color);
		start++;
	}
}

void reboot()
{
    while (inb(0x64) & 0x02); 
    outb(0x64, 0xFE);
}

void echo(char *cmd)
{
	char arg[80] = {0};
	int cmd_len = isolate_cmd();
	int16_t start_idx = COORD(11, 24);
        for (int i = 0; i < cmd_len; i++) {
            arg[i] = (char)(screens[current_screen].buffer[start_idx + i] & 0xFF);
			// ft_printf(-1, " -> cmd len = %d, cmd = %s", cmd_len, cmd);
        }
	arg[cmd_len] = '\0';
	ft_printf(-1, "%s\n", arg);
}

void exec_cmd(char *cmd)
{
	if(!ft_strcmp(cmd, "clear"))
		clear();
	if(!ft_strcmp(cmd, "ascii"))
		print_kfs_logo();
	if (!ft_strcmp(cmd, "reboot")) {
		ft_printf(-1, "System is rebooting...\n");
		outb(0x64, 0xFE);
	}
	if(!ft_strncmp(cmd, "echo", 4))
		echo(cmd);
}

void enter()
{
	char cmd[81] = {0};
	int cmd_len = isolate_cmd();
	if (cmd_len > 0 && cmd_len < 80) {
        int16_t start_idx = COORD(7, 24);
        for (int i = 0; i < cmd_len; i++) {
            cmd[i] = (char)(screens[current_screen].buffer[start_idx + i] & 0xFF);
			// ft_printf(-1, " -> cmd len = %d, cmd = %s", cmd_len, cmd);
        }
        cmd[cmd_len] = '\0';
    }
	
	// update_cursor(7, VGA_HEIGHT - 1);
    if (screens[current_screen].term_row == VGA_HEIGHT -1) {
		uint16_t prev_pos = screens[current_screen].cursor_pos;
		uint16_t line_offset = VGA_WIDTH;
		// ft_printf(-1, "%d",isolate_cmd());
        char prev_char = screens[current_screen].buffer[prev_pos] & 0xFF;
		// ft_memmove(cmd, screens[current_screen].buffer + line_offset + 8, isolate_cmd());
        screens[current_screen].buffer[prev_pos] = vga_entry(prev_char, screens[current_screen].term_color);
		ft_memmove(screens[current_screen].buffer + line_offset, 
           screens[current_screen].buffer + (line_offset * 2),
           (VGA_HEIGHT - 2) * VGA_WIDTH * 2);
        for (int x = 0; x < VGA_WIDTH; x++) {
            screens[current_screen].buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = 
                vga_entry(' ', screens[current_screen].term_color);
        }
		update_cursor(7, VGA_HEIGHT - 1);
		char prompt[6] = "KFS2 >";
		for(int i = 0; i < 6 ;i++){
			screens[current_screen].buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + i] = vga_entry(prompt[i], LIGHT_BLUE);
	
		}
		screens[current_screen].term_column = 7;
		if(cmd_len > 0)
		{
			exec_cmd(cmd);
			// ft_printf(-1, " -> cmd len = %d, cmd = %s", cmd_len, cmd);
		}
		return;
    }
}
