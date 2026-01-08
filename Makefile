# #commande a lancer pour creer myos.bin
# docker run --rm -v "$(pwd)":/root/env -w /root/env debian:latest sh -c "chmod +x build.sh && ./build.sh"
# #lancer l'os
# qemu-system-i386 -kernel myos.bin
#NASM
#gcc-m32
# NAME = myos.iso
# CC = gcc
# SRC_DIR = src
# OBJ_DIR = obj
# SRC_C = kernel.c
# AS = boot
# NASM_BOOT = $(SRC_DIR)/$(AS_SRC).s -o $(OBJ_DIR)/$(AS).o
# SRC_LD = linker.ld
# OBJS = $(addprefix $(OBJ_DIR)/,$(SRC_NAME:%.c=%.o))
# SRC = $(addprefix $(SRC_DIR)/,$(SRC_NAME))
# CFLAGS = -m32 -std=gnu99 -ffreestanding -O2 -Wall -Wextra \
# 		-ffreestanding -O2 -nostdlib -fno-builtin \
# 		-fno-exceptions -fno-stack-protector -nostdlib \
# 		-nodefaultlibs -fno-pie
# RM = rm -rf

# all: $(NAME)

# $(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
# 	@mkdir -p $(OBJ_DIR)
# 	$(CC) $(CFLAGS) -c $< -o $@

# $(NAME): $(OBJS)
# 	$(NASM_BOOT)
# 	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)
# 	gcc -m32 -T $(SRC_DIR)/linker.ld -o myos.bin -ffreestanding -O2 -nostdlib $(OBJ_DIR)/boot.o $(OBJ_DIR)/kernel.o -lgcc -no-pie
# 	docker build -t iso_gen $(SRC_DIR)
# 	docker run --rm -v "$(pwd)":/os iso_gen

# clean: 
# 	$(RM) .obj

# fclean: clean
# 	$(RM) src/myos.bin

NAME        = kfs1.iso
BIN         = kfs1.bin
CC          = gcc
AS          = gcc
SRC_DIR     = src
OBJ_DIR     = .obj

# Flags de compilation (ajout de -fno-pie pour éviter les erreurs d'adressage)
CFLAGS      = -m32 -std=gnu99 -ffreestanding -O2 -Wall -Wextra -fno-pie -nostdlib
ASFLAGS     = -m32 -c
LDFLAGS     = -m32 -T -lgcc $(SRC_DIR)/linker.ld -ffreestanding -O2 -nostdlib -lgcc -no-pie

# Fichiers
SRCS_C      = $(SRC_DIR)/kernel.c
SRCS_S      = $(SRC_DIR)/boot.s
OBJS        = $(OBJ_DIR)/kernel.o $(OBJ_DIR)/boot.o

RM          = rm -rf

all: $(NAME)

# 1. Compilation du binaire kernel
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.s
	@mkdir -p $(OBJ_DIR)
	$(AS) $(ASFLAGS) $< -o $@

$(BIN): $(OBJS)
	@docker run --rm -v "$$(pwd)"\
	:/root/env -w /root/env debian:latest \
	sh -c "apt-get update && apt-get install -y gcc-i686-linux-gnu binutils-i686-linux-gnu &&\
	i686-linux-gnu-gcc -T src/linker.ld -o kfs1.bin \
	-ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc -no-pie\
	"

# 2. Création de l'ISO via Docker (on réutilise l'image iso_gen)
$(NAME): $(BIN)
	@docker build -t iso_gen $(SRC_DIR)
	@docker run --rm -v "$$(pwd)":/os iso_gen

boot: $(NAME)
	qemu-system-i386 -cdrom $(NAME)

clean:
	$(RM) $(OBJ_DIR)
	docker system prune -a --volumes -f

fclean: clean
	$(RM) $(BIN) $(NAME)

re: fclean all