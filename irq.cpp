#include "io.h"
#include <stdint.h>

extern "C" void kprintf(const char* fmt, ...);
extern "C" void PIC_sendEOI(uint8_t irq);

extern "C" void irq_handler(int irq) {
    kprintf("[IRQ %d] interrupt received", irq); // print statements to keep track during testing

    if (irq == 0) {
        kprintf("timer tick"); // this whole conditonal is just some print stuff to tracking
    } else if (irq == 1) {
        u8 scancode = inb(0x60);
        kprintf("keyboard scancode: 0x%x", scancode);
    }
    PIC_sendEOI(irq);
}

// (TODO) The interrupts will be defined here and can be called above
void interrupt_0()
{
    kprintf("Divide by zero error"); 
    __asm__ volatile ("cli; hlt"); 
}
