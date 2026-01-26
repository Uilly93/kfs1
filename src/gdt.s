bits 32 

;https://ordoflammae.github.io/littleosbook/#the-global-descriptor-table-gdt

section .text
global load_gdt

load_gdt:
    mov eax, [esp + 4]    ; get parameter adress
    lgdt [eax]            ; load GDT info in GDT register 
    
    mov ax, 0x10          ; define offset segment selector and load it for segment regsister
	;
	;file:///home/vstineau/Downloads/325462-089-sdm-vol-1-2abcd-3abcd-4.pdf chapter 3.4.2
	;https://wiki.osdev.org/Segmentation
	; data segment
    mov ds, ax 
    mov es, ax
    mov fs, ax
    mov gs, ax
	; stack segment
    mov ss, ax
    
    ; far jump to reload code segment (CS)
    jmp 0x08:.reload_cs   ; 0x08 = kernel code segment offset (index 1)
    
.reload_cs:
    ret

