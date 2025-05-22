extern "C" void printf(const char* fmt, ...);

extern "C" int main()
{
<<<<<<< HEAD
    printf("Hello!");
    printf("Decimal: %d, Hex: %x, Char: %c, String: %s\n", 123, 0xABC, 'Z', "dot-os");
    return 0;
=======
    volatile char* vga_buffer = (volatile char*) 0xb8000; 
    int offset = 0; 
    for(int i = 0; i < 1000; ++i)
    {
        vga_buffer[offset] = 'A' + i; 
        vga_buffer[offset + 1] = 0x0F; 
        offset += 2; 
    }
    return 0; 
>>>>>>> f420879cb3eb2f188073e5a0389fe000ed5ebdf2
}
