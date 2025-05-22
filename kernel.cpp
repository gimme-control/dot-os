extern "C" void printf(const char* fmt, ...);

extern "C" int main()
{
    printf("Hello!");
    printf("Decimal: %d, Hex: %x, Char: %c, String: %s\n", 123, 0xABC, 'Z', "dot-os");
    return 0;
}
