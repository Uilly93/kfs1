#include "kernel.h"

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

int ft_strwcmp(char *s1, char *s2)
{
	int i = 0;
    while (s1[i] && s2[i] && s1[i] == s2[i] && s1[i] != ' ')
        i++;
    if (s2[i] == '\0' && (s1[i] == '\0' || s1[i] == ' '))
        return 0;
    return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}


uint8_t fill_color(char *cmd)
{
	ft_printf(-1, "arg = .%s.\n", cmd);
	if(*cmd == '\0') return 16;
    if (!ft_strwcmp(cmd, "black"))		   return BLACK;
    if (!ft_strwcmp(cmd, "blue"))          return BLUE;
    if (!ft_strwcmp(cmd, "green"))         return GREEN;
    if (!ft_strwcmp(cmd, "cyan"))          return CYAN;
    if (!ft_strwcmp(cmd, "red"))           return RED;
    if (!ft_strwcmp(cmd, "magenta"))       return MAGENTA;
    if (!ft_strwcmp(cmd, "brown"))         return BROWN;
    if (!ft_strwcmp(cmd, "light-grey"))    return LIGHT_GREY;
    if (!ft_strwcmp(cmd, "dark-grey"))     return DARK_GREY;
    if (!ft_strwcmp(cmd, "light-blue"))    return LIGHT_BLUE;
    if (!ft_strwcmp(cmd, "light-green"))   return LIGHT_GREEN;
    if (!ft_strwcmp(cmd, "light-cyan"))    return LIGHT_CYAN;
    if (!ft_strwcmp(cmd, "light-red"))     return LIGHT_RED;
    if (!ft_strwcmp(cmd, "light-magenta")) return LIGHT_MAGENTA;
    if (!ft_strwcmp(cmd, "light-brown"))   return LIGHT_BROWN;
    if (!ft_strwcmp(cmd, "white"))         return WHITE;
	ft_printf(-1, "no\n");
    return 16;
}

void change_color(char *cmd)
{
	if(!ft_strcmp(cmd, "setcolor"))
	{
		ft_printf(-1, "setcolor: 'background' 'foreground' 'prompt'\n");
		return ;
	}
	uint8_t bg = BLACK;
	uint8_t fg = WHITE;
	uint8_t pr = GREEN;
	int i = 0;
	while(cmd[i] && cmd[i] != ' ')
		i++;
	i++;
	bg = fill_color(cmd + i);
	if(bg == 16)
	{
		ft_printf(-1, "error: invalid background color: .%s.\n", cmd + i);
		return;
	}
	while(cmd[i] && cmd[i] != ' ')
		i++;
	i++;
	fg = fill_color(cmd + i);
	if(fg == 16)
	{
		ft_printf(-1, "error: invalid foreground color: .%s.\n", cmd + i);
		return;
	}
	while(cmd[i] && cmd[i] != ' ')
		i++;
	i++;
	pr = fill_color(cmd + i);
	if(pr == 16)
	{
		ft_printf(-1, "error: invalid prompt color: .%s.\n", cmd + i);
		return;
	}
	if(bg == fg || bg == pr)
	{
		ft_printf(-1, "error: invalid matching colors\n");
		return;
	}
	set_terminal_color(current_screen, bg, fg, pr);
}



void print_stack_cmd(char *cmd)
{
	if(!ft_strcmp(cmd, "printstack"))
	{
		ft_printf(-1, "you need on argument 'size' between 1 and 125 to print\n");
		return ;
	}
	int i = 0;
	while(cmd[i] && cmd[i] != ' ')
		i++;
	i++;
	int size = ft_atoi(cmd + i);
	if(size == 42000)
	{
		ft_printf(-1, "invalid arg: %s\n", cmd + i);
		return ;
	}
	print_stack(size);
}

void exec_cmd(char *cmd)
{
	// ft_printf(-1, "cmd = %s\n", cmd);
	if(!ft_strcmp(cmd, "clear"))
		clear();
	else if(!ft_strcmp(cmd, "ascii"))
		print_kfs_logo();
	else if(!ft_strncmp(cmd, "setcolor", 8))
		change_color(cmd);
	else if(!ft_strncmp(cmd, "printstack", 10))
		print_stack_cmd(cmd);
	else if (!ft_strcmp(cmd, "reboot"))
		outb(0x64, 0xFE);
	else if(!ft_strncmp(cmd, "resetcolor", 10))
		set_terminal_color(current_screen, BLACK, LIGHT_GREY, GREEN);
	else if(!ft_strncmp(cmd, "help", 4))
		ft_printf(-1, "try to use: clear, ascii, setcolor, printstack, reboot or resetcolors\n");
}