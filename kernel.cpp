extern "C" int main()
{
    volatile char* vga_buffer = (volatile char*) 0xb8000; 
    int offset = 0; 
    for(int i = 0; i < 1000; ++i)
    {
        vga_buffer[offset] = 'A' + i; 
        vga_buffer[offset + 1] = 0x0F; 
        offset += 2; 
    }
    return 0; 
}
