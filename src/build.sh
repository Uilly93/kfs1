#! /bin/sh

apt-get update && apt-get install -y gcc-i686-linux-gnu binutils-i686-linux-gnu
i686-linux-gnu-as boot.s -o boot.o
i686-linux-gnu-gcc -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
i686-linux-gnu-gcc -T linker.ld -o myos.bin -ffreestanding -O2 -nostdlib -fno-builtin -fno-exceptions -fno-stack-protector -fno-rtti -nostdlib -nodefaultlibs boot.o kernel.o -lgcc
