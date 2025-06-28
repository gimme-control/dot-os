#include "../util.h"

#define PS2_PORT 0x60

extern "C" void kprintf(const char* fmt, ...);  
extern "C" void irq_install_handler(int irq, void (*handler)(struct InterruptRegisters *regs));


const char scan_code_keys[] = 
{ 
    '1',
    '2',
    '3',
    '4',
    '5',
    '6',
    '7',
    '8',
    '9'
};

void wait_for_ack() 
{ 
    unsigned char response; 
    do {
        response = inb(PS2_PORT);
    } while (response != 0xFA); 
}

extern "C" bool capsOn()
{
    outb(PS2_PORT, 0xED);
    io_wait(); 
    outb(PS2_PORT, 1); 

    unsigned char response = inb(PS2_PORT);

    if(response == 0xFE)
        return false; 

    return true; 
}

void onIrq1(struct InterruptRegisters *regs)
{
    unsigned char scancode = inb(PS2_PORT); 
    int i = scancode; 
    char desc = scan_code_keys[i];
    kprintf("%c\n", desc);
}

extern "C" void keyboard_init()
{
    irq_install_handler(1, &onIrq1);

    outb(PS2_PORT, 0xF0); 
    wait_for_ack(); 
    outb(PS2_PORT, 0x02); 
    wait_for_ack(); 
}
