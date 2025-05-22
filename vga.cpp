#include <stdarg.h>

// VGA text buffer starts here
volatile char* vga = (volatile char*)0xB8000;
int cursor = 0;

// Print a single character to screen
void print_char(char c) {
    if (c == '\n') {
        cursor = ((cursor / 160) + 1) * 160; // Move to next line
    } else {
        vga[cursor++] = c;
        vga[cursor++] = 0x0F; // White on black
    }

    if (cursor >= 80 * 25 * 2) {
        // scroll up
        for (int i = 0; i < 80 * 24 * 2; i++)
            vga[i] = vga[i + 160];
        for (int i = 80 * 24 * 2; i < 80 * 25 * 2; i += 2) {
            vga[i] = ' ';
            vga[i + 1] = 0x0F;
        }
        cursor -= 160;
    }
}

// Print a null-terminated string
void print_str(const char* str) {
    while (*str)
        print_char(*str++);
}

// Print an integer (base 10)
void print_int(int num) {
    if (num < 0) {
        print_char('-');
        num = -num;
    }

    char buf[10];
    int i = 0;

    do {
        buf[i++] = '0' + (num % 10);
        num /= 10;
    } while (num);

    while (i--)
        print_char(buf[i]);
}

// Print an unsigned hex (base 16)
void print_hex(unsigned int val) {
    const char* hex = "0123456789ABCDEF";
    print_str("0x");
    for (int i = 28; i >= 0; i -= 4) {
        print_char(hex[(val >> i) & 0xF]);
    }
}

extern "C" void kprintf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    for (int i = 0; fmt[i]; ++i) {
        if (fmt[i] == '%' && fmt[i + 1]) {
            i++;
            switch (fmt[i]) {
                case 'd': print_int(va_arg(args, int)); break;
                case 'x': print_hex(va_arg(args, unsigned int)); break;
                case 'c': print_char((char)va_arg(args, int)); break;
                case 's': print_str(va_arg(args, const char*)); break;
                default: print_char(fmt[i]); break;
            }
        } else {
            print_char(fmt[i]);
        }
    }

    va_end(args);
}
