extern "C" void kprintf(const char* fmt, ...);
extern "C" void kclear();

extern "C" int main() {

    kclear();

    kprintf("Hello from DotOS!\n");
    kprintf("Kernel successfully loaded at 0x1000\n");

    kprintf("Testing integers: %d, %d, %d\n", 42, -17, 0);
    kprintf("Testing hex: %x, %x\n", 0xDEADBEEF, 255);
    kprintf("Testing character: %c\n", 'A');
    kprintf("Testing string: %s\n", "Success!");

    // Test edge cases
    kprintf("Percent literal: %%\n");
    kprintf("Multiple args: %d + %d = %d\n", 5, 3, 8);

    return 0;
}
