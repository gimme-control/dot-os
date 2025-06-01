#!/bin/bash

export PATH=$PATH:/usr/local/i386elfgcc/bin

# Create binaries directory if it doesn't exist
mkdir -p Binaries

nasm "boot.asm" -f bin -o "Binaries/boot.bin"
nasm "kernel.asm" -f elf -o "Binaries/kernel_entry.o"
nasm "idt_load.asm" -f elf -o "Binaries/idt_load.o"
nasm "kernel.asm" -f elf -o "Binaries/kernel_entry.o"

i386-elf-gcc -ffreestanding -m32 -g -c "kernel.cpp" -o "Binaries/kernel.o"
i386-elf-gcc -ffreestanding -m32 -g -c "idt.cpp" -o "Binaries/idt.o"
i386-elf-gcc -ffreestanding -m32 -g -c "pic.cpp" -o "Binaries/pic.o"
i386-elf-gcc -ffreestanding -m32 -g -c "vga.cpp" -o "Binaries/vga.o"


i386-elf-ld -o "Binaries/full_kernel.bin" -Ttext 0x1000 "Binaries/kernel_entry.o" "Binaries/kernel.o" "Binaries/idt_load.o" "Binaries/vga.o" "Binaries/idt.o"  "Binaries/pic.o" --oformat binary

cat "Binaries/boot.bin" "Binaries/full_kernel.bin" > "Binaries/OS.bin"

ls -la Binaries/

qemu-system-x86_64 -drive format=raw,file="Binaries/OS.bin",index=0,if=floppy -m 128M -no-reboot # (TODO) Add way to launch with a bigger res

# For logging SMM, interrupts or exceptions in qemy, use: -d int
