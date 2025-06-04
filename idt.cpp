#include <stdint.h>
#include "io.h"

extern "C" void kprintf(const char* fmt, ...);
extern "C" void print_char(char c);
extern "C" void print_str(const char* str);
extern "C" void print_int(int num);
extern "C" void irq0();
extern "C" void irq1();
extern "C" void irq2();
extern "C" void irq3();
extern "C" void irq4();
extern "C" void irq5();
extern "C" void irq6();
extern "C" void irq7();
extern "C" void irq8();
extern "C" void irq9();
extern "C" void irq10();
extern "C" void irq11();
extern "C" void irq12();
extern "C" void irq13();
extern "C" void irq14();
extern "C" void irq15();

struct idt_entry_t
{
    u16 addr_low; // Lower 16 bits of ISRs address
    u16 kernel_cs; // GDT segment selector to load
    u8 reserved; // Set to zero
    u8 attributes; // Types and attributes
    u16 addr_high; // Higher 16 bits of ISRs address

} __attribute__((packed));

__attribute__((aligned(0x10)))
    static idt_entry_t idt[256]; // Array of IDT entries; aligned for performance


typedef struct
{
    u16 limit;
    u32 base;
} __attribute__((packed)) idtr_t;

static idtr_t idtr;

extern "C" void __attribute__ ((noreturn))
    exception_handler(int vector);

extern "C" void exception_handler(int vector)
{
    // print_str("EXCEPTION: ");
    // print_int(vector);
    // print_char('\n');


    while(1)
    {
        __asm__ volatile ("cli; hlt"); // halts computer
    }
}

void idt_set_descriptor(u8 vector, void* isr, u8 flags)
{
    idt_entry_t* descriptor = &idt[vector];
    descriptor->addr_low    = (u32)((uintptr_t)isr & 0xFFFF);
    descriptor->kernel_cs   = 0x08; // (NOTE) Figure out this value
    descriptor->attributes  = flags;
    descriptor->addr_high   = (u32)((uintptr_t)isr >> 16);
    descriptor->reserved    = 0;
}

#define IDT_MAX_DESCRIPTORS 256
static bool vectors[IDT_MAX_DESCRIPTORS];
extern void* isr_stub_table[];

extern "C" void idt_init()
{
    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = (uint16_t)sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS - 1;

    for (uint8_t vector = 0; vector < 32; vector++) {
        idt_set_descriptor(vector, isr_stub_table[vector], 0x8E);
        vectors[vector] = true;
    }
    void (*irq_table[16])() = {
        irq0, irq1, irq2, irq3,
        irq4, irq5, irq6, irq7,
        irq8, irq9, irq10, irq11,
        irq12, irq13, irq14, irq15
    };
    for (uint8_t irq = 0; irq < 16; irq++) {
        idt_set_descriptor(32+irq, (void*)irq_table[irq], 0x8E);
        vectors[32+irq] = true;
    }


    __asm__ volatile ("lidt %0" : : "m"(idtr)); // load the new IDT
    __asm__ volatile ("sti"); // enables interrupts
}

