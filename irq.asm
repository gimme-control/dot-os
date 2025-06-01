[bits 32]

global irq0
irq1:
    hlt 
    jmp irq1
    iretd


