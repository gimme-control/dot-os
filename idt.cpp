#include <stdint.h>

#define u16 uint16_t
#define u8 uint8_t

struct interrupt_descriptor
{
    u16 address_low; 
    u16 selector; 
    u8 ist; 
    u8 flags; 
    u16 address_mid; 
    u16 address_high; 
    u16 reserved; 
} __attribute__((packed));



