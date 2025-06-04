#include "io.h"

#define PIC1		0x20		// IO base address for master PIC
#define PIC2		0xA0		// IO base address for slave PIC
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	(PIC2+1)

#define PIC_EOI     0x20
#define u8 uint8_t

extern "C" {

void PIC_sendEOI(u8 irq)
{
    if(irq >= 8)
        outb(PIC2_COMMAND, PIC_EOI);

    outb(PIC1_COMMAND, PIC_EOI);
}

#define ICW1_ICW4 0x01
#define ICW1_INIT 0x10
#define ICW4_8086 0x01

// Selectively disable IRQs
void IRQ_set_mask(u8 irq_line) {
    u16 port;
    u8 value;

    if (irq_line < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        irq_line -= 8;
    }
    value = inb(port) | (1 << irq_line);
    outb(port, value);
}
}

extern "C" void PIC_remap(int offset1, int offset2) {
    u8 a1 = inb(PIC1_DATA); // save masks
    u8 a2 = inb(PIC2_DATA);

    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC1_DATA, offset1); // Master PIC vector offset
    io_wait();
    outb(PIC2_DATA, offset2); // Slave PIC vector offset 
    io_wait();
    outb(PIC1_DATA, 4); // Tell master that there is a slave at IRQ2(0000 0100)
    io_wait();
    outb(PIC2_DATA, 2); // Tell slave its cascade identity
    io_wait();
    outb(PIC1_DATA, ICW4_8086); // 8086 mode (not 8080)
    io_wait();
    outb(PIC2_DATA, ICW4_8086);
    io_wait();

    // Unmask both PICs
    outb(PIC1_DATA, a1);
    outb(PIC2_DATA, a2);
}

// Disables the entire PIC in case we want to use APIC or UAPIC
void pic_disable(void)
{
    outb(PIC1_DATA, 0xff);
    outb(PIC2_DATA, 0xff);
}


extern "C" void IRQ_clear_mask(u8 irq_line) {
    u16 port;
    u8 value;

    if (irq_line < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        irq_line -= 8;
    }
    value = inb(port) & ~(1 << irq_line);
    outb(port, value);
}


// diagnostic read functions to help debug and handle spurious IRQs
#define PIC_READ_IRR 0x0A
#define PIC_READ_ISR 0x0B

static u16 __pic_get_irq_reg(int ocw3) {
    outb(PIC1_COMMAND, ocw3);
    outb(PIC2_COMMAND, ocw3);
    return (inb(PIC2_COMMAND) << 8) | inb(PIC1_COMMAND);
}

u16 pic_get_irr(void) { return __pic_get_irq_reg(PIC_READ_IRR); }
u16 pic_get_isr(void) { return __pic_get_irq_reg(PIC_READ_ISR); }
