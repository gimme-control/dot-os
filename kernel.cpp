#include <stdint.h>

extern "C" void kprintf(const char* fmt, ...);
extern "C" void kclear();
extern "C" void kprint(const char*);
extern "C" void disable_cursor();
extern "C" void enable_cursor();
extern "C" void idt_init();
extern "C" void PIC_remap(int, int);
extern "C" void irq_handler(int irq);
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
extern "C" void IRQ_clear_mask(uint8_t irq);



extern "C" int main() {
    idt_init(); // loading idt
    PIC_remap(0x20, 0x28); // remap irqs
    //
    // IRQ_clear_mask(0);
    // IRQ_clear_mask(1);
    //
    //
    kclear();

    kprintf("Hello from dot-os!\n");
    kprintf("Kernel successfully loaded at 0x1000\n");

    kprintf("Testing integers: %d, %d, %d\n", 42, -17, 0);
    kprintf("Testing hex: %x, %x\n", 0xDEADBEEF, 255);
    kprintf("Testing character: %c\n", 'A');
    kprintf("Testing string: %s\n", "Success!");

    // Test edge cases
    kprintf("Percent literal: %%\n");
    kprintf("Multiple args: %d + %d = %d\n", 5, 3, 8);


    // (NOTE) Cursor drawn directly by the GPU, not the BIOS
    // disable_cursor();
    // enable_cursor();  // cursor_start & cursor_end args available

    return 0;
}
