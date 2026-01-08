#! /bin/sh

apt-get update && apt-get install -y gcc-i686-linux-gnu binutils-i686-linux-gnu
i686-linux-gnu-as src/boot.s -o boot.o
i686-linux-gnu-gcc -c src/kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -ffreestanding -O2 -nostdlib -fno-builtin -fno-exceptions -fno-stack-protector -nostdlib -nodefaultlibs -fno-pie
i686-linux-gnu-gcc -T src/linker.ld -o myos.bin -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc -no-pie
# i686-linux-gnu-objdump -h myos.bin