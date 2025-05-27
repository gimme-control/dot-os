#include <stdint.h>

#define u32 uint32_t
#define u16 uint16_t
#define u8 uint8_t


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

__attribute__((noreturn))
    void exception_handler(void);

void exception_handler()
{
    __asm__ volatile ("cli; hlt"); // halts computer
}



