#include "kernel.h"

void print_stack(uint8_t size) 
{
	if (size > 125 || size == 0)
		size = 125;
	uint32_t esp; //ge get stack pointer adress
	__asm__ volatile("mov %%esp, %0" : "=r"(esp));

	uint32_t *stack = &esp; 
	ft_printf(-1,"stack pointer adress: %X\n", &esp);

	for (int i = 0; i < size; i++)
	{
		ft_printf(-1, "0x%x:0x%X ", &stack[i], stack[i]);
		if( i % 3 == 0)
			ft_printf(-1, "\n");
	}
}
