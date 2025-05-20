bits 16 ; 16 bit mode because bootloader runs in real mode
org 0x7c00 ; Specifies where in memory the program starts
KERNEL_LOCATION equ 0x1000 ; Where the kernel is in memory

section .text

start:
    ; dl register stores boot disk value 
    mov [BOOT_DISK], dl 

    ; init registers
    xor ax, ax
    mov es, ax
    mov ds, ax
    mov bp, 0x8000
    mov sp, bp

    mov si, my_string

print_loop:
    lodsb
    or al, al
    jz stage_1
    mov ah, 0x0e
    int 0x10
    jmp print_loop

stage_1: 
    mov bx, KERNEL_LOCATION 
    mov dh, 2

    mov ah, 0x02 
    mov al, dh 
    mov ch, 0x00
    mov dh, 0x00 
    mov cl, 0x02 
    mov dl, [BOOT_DISK]

    int 0x13

    ; Video Mode (optional but generally mode)
    mov ah, 0x0 
    mov al, 0x3 
    int 0x10 

stage_2: 
    CODE_SEG equ GDT_code - GDT_start 
    DATA_SEG equ GDT_data - GDT_start 

    cli 
    lgdt [GDT_descriptor]

    mov eax, cr0 ; cr0 contains the settings
    or eax, 1 ; setting protected mode by flipping lowest bit
    mov cr0, eax 

    jmp CODE_SEG:start_protected_mode
    jmp $ ; placeholder in case above instruction fails


BOOT_DISK: db 0 

GDT_start:
    GDT_null:
        dd 0x0
        dd 0x0

    GDT_code: 
        dw 0xffff
        dw 0x0
        dw 0x0
        db 0b10011010
        db 0b11001111
        db 0x0 

    GDT_data: 
        dw 0xffff
        dw 0x0
        db 0x0
        db 0b10010010 
        db 0b11001111
        db 0x0 
GDT_end: 

GDT_descriptor: 
    dw GDT_end - GDT_start - 1
    dd GDT_start 

[bits 32]

start_protected_mode: 
    mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax 
    mov es, ax
    mov fs, ax
    mov gs, ax 

    mov ebp, 0x90000
    mov esp, ebp 

    jmp KERNEL_LOCATION

my_string: db "Bootloader starting", 0

times 510 - ($-$$) db 0 ; 0s where we don't have code
dw 0xAA55 ; magic number at the end to specify bootloader
