[bits 32]

; INTERRUPT SERVICE ROUTINES 

global _isr0
global _isr1
global _isr2
global _isr3
global _isr4
global _isr5
global _isr6
global _isr7
global _isr8
global _isr9
global _isr10
global _isr11
global _isr12
global _isr13
global _isr14
global _isr15
global _isr16
global _isr17
global _isr18
global _isr19
global _isr20
global _isr21
global _isr22
global _isr23
global _isr24
global _isr25
global _isr26
global _isr27
global _isr28
global _isr29
global _isr30
global _isr31

[extern _fault_handler]

_isr0:
	cli
	push dword 0	; A normal ISR stub that pops a dummy error code to keep a uniform stack frame
	push dword 0
	jmp isr_common_stub

_isr1:
	cli
	push dword 0
	push dword 1
	jmp isr_common_stub
_isr2:
	cli
	push dword 0
	push dword 2
	jmp isr_common_stub
_isr3:
	cli
	push dword 0
	push dword 3
	jmp isr_common_stub
_isr4:
	cli
	push dword 0
	push dword 4
	jmp isr_common_stub
_isr5:
	cli
	push dword 0
	push dword 5
	jmp isr_common_stub
_isr6:
	cli
	xchg bx, bx
	push dword 0
	push dword 6
	jmp isr_common_stub
_isr7:
	cli
	push dword 0
	push dword 7
	jmp isr_common_stub
_isr8:
	cli
	push dword 8
	jmp isr_common_stub
_isr9:
	cli
	push dword 0
	push dword 9
	jmp isr_common_stub
_isr10:
	cli
	push dword 10
	jmp isr_common_stub
_isr11:
	cli
	push dword 11
	jmp isr_common_stub
_isr12:
	cli
	push dword 12
	jmp isr_common_stub
_isr13:
	cli
	push dword 13
	jmp isr_common_stub
_isr14:
	cli
	push dword 14
	jmp isr_common_stub
_isr15:
	cli
	push dword 0
	push dword 15
	jmp isr_common_stub
_isr16:
	cli
	push dword 0
	push dword 16
	jmp isr_common_stub
_isr17:
	cli
	push dword 0
	push dword 17
	jmp isr_common_stub
_isr18:
	cli
	push dword 0
	push dword 18
	jmp isr_common_stub
_isr19:
	cli
	push dword 0
	push dword 19
	jmp isr_common_stub
_isr20:
	cli
	push dword 0
	push dword 20
	jmp isr_common_stub
_isr21:
	cli
	push dword 0
	push dword 21
	jmp isr_common_stub
_isr22:
	cli
	push dword 0
	push dword 22
	jmp isr_common_stub
_isr23:
	cli
	push dword 0
	push dword 23
	jmp isr_common_stub
_isr24:
	cli
	push dword 0
	push dword 24
	jmp isr_common_stub
_isr25:
	cli
	push dword 0
	push dword 25
	jmp isr_common_stub
_isr26:
	cli
	push dword 0
	push dword 26
	jmp isr_common_stub
_isr27:
	cli
	push dword 0
	push dword 27
	jmp isr_common_stub
_isr28:
	cli
	push dword 0
	push dword 28
	jmp isr_common_stub
_isr29:
	cli
	push dword 0
	push dword 29
	jmp isr_common_stub
_isr30:
	cli
	push dword 0
	push dword 30
	jmp isr_common_stub
_isr31:
	cli
	push dword 0
	push dword 31
	jmp isr_common_stub


isr_common_stub:
	pushad
	push ds
	push es
	push fs
	push gs
	mov ax, 0x10                   ; Load the Kernel Data Segment descriptor!
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov eax, esp                   ; Push us the stack
	
	push eax
	mov eax, _fault_handler		   ; checks if interrupt number < 32 (if it represents an exception)
                                   ; prints exception message and halts system.
	call eax	                   ; A special call, preserves the 'eip' register
	pop eax
	pop gs
	pop fs
	pop es
	pop ds
	popad
	add esp, 8	                   ; Cleans up the pushed error code and pushed ISR number
	iret		                   ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP!
	
