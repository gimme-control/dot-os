#include <stdint.h>
#include "io.h"

extern "C" 
{
    extern void _isr0();
    extern void _isr1();
    extern void _isr2();
    extern void _isr3();
    extern void _isr4();
    extern void _isr5();
    extern void _isr6();
    extern void _isr7();
    extern void _isr8();
    extern void _isr9();
    extern void _isr10();
    extern void _isr11();
    extern void _isr12();
    extern void _isr13();
    extern void _isr14();
    extern void _isr15();
    extern void _isr16();
    extern void _isr17();
    extern void _isr18();
    extern void _isr19();
    extern void _isr20();
    extern void _isr21();
    extern void _isr22();
    extern void _isr23();
    extern void _isr24();
    extern void _isr25();
    extern void _isr26();
    extern void _isr27();
    extern void _isr28();
    extern void _isr29();
    extern void _isr30();
    extern void _isr31();
}

void isrs_install(); 

typedef struct regs 
{
    unsigned int gs, fs, es, ds; 
    unsigned int edi, esi, ebp, esp ,ebx, edx, ecx, eax; 
    unsigned int int_no, err_code;  
    unsigned int eip, cs, eglags, useresp, ss; 

} __attribute__((packed)) regs; 

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
    _fault_handler(struct regs*);

extern "C" void _fault_handler(struct regs *r)
{
    kprint("Received interrupt: ");
    kprintf("%d", r->int_no);
    unsigned int cr2;
    asm volatile("mov %%cr2, %0" : "=r" (cr2));
    kprint("\nCR2 Register: ");
    kprintf("%x", cr2);
    unsigned int stack_value;
    asm volatile("movl 4(%%ebp), %0" : "=r" (stack_value));
    kprint("\nValue on stack: ");
    kprintf("%x", stack_value);
    unsigned int return_pointer;
    asm volatile("movl 8(%%ebp), %0" : "=r" (return_pointer));
    kprint("\nReturn Pointer: ");
    kprintf("%x", return_pointer);
    while(1);
}

void idt_set_descriptor(u8 vector, void* isr, u8 flags)
{
    idt_entry_t* descriptor = &idt[vector];
    descriptor->addr_low    = (u32)((uintptr_t)isr & 0xFFFF);
    descriptor->addr_high   = (u32)((uintptr_t)isr >> 16);

    descriptor->kernel_cs   = 0x08; // (TODO) Segment selector, figure this out 
    descriptor->attributes  = flags;
    descriptor->reserved    = 0;
}

#define IDT_MAX_DESCRIPTORS 256
static bool vectors[IDT_MAX_DESCRIPTORS];
extern void* isr_stub_table[];

extern "C" void idt_init()
{
    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = (uint16_t)sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS - 1;
    
    isrs_install(); 

    __asm__ volatile ("lidt %0" : : "m"(idtr)); // load the new IDT
    __asm__ volatile ("sti"); // enables interrupts

}

void isrs_install()
{
	idt_set_descriptor(0, (void*)_isr0, 0x8E);
	idt_set_descriptor(1, (void*)_isr1, 0x8E);
	idt_set_descriptor(2, (void*)_isr2, 0x8E);
	idt_set_descriptor(3, (void*)_isr3, 0x8E);
	idt_set_descriptor(4, (void*)_isr4, 0x8E);
	idt_set_descriptor(5, (void*)_isr5, 0x8E);
	idt_set_descriptor(6, (void*)_isr6, 0x8E);
	idt_set_descriptor(7, (void*)_isr7, 0x8E);
	idt_set_descriptor(8, (void*)_isr8, 0x8E);
	idt_set_descriptor(9, (void*)_isr9, 0x8E);
	idt_set_descriptor(10, (void*)_isr10, 0x8E);
	idt_set_descriptor(11, (void*)_isr11, 0x8E);
	idt_set_descriptor(12, (void*)_isr12, 0x8E);
	idt_set_descriptor(13, (void*)_isr13, 0x8E);
	idt_set_descriptor(14, (void*)_isr14, 0x8E);
	idt_set_descriptor(15, (void*)_isr15, 0x8E);
	idt_set_descriptor(16, (void*)_isr16, 0x8E);
	idt_set_descriptor(17, (void*)_isr17, 0x8E);
	idt_set_descriptor(18, (void*)_isr18, 0x8E);
	idt_set_descriptor(19, (void*)_isr19, 0x8E);
	idt_set_descriptor(20, (void*)_isr20, 0x8E);
	idt_set_descriptor(21, (void*)_isr21, 0x8E);
	idt_set_descriptor(22, (void*)_isr22, 0x8E);
	idt_set_descriptor(23, (void*)_isr23, 0x8E);
	idt_set_descriptor(24, (void*)_isr24, 0x8E);
	idt_set_descriptor(25, (void*)_isr25, 0x8E);
	idt_set_descriptor(26, (void*)_isr26, 0x8E);
	idt_set_descriptor(27, (void*)_isr27, 0x8E);
	idt_set_descriptor(28, (void*)_isr28, 0x8E);
	idt_set_descriptor(29, (void*)_isr29, 0x8E);
	idt_set_descriptor(30, (void*)_isr30, 0x8E);
	idt_set_descriptor(31, (void*)_isr31, 0x8E);
}

