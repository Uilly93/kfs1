bits 32

section .multiboot
align 4
	dd 0x1BADB002			;define magic number for multiboot standard
	dd 0x0					;define multiboot flags here no flags needed
	dd -(0x1BADB002 + 0x0)

section .bss
align 16
stack_bottom:
resb 16384					;reserve 16 KiB for stack
stack_top:

section .text
global _start
extern kernel_main

_start:
	mov esp, stack_top

	call kernel_main		;call function kernel_main defined in kernel.c

	cli						;clear interrupt because at this time kernel is not ready to handle it
.hang:	hlt					;stop cpu if kernel_main return or if an interrupt is caught (should not happend)
	jmp .hang
.end:
