#include "util.h"

#define PIC1		0x20		// IO base address for master PIC
#define PIC2		0xA0		// IO base address for slave PIC
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	(PIC2+1)

#define PIC_EOI     0x20
#define u8 uint8_t

extern "C"
{
    extern void irq0();
    extern void irq1();
    extern void irq2();
    extern void irq3();
    extern void irq4();
    extern void irq5();
    extern void irq6();
    extern void irq7();
    extern void irq8();
    extern void irq9();
    extern void irq10();
    extern void irq11();
    extern void irq12();
    extern void irq13();
    extern void irq14();
    extern void irq15();

    extern void idt_set_descriptor(u8, void*, u8); 
}

    
/* reinitialize the PIC controllers, giving them specified vector offsets
    rather than 8h and 70h, as configured by default */

#define ICW1_ICW4	0x01		/* Indicates that ICW4 will be present */
#define ICW1_SINGLE	0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL	0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	0x10		/* Initialization - required! */

#define ICW4_8086	0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
#define ICW4_SFNM	0x10		/* Special fully nested (not) */

void irq_install();

extern "C" {

    void PIC_sendEOI(u8 irq)
    {
        if(irq >= 8)
            outb(PIC2_COMMAND, PIC_EOI);

        outb(PIC1_COMMAND, PIC_EOI);
    }

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
    outb(PIC1_DATA, 0);
    outb(PIC2_DATA, 0);

    irq_install();

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

void* irq_routines[16] = 
{
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0 
};

extern "C" void irq_install_handler(int irq, void (*handler)(struct InterruptRegisters *regs))
{
    irq_routines[irq] = (void*)handler; 
}

void irq_uninstall_handler(int irq) 
{
    irq_routines[irq] = 0; 
}

static volatile int currentInterrupts[15];

void irq_install()
{
    idt_set_descriptor(32, (void*)irq0, 0x8E);
    idt_set_descriptor(33, (void*)irq1, 0x8E);
    idt_set_descriptor(34, (void*)irq2, 0x8E);
    idt_set_descriptor(35, (void*)irq3, 0x8E);
    idt_set_descriptor(36, (void*)irq4, 0x8E);
    idt_set_descriptor(37, (void*)irq5, 0x8E);
    idt_set_descriptor(38, (void*)irq6, 0x8E);
    idt_set_descriptor(39, (void*)irq7, 0x8E);
    idt_set_descriptor(40, (void*)irq8, 0x8E);
    idt_set_descriptor(41, (void*)irq9, 0x8E);
    idt_set_descriptor(42, (void*)irq10, 0x8E);
    idt_set_descriptor(43, (void*)irq11, 0x8E);
    idt_set_descriptor(44, (void*)irq12, 0x8E);
    idt_set_descriptor(45, (void*)irq13, 0x8E);
    idt_set_descriptor(46, (void*)irq14, 0x8E);
    idt_set_descriptor(47, (void*)irq15, 0x8E);

    for(int i = 0; i < 16; i++){
        currentInterrupts[i] = 0;
    }
}

extern "C" void _irq_handler(regs *r)
{
    currentInterrupts[r->int_no - 32] = 1;
    // kprintf("%i\n", r->int_no - 32); Useful for debugging interrupts
    void (*handler)(struct regs *r);

    handler = (void (*)(regs*))irq_routines[r->int_no - 32];
    if (handler)
    {
        handler(r);
    }

    u8 interrupt = r->int_no; // unsigned int to u8 conversion 
    PIC_sendEOI(interrupt - 32);
}
