[bits 32]
gloabl irq0
irq0:
    call irq_handler
    iretd
