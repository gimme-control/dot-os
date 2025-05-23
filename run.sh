#!/bin/bash

export PATH=$PATH:/usr/local/i386elfgcc/bin

# Create binaries directory if it doesn't exist
mkdir -p Binaries

echo "Building bootloader..."
nasm "dot.asm" -f bin -o "Binaries/boot.bin"

echo "Building kernel entry..."
nasm "kernel.asm" -f elf -o "Binaries/kernel_entry.o"

echo "Building kernel..."
i386-elf-gcc -ffreestanding -m32 -g -c "kernel.cpp" -o "Binaries/kernel.o"

echo "Building VGA driver..."
i386-elf-gcc -ffreestanding -m32 -g -c "vga.cpp" -o "Binaries/vga.o"

echo "Creating padding..."
nasm "zeroes.asm" -f bin -o "Binaries/zeroes.bin"

echo "Linking kernel..."
i386-elf-ld -o "Binaries/full_kernel.bin" -Ttext 0x1000 "Binaries/kernel_entry.o" "Binaries/kernel.o" "Binaries/vga.o" --oformat binary

echo "Creating OS image..."
cat "Binaries/boot.bin" "Binaries/full_kernel.bin" "Binaries/zeroes.bin" > "Binaries/OS.bin"

echo "Checking file sizes..."
ls -la Binaries/

echo "Running QEMU..."
qemu-system-x86_64 -drive format=raw,file="Binaries/OS.bin",index=0,if=floppy -m 128M -no-reboot
