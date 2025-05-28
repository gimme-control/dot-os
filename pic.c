#include <stdint.h> 
#include "io.cpp" // (TODO) Change to io.h at some point 

#define PIC1		0x20		// IO base address for master PIC 
#define PIC2		0xA0		// IO base address for slave PIC 
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	(PIC2+1)

#define PIC_EOI     0x20
#define u8 uint8_t


void PIC_sendEOI(u8 irq)
{
    if(irq >= 8)
        outb(PIC2_COMMAND, PIC_EOI);
        
    outb(PIC1_COMMAND, PIC_EOI); 
}
