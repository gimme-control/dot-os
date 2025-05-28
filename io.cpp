#include <stdint.h> 

#define u16 uint16_t
#define u8 uint8_t

// IO helper functions 
static inline void outb(u16 port, u8 val)
{
    __asm__ volatile ( "outb %%al, %%dx" : : "a"(val), "Nd"(port) : "memory"); 
}

static inline u8 inb(u16 port)
{
    u8 ret; 
    __asm__ volatile ( "inb %%dx, %%al" 
                    : "=a" (ret)
                    : "Nd" (port)
                    : "memory");

    return ret; 
}
