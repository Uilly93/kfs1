IMG = myos.bin
ASM_SRC = boot.s
C_SRC = kernel.c libft/memcpy.c libft/memmove.c libft/strlen.c libft/memset.c
CFLAGS = -m32  -c -fno-builtin -fno-stack-protector -nostdlib -nodefaultlibs -Wall -Werror -Wextra
OBJ = boot.o kernel.o libft/memcpy.o libft/memmove.o libft/strlen.o libft/memset.o

all: $(IMG)

$(IMG): $(OBJ)
	gcc -m32 -T linker.ld -o $(IMG) -ffreestanding -nostdlib $(OBJ)

boot.o: boot.s
	nasm -f elf32 boot.s -o boot.o

kernel.o: kernel.c
	gcc $(CFLAGS) kernel.c -o kernel.o

libft/memcpy.o: libft/memcpy.c
	gcc $(CFLAGS) libft/memcpy.c -o libft/memcpy.o

libft/memmove.o: libft/memmove.c
	gcc $(CFLAGS) libft/memmove.c -o libft/memmove.o

libft/strlen.o: libft/strlen.c
	gcc $(CFLAGS) libft/strlen.c -o libft/strlen.o

libft/memset.o: libft/memset.c
	gcc $(CFLAGS) libft/memset.c -o libft/memset.o

clean: 
	rm -f $(OBJ) $(IMG)

run: $(IMG)
	qemu-system-i386 -kernel $(IMG)

.PHONY: all clean run
