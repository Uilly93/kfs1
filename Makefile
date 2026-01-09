NAME        = kfs1.iso
BIN         = kfs1.bin
CC          = gcc
SRC_DIR     = src
OBJ_DIR     = .obj
CFLAGS      = -m32 -std=gnu99 -ffreestanding -O2 -Wall -Wextra -fno-pie -nostdlib
ASFLAGS     = -m32 -c
SRCS_C      = kernel.c
OBJS        = $(addprefix $(OBJ_DIR)/,$(SRCS_C:%.c=%.o)) $(OBJ_DIR)/boot.o
RM          = rm -rf

all: $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.s
	@mkdir -p $(OBJ_DIR)
	$(CC) $(ASFLAGS) $< -o $@


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