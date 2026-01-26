NAME        = kfs.iso
BIN         = kfs.bin
CC          = gcc
SRC_DIR     = src
OBJ_DIR     = .obj
CFLAGS      = -m32 -std=gnu99 -ffreestanding -Wall -Wextra -Werror -nostdlib -fno-builtin -fno-exceptions -fno-stack-protector -nodefaultlibs
ASFLAGS     = -f elf32 
SRCS_C      = kernel.c libft.c cursor.c screen.c printf.c gdt_init.c print_stack.c
SRCS_ASM 	= boot.s gdt.s 
OBJS        = $(addprefix $(OBJ_DIR)/,$(SRCS_C:%.c=%.o)) $(addprefix $(OBJ_DIR)/,$(SRCS_ASM:%.s=%.o))
RM          = rm -rf

all: $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.s
	@mkdir -p $(OBJ_DIR)
	@nasm $(ASFLAGS) $< -o $@


$(BIN): $(OBJS)
	gcc -m32 -T $(SRC_DIR)/linker.ld -o $(OBJ_DIR)/$(BIN) -ffreestanding -nostdlib $(OBJS)

$(NAME): $(BIN)
	@docker build -t iso_gen $(SRC_DIR)
	@docker run --rm -v "$$(pwd)":/os iso_gen

boot: $(NAME)
	qemu-system-i386 -cdrom $(NAME)

clean:
	$(RM) $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)
	docker system prune -a --volumes -f

re: fclean all
