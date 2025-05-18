; Instructs code to be in 16 bit mode because bootloader runs in real mode
bits 16

; Specifies where in memory the program starts, allowing offset calculations
org 0x7c00

section .text

start: 
    xor ax, ax
    mov ds, ax 
    mov si, my_string

print_loop:
    lodsb
    or al, al 
    jz load_kernel
    mov ah, 0x0e
    int 0x10
    jmp print_loop

load_kernel: 
    mov ah, 0x02 
    mov al, 0x01 
    mov ch, 0x00 
    mov cl, 0x02 
    mov dh, 0x00 
    mov dl, 0x00 

    mov ax, 0x1000
    mov es, ax
    mov bx, 0x0000 

    int 0x13 
    jc disk_error

    ; Jumping to the kernel 
    jmp 0x1000:0x0000

disk_error: 
    cli
    hlt

exit: 
    cli
    hlt

my_string: db "Hello ASM", 0 

; Puts 0's in the part where we dont have any code because bootloader is 512 bytes
times 510 - ($-$$) db 0 
dw 0xAA55
