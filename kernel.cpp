#include <stdint.h>

extern "C" void kprintf(const char* fmt, ...);
extern "C" void kclear();
extern "C" void kprint(const char*);
extern "C" void disable_cursor();
extern "C" void enable_cursor();
extern "C" void idt_init();
extern "C" void PIC_remap(int, int);
extern "C" void irq_handler(int irq);
extern "C" void IRQ_clear_mask(uint8_t irq);
extern "C" void vga_init(); 
extern "C" void initTimer(); 
extern "C" bool capsOn(); 
extern "C" void keyboard_init(); 

extern "C" int main() {
    idt_init(); // loading idt
    PIC_remap(0x20, 0x28); // remap irqs
    vga_init();
    keyboard_init(); 
    initTimer(); 
     
    enable_cursor(); 

    kprintf("Hello\n");
    kprintf("Kernel successfully loaded at 0x1000\n");
    kprintf("Testing integers: %d, %d, %d\n", 42, -17, 0);
    kprintf("Testing hex: %x, %x\n", 0xDEADBEEF, 255);
    kprintf("Testing character: %c\n", 'A');
    kprintf("Testing string: %s\n", "Success!");

    // Test edge cases
    kprintf("Percent literal: %%\n");
    kprintf("Multiple args: %d + %d = %d\n", 5, 3, 8);


    // (NOTE) Cursor drawn directly by the GPU, not the BIOS
    disable_cursor();
    enable_cursor();  // cursor_start & cursor_end args available

    return 0;
}
