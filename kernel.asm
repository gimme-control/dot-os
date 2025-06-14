[bits 32]
[extern main]

global _start

section .text
_start:
    call main
    jmp $

