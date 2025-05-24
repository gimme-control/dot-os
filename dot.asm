bits 16 ; 16 bit real mode
[org 0x7c00] ; where the bootloader begins in memory

KERNEL_LOCATION equ 0x1000
BOOT_DISK: db 0

section .text

start:

    cli

    mov [BOOT_DISK], dl

    xor ax, ax
    mov es, ax
    mov ds, ax
    mov ss, ax

    mov bp, 0x8000
    mov sp, bp

    sti

    mov si, loading_msg
    call print_string

    call load_kernel

    jc disk_error

    mov si, success_msg
    call print_string

    ; Set video mode (80x25 color text)
    mov ah, 0x00
    mov al, 0x03
    int 0x10

    jmp enter_protected_mode

disk_error:
    mov si, error_msg
    call print_string
    jmp hang

load_kernel:
    pusha

    mov ah, 0x00
    mov dl, [BOOT_DISK]
    int 0x13
    jc .reset_failed

    ; Load kernel (2 sectors starting from sector 2)
    mov bx, KERNEL_LOCATION    ; ES:BX = destination buffer
    mov ah, 0x02               ; Read sectors function
    mov al, 15                 ; Number of sectors to read (increased)
    mov ch, 0x00               ; Cylinder/track number
    mov cl, 0x02               ; Sector number (starts from 1)
    mov dh, 0x00               ; Head number
    mov dl, [BOOT_DISK]        ; Drive number

    int 0x13
    jc .load_failed

    popa
    clc
    ret

.reset_failed:
.load_failed:
    popa
    stc
    ret

print_string:
    pusha
    mov ah, 0x0e

.loop:
    lodsb
    cmp al, 0
    je .done
    int 0x10
    jmp .loop
    
.done:
    popa
    ret

enter_protected_mode:
    ; Define GDT segment constants
    CODE_SEG equ GDT_code - GDT_start
    DATA_SEG equ GDT_data - GDT_start

    cli                       ; Disable interrupts
    lgdt [GDT_descriptor]     ; Load GDT

    ; Enable protected mode
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ; Far jump to protected mode code
    jmp CODE_SEG:start_protected_mode

hang:
    jmp hang

; GDT (Global Descriptor Table)
GDT_start:
    GDT_null:                 ; Null descriptor
        dd 0x0
        dd 0x0

    GDT_code:                 ; Code segment descriptor
        dw 0xffff             ; Limit (low)
        dw 0x0000             ; Base (low)
        db 0x00               ; Base (middle)
        db 0b10011010         ; Access byte
        db 0b11001111         ; Granularity byte
        db 0x00               ; Base (high)

    GDT_data:                 ; Data segment descriptor
        dw 0xffff             ; Limit (low)
        dw 0x0000             ; Base (low)
        db 0x00               ; Base (middle)
        db 0b10010010         ; Access byte
        db 0b11001111         ; Granularity byte
        db 0x00               ; Base (high)

GDT_end:

GDT_descriptor:
    dw GDT_end - GDT_start - 1
    dd GDT_start

; Messages
loading_msg db 'Loading kernel...', 0x0D, 0x0A, 0
success_msg db 'Kernel loaded successfully!', 0x0D, 0x0A, 0
error_msg db 'Disk read error!', 0x0D, 0x0A, 0

[bits 32]
start_protected_mode:
    ; Set up data segments
    mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Set up 32-bit stack
    mov ebp, 0x90000
    mov esp, ebp

    call KERNEL_LOCATION

    jmp $


times 510-($-$$) db 0
dw 0xaa55
