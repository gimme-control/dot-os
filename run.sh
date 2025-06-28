#!/bin/bash

export PATH=/usr/local/i386elfgcc/bin:$PATH

# Create binaries directory if it doesn't exist
mkdir -p Binaries

nasm "boot.asm" -f bin -o "Binaries/boot.bin"
nasm "kernel.asm" -f elf -o "Binaries/kernel_entry.o"
nasm "idt_load.asm" -f elf -o "Binaries/idt_load.o"

i386-elf-gcc -ffreestanding -m32 -g -c "kernel.cpp" -o "Binaries/kernel.o"
i386-elf-gcc -ffreestanding -m32 -g -c "idt.cpp" -o "Binaries/idt.o"
i386-elf-gcc -ffreestanding -m32 -g -c "pic.cpp" -o "Binaries/pic.o"
i386-elf-gcc -ffreestanding -m32 -g -c "vga.cpp" -o "Binaries/vga.o"
i386-elf-gcc -ffreestanding -m32 -g -c "cbuff.cpp" -o "Binaries/cbuff.o"
i386-elf-gcc -ffreestanding -m32 -g -c "drivers/timer.cpp" -o "Binaries/timer.o"
i386-elf-gcc -ffreestanding -m32 -g -c "drivers/keyboard.cpp" -o "Binaries/keyboard.o"

i386-elf-ld -o "Binaries/full_kernel.elf" -g -Ttext 0x1000 \
    Binaries/kernel_entry.o \
    Binaries/kernel.o \
    Binaries/vga.o \
    Binaries/idt_load.o \
    Binaries/idt.o \
    Binaries/pic.o \
    Binaries/timer.o \
    Binaries/keyboard.o 

    # Binaries/cbuff.o \
    # Binaries/keyboard.o \ 

i386-elf-objcopy -O binary "Binaries/full_kernel.elf" "Binaries/full_kernel.bin"

cat "Binaries/boot.bin" "Binaries/full_kernel.bin" > "Binaries/OS.bin"

ls -la Binaries/

qemu-system-i386 -s -S -drive format=raw,file="Binaries/OS.bin",index=0,if=floppy -m 128M -no-reboot # (TODO) Add way to launch with a bigger res
# 

# For logging SMM, interrupts or exceptions in qemu, use: -d int
