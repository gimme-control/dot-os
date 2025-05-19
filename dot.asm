; constants
BOOT_DRIVE equ 0x80 ; BIOS drive number
stage2_SECTORS equ 4  ; changeable to what the size of our stage2.bin actually is divided by 512 per 512 byte sector

; Instructs code to be in 16 bit mode because bootloader runs in real mode
bits 16

; Specifies where in memory the program starts, allowing offset calculations
org 0x7c00

section .text

start:
    mov [boot_drive], dl
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
  ;  mov ah, 0x02
  ;  mov al, 0x01
  ;  mov ch, 0x00
  ;  mov cl, 0x02
  ;  mov dh, 0x00
  ;  mov dl, 0x00

  ;  mov ax, 0x1000
  ;  mov es, ax
  ;  mov bx, 0x0000

  ;  int 0x13
  ;  jc disk_error

  ;  ; Jumping to the kernel
  ;  jmp 0x1000:0x0000

  ; My attempt at loading the kernel -> looping to read N sectors of Stage-2 into the memory address 0x1000:0x0000
    mov si, 0 ; sector count = 0
    mov ah, 0x02 ; BIOS function 02h = read sectors
    mov ch, 0x00 ;track/cylinder = 0
    mov cl, 0x02 ; first sector to read in this case being 2
    mov dh, 0x00 ; head = 0
    mov dl, [boot_drive] ; drive

    mov ax, 0x1000 ; ES:BX = 0x1000:0x0000 <- load target
    mov es, ax
    xor bx, bx

read_loop:
    push si ; save how many we've read so far
    mov al, 1 ; total sectors intended to read
    int 0x13 ; BIOS disk read
    jc disk_error ; if CF=1 -> error

    pop si
    inc si ; same as sectors_read++
    inc cl ; mov to next sector on the disk
    cmp si, STAGE2_SECTORS
    jl read_loop ; loop until we've read them all

    jmp 0x1000:0x0000 ; far-jump into the Stage2 Code

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
