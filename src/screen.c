#include "kernel.h"
#include "vga.h"

void init_screen(int index)
{
	int old_screen = current_screen;
    current_screen = index;
	screens[index].term_row = 24;	
	screens[index].term_column = 7;
	set_terminal_color(index, BLACK, LIGHT_GREY, GREEN);
	set_cursor_color(index, LIGHT_GREY, BLACK);
	screens[index].cursor_pos = index;
	for (uint16_t j = 0; j < VGA_LEN; j++)
	{
		screens[index].buffer[j] = vga_entry(' ', screens[index].term_color);
	}
	char prompt[6] = "KFS2 >";
	for(int i = 0; i < 6 ;i++){
		screens[index].buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + i] = vga_entry(prompt[i], screens[index].prompt_color);
	}
	print_kfs_logo();
	ft_printf(0, "%d Kfs | screen: %d", 42, current_screen + 1);
	current_screen = old_screen;
 	update_cursor(screens[index].term_column, screens[index].term_row);
}

void screen_multi_init(void)
{
	// char prompt[6] = "KFS2 >";
	for(int i = 0; i < MAX_SCREEN; i++){
		init_screen(i);
	}
}

void set_terminal_color(uint8_t screen_index, vga_color bg, vga_color fg, vga_color prompt) 
{
    screens[screen_index].term_color = (bg << 4) | fg;
    screens[screen_index].prompt_color = (bg << 4) | prompt;
    for (int r = 0; r < VGA_HEIGHT; r++)
    {
        int row_start = r * VGA_WIDTH;
        char line_start[7] = {0};

        for (int j = 0; j < 6; j++) {
            line_start[j] = (char)(screens[screen_index].buffer[row_start + j] & 0xFF);
        }
        int is_prompt_line = (ft_strncmp(line_start, "KFS2 >", 6) == 0);
        for (int c = 0; c < VGA_WIDTH; c++)
        {
            int i = row_start + c;
            char entry = (char)(screens[screen_index].buffer[i] & 0xFF);
            if (is_prompt_line && c < 6)
                screens[screen_index].buffer[i] = vga_entry(entry, screens[screen_index].prompt_color);
            else
                screens[screen_index].buffer[i] = vga_entry(entry, screens[screen_index].term_color);
        }
    }
}

void switch_screen(u_int8_t index) {
 	current_screen = index;
 	memset(terminal_buffer, ' ', VGA_LEN);
 	memcpy(terminal_buffer, &screens[current_screen].buffer, VGA_LEN);
 	update_cursor(screens[current_screen].term_column, screens[current_screen].term_row);
}

void screen_putchar(char c, int16_t *idx) 
{
	const int TOP_LIMIT = 1;
	const int BOTTOM_LIMIT = 23;
	if (idx != NULL && *idx != -1) 
    {
        if (c == '\n') 
            *idx = ((*idx / 80) + 1) * 80;
        else {
            screens[current_screen].buffer[*idx] = vga_entry(c, screens[current_screen].term_color);
            (*idx)++;
        }
		if (*idx >= BOTTOM_LIMIT * VGA_WIDTH + VGA_WIDTH) {
            ft_memmove(screens[current_screen].buffer + (TOP_LIMIT * VGA_WIDTH), 
                       screens[current_screen].buffer + ((TOP_LIMIT + 1) * VGA_WIDTH), 
                       (BOTTOM_LIMIT - TOP_LIMIT) * VGA_WIDTH * 2);

            for (int x = 0; x < VGA_WIDTH; x++) {
                screens[current_screen].buffer[BOTTOM_LIMIT * VGA_WIDTH + x] = 
                    vga_entry(' ', screens[current_screen].term_color);
            }
            *idx -= 80; 
        }
        return;
    }
	if(c == '\n')
	{		
		enter();
		return;
		// ft_memmove(screens[current_screen].buffer, screens[current_screen].buffer + 80, VGA_SCROLL);
		// for (int x = 0; x < VGA_WIDTH; x++) {
		// 	screens[current_screen].buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = (uint16_t)' ';
		// }
		// screens[current_screen].term_column = 7;
		// screens[current_screen].term_row = 24;
		// update_cursor(screens[current_screen].term_column, screens[current_screen].term_row);
	}
	if (screens[current_screen].term_column == VGA_WIDTH - 1)
		return ;
	int16_t index = *idx;
	if(index == -1)
	{
		index = screens[current_screen].term_row * VGA_WIDTH + screens[current_screen].term_column;
		screens[current_screen].buffer[index] = vga_entry(c, screens[current_screen].term_color);
		screens[current_screen].term_column++;
		update_cursor(screens[current_screen].term_column, screens[current_screen].term_row);
		return;
	}
	index = *idx;
	screens[current_screen].buffer[index] = vga_entry(c, screens[current_screen].term_color);
	update_cursor(screens[current_screen].term_column, screens[current_screen].term_row);
	*idx += 1;
}
