#include "kernel.h"
#include "vga.h"
#include <stddef.h>
#include <stdint.h>

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
	uint16_t prev_pos = screens[current_screen].cursor_pos;
	uint16_t pos = y * VGA_WIDTH + x;
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
		update_cursor(screens[current_screen].term_column, screens[current_screen].term_row);
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
	update_cursor(screens[current_screen].term_column, screens[current_screen].term_row);
}

// int isolate_cmd()
// {
// 	// size_t start = COORD(7, 24);
// 	// size_t end = COORD(80, 24);
// 	int16_t start = (VGA_HEIGHT - 1) * VGA_WIDTH + 7;
//     int16_t end = (VGA_HEIGHT - 1) * VGA_WIDTH + (VGA_WIDTH - 1);
//     while (end >= start && (screens[current_screen].buffer[end] & 0xFF) == ' ') {
//         end--;
//     }
// 	if (end < start)
// 		return 0;
// 	return (end - start + 1);
// }

// void clear()
// {
// 	int start = COORD(0, 1);
// 	int end = COORD(80, 23);
// 	while(start < end)
// 	{
// 		screens[current_screen].buffer[start] = vga_entry(' ', screens[current_screen].term_color);
// 		start++;
// 	}
// }

// void reboot()
// {
//     while (inb(0x64) & 0x02); 
//     outb(0x64, 0xFE);
// }

// int ft_strwcmp(char *s1, char *s2)
// {
// 	int i = 0;
//     while (s1[i] && s2[i] && s1[i] == s2[i] && s1[i] != ' ')
//         i++;
//     if (s2[i] == '\0' && (s1[i] == '\0' || s1[i] == ' '))
//         return 0;
//     return ((unsigned char)s1[i] - (unsigned char)s2[i]);
// }


// uint8_t fill_color(char *cmd)
// {
// 	ft_printf(-1, "arg = .%s.\n", cmd);
// 	if(*cmd == '\0') return 16;
//     if (!ft_strwcmp(cmd, "black"))		   return BLACK;
//     if (!ft_strwcmp(cmd, "blue"))          return BLUE;
//     if (!ft_strwcmp(cmd, "green"))         return GREEN;
//     if (!ft_strwcmp(cmd, "cyan"))          return CYAN;
//     if (!ft_strwcmp(cmd, "red"))           return RED;
//     if (!ft_strwcmp(cmd, "magenta"))       return MAGENTA;
//     if (!ft_strwcmp(cmd, "brown"))         return BROWN;
//     if (!ft_strwcmp(cmd, "light-grey"))    return LIGHT_GREY;
//     if (!ft_strwcmp(cmd, "dark-grey"))     return DARK_GREY;
//     if (!ft_strwcmp(cmd, "light-blue"))    return LIGHT_BLUE;
//     if (!ft_strwcmp(cmd, "light-green"))   return LIGHT_GREEN;
//     if (!ft_strwcmp(cmd, "light-cyan"))    return LIGHT_CYAN;
//     if (!ft_strwcmp(cmd, "light-red"))     return LIGHT_RED;
//     if (!ft_strwcmp(cmd, "light-magenta")) return LIGHT_MAGENTA;
//     if (!ft_strwcmp(cmd, "light-brown"))   return LIGHT_BROWN;
//     if (!ft_strwcmp(cmd, "white"))         return WHITE;
// 	ft_printf(-1, "no\n");
//     return 16;
// }

// void change_color(char *cmd)
// {
// 	if(!ft_strcmp(cmd, "setcolor"))
// 	{
// 		ft_printf(-1, "setcolor: 'background' 'foreground' 'prompt'\n");
// 		return ;
// 	}
// 	uint8_t bg = BLACK;
// 	uint8_t fg = WHITE;
// 	uint8_t pr = GREEN;
// 	int i = 0;
// 	while(cmd[i] && cmd[i] != ' ')
// 		i++;
// 	i++;
// 	bg = fill_color(cmd + i);
// 	if(bg == 16)
// 	{
// 		ft_printf(-1, "error: invalid background color: .%s.\n", cmd + i);
// 		return;
// 	}
// 	while(cmd[i] && cmd[i] != ' ')
// 		i++;
// 	i++;
// 	fg = fill_color(cmd + i);
// 	if(fg == 16)
// 	{
// 		ft_printf(-1, "error: invalid foreground color: .%s.\n", cmd + i);
// 		return;
// 	}
// 	while(cmd[i] && cmd[i] != ' ')
// 		i++;
// 	i++;
// 	pr = fill_color(cmd + i);
// 	if(pr == 16)
// 	{
// 		ft_printf(-1, "error: invalid prompt color: .%s.\n", cmd + i);
// 		return;
// 	}
// 	if(bg == fg || bg == pr)
// 	{
// 		ft_printf(-1, "error: invalid matching colors\n");
// 		return;
// 	}
// 	set_terminal_color(current_screen, bg, fg, pr);
// }



// void print_stack_cmd(char *cmd)
// {
// 	if(!ft_strcmp(cmd, "printstack"))
// 	{
// 		ft_printf(-1, "you need on argument 'size' between 1 and 125 to print\n");
// 		return ;
// 	}
// 	int i = 0;
// 	while(cmd[i] && cmd[i] != ' ')
// 		i++;
// 	i++;
// 	int size = ft_atoi(cmd + i);
// 	if(size == 42000)
// 	{
// 		ft_printf(-1, "invalid arg: %s\n", cmd + i);
// 		return ;
// 	}
// 	print_stack(size);
// }

// void exec_cmd(char *cmd)
// {
// 	// ft_printf(-1, "cmd = %s\n", cmd);
// 	if(!ft_strcmp(cmd, "clear"))
// 		clear();
// 	else if(!ft_strcmp(cmd, "ascii"))
// 		print_kfs_logo();
// 	else if(!ft_strncmp(cmd, "setcolor", 8))
// 		change_color(cmd);
// 	else if(!ft_strncmp(cmd, "printstack", 10))
// 		print_stack_cmd(cmd);
// 	else if (!ft_strcmp(cmd, "reboot"))
// 		outb(0x64, 0xFE);
// 	else if(!ft_strncmp(cmd, "resetcolor", 10))
// 		set_terminal_color(current_screen, BLACK, LIGHT_GREY, GREEN);
// 	else if(!ft_strncmp(cmd, "help", 4))
// 		ft_printf(-1, "try to use: clear, ascii, setcolor, printstack, reboot or resetcolors\n");
// }

void display_prompt()
{
	update_cursor(7, VGA_HEIGHT - 1);
	char prompt[6] = "KFS2 >";
	for(int i = 0; i < 6 ;i++){
		screens[current_screen].buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + i] = vga_entry(prompt[i], screens[current_screen].prompt_color);
	}
	screens[current_screen].term_column = 7;
}

void enter()
{
	char cmd[81] = {0};
	int cmd_len = isolate_cmd();
	if (cmd_len > 0 && cmd_len < 80) {
        int16_t start_idx = COORD(7, 24);
        for (int i = 0; i < cmd_len; i++) {
            cmd[i] = (char)(screens[current_screen].buffer[start_idx + i] & 0xFF);
        }
		// ft_printf(-1, " -> cmd len = %d, cmd = %s", cmd_len, cmd);
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
			screens[current_screen].buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + i] = vga_entry(prompt[i], screens[current_screen].prompt_color);
		}
		screens[current_screen].term_column = 7;
		if(cmd_len > 0 && *cmd != '\0')
		{
			exec_cmd(cmd);
			// ft_printf(-1, " -> cmd len = %d, cmd = %s", cmd_len, cmd);
		}
		update_cursor(7, VGA_HEIGHT - 1);
		return;
    }
}
