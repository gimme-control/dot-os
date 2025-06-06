#pragma once 

#include <stdint.h> 

#define u32 uint32_t 
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

static inline void io_wait(void){
    outb(0x80, 0); //dummy I/O write to introduce a small delay
}

typedef struct regs 
{
    unsigned int gs, fs, es, ds; 
    unsigned int edi, esi, ebp, esp ,ebx, edx, ecx, eax; 
    unsigned int int_no, err_code;  
    unsigned int eip, cs, eglags, useresp, ss; 

} __attribute__((packed)) regs; 

extern "C" void kprint(const char*);
extern "C" void kprintf(const char*, ...);
