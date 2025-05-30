#!/bin/bash

export PATH=$PATH:/usr/local/i386elfgcc/bin

# Create binaries directory if it doesn't exist
mkdir -p Binaries

nasm "boot.asm" -f bin -o "Binaries/boot.bin"

nasm "kernel.asm" -f elf -o "Binaries/kernel_entry.o"

nasm "irq.asm" -f elf -o "Binaries/irq.o"

i386-elf-gcc -ffreestanding -m32 -g -c "kernel.cpp" -o "Binaries/kernel.o"

i386-elf-gcc -ffreestanding -m32 -g -c "vga.cpp" -o "Binaries/vga.o"

nasm "zeroes.asm" -f bin -o "Binaries/zeroes.bin"

i386-elf-ld -o "Binaries/full_kernel.bin" -Ttext 0x1000 "Binaries/kernel_entry.o" "Binaries/kernel.o" "Binaries/vga.o" "Binaries/irq.o"  --oformat binary

cat "Binaries/boot.bin" "Binaries/full_kernel.bin" "Binaries/zeroes.bin" > "Binaries/OS.bin"

ls -la Binaries/

qemu-system-x86_64 -drive format=raw,file="Binaries/OS.bin",index=0,if=floppy -m 128M -no-reboot # (TODO) Add way to launch with a bigger res
