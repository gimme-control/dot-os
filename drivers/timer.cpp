#include <stdint.h>
#include "../io.h"

extern "C" void irq_install_handler(int irq, void (*handler)(struct InterruptRegisters *regs));

uint64_t ticks; 
const uint32_t freq = 10000; // Adjust this for the speed

void onIrq0(struct InterruptRegisters *regs)
{
    ticks += 1; 
}

extern "C" void initTimer()
{
    ticks = 0; 
    irq_install_handler(0, &onIrq0);

    uint32_t divisor = 1193180/freq;
    outb(0x43, 0x36);
    outb(0x40, (uint8_t)(divisor & 0xFF));
    outb(0x40, (uint8_t)((divisor >> 8) & 0xFF));
}
