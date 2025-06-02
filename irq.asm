[bits 32]

%macro IRQ_STUB 1
global irq%1
irq%1:
    cli
    pushad ; save all general purpose regiusters
    push dword %1 ; push IRQ num
    call irq_handler
    add esp, 4 ; pop arg (irq num in this case)
    popad ; restore registers
    sti
    iret
%endmacro

%assign i 0
%rep 16
IRQ_STUB i
%assign i i+1
%endrep

extern irq_handler
