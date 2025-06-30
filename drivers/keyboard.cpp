#include "../util.h"

#define PS2_PORT 0x60

extern "C" void kprintf(const char* fmt, ...);  
extern "C" void irq_install_handler(int irq, void (*handler)(struct InterruptRegisters *regs));

bool caps = false; 
bool shiftOn = false; 

const char scan_code[128] =
{
    0,  27, 
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 
    '-', '=', 
    '\b', // backspace	
    '\t', // tabs
    'q', 'w', 'e', 'r',	't', 'y', 'u', 'i', 'o', 'p', '[', ']', 
    '\n', // Enter key 
     0,	  // Control 
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	
    '\'', '`',   
    0,	  // Left shift 
    '\\', 'z', 'x', 'c', 'v', 'b', 'n',			
    'm', ',', '.', '/',   
    0,    // Right shift 
    '*',
    0,	  // Alt 
    ' ',  // Space bar 
    0,	  // Caps lock 
    0,	  // F1 key 
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	// F10 
    0,	// Num lock
    0,	// Scroll Lock 
    0,	// Home key 
    0,	// Up Arrow 
    0,	// Page Up 
    '-',
    0,	// Left Arrow 
    0,
    0,	// Right Arrow
  '+',
    0,	// End key
    0,	// Down Arrow 
    0,	// Page Down 
    0,	// Insert Key 
    0,	// Delete Key 
    0,   0,   0,
    0,	// F11 Key 
    0,	// F12 Key 
    0,	// All other keys are undefined
};

char to_upper(char c)
{
    if(c >= 'a' && c <= 'z')
    {
        return c - ('a' - 'A');
    }
    return c; 
}

void wait_for_ack() 
{ 
    unsigned char response; 
    do {
        response = inb(PS2_PORT);
    } while (response != 0xFA); 
}

extern "C" bool capsOn() // dummy test function 
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
    int size = sizeof(scan_code) / sizeof(scan_code[0]); 
    unsigned char scancode = inb(PS2_PORT); 
    int i = scancode; 
    if(i > size) 
        return; 

    char desc = scan_code[i];
    
    switch(i)
    {
        case 58: // If i equals index for caps key
        {
            caps = caps ? false : true;
        } break;
        case 43: 
        {
            shiftOn = shiftOn ? false : true;
        } break; 
        default: 
        {
            desc = caps ? desc : to_upper(desc);
            desc = shiftOn ? desc : to_upper(desc);
            kprintf("%c", desc);
        } break; 
    }
}

extern "C" void keyboard_init()
{
    irq_install_handler(1, &onIrq1);

    outb(PS2_PORT, 0xF0); 
    wait_for_ack(); 
    outb(PS2_PORT, 0x02); 
    wait_for_ack(); 
}
