// you have to build this after during compilation, using a similar command line used to build kernel
// i386-elf-gcc -ffreestanding -m32 -g -c "vga.cpp" -o "Binaries/vga.o"

#include <stdarg.h> // allows for handling functions with variable args

volatile char* vga = (volatile char*)0xB8000; // VGA buffer memory starts at specific address
int cursor = 0; // index to keep track of where the next character should go (every char is 2 bytes)

// outputs a byte to an I/O port
static inline void outb(unsigned short port, unsigned char val) {
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

//moves the hardware cursor to match 'cursor' variable
void move_cursor() {
    unsigned short pos = cursor / 2; // convert byte offset to character position
    outb(0x3D4, 14);
    outb(0x3D5, (pos >> 8) & 0xFF);
    outb(0x3D4, 15);
    outb(0x3D5, pos & 0xFF);
}

// scroller function to scroll up by one line if we reach the bottom of the screen
void scroll() {
    if (cursor >= 80 * 25 * 2) {
        for (int i = 0; i< 80 * 24 * 2; i++) {
            vga[i] = vga[i+160]; // shifts each line up by 1
        }
        for (int i = 80 * 24 * 2; i < 80 * 25 * 2; i += 2) {
            vga[i] = ' ';
            vga[i+1] = 0x0F; // clear last line
        }
        cursor -= 160;
    }
}

// print a single char to VGA text buffer
void print_char(char c) {
    if (c == '\n') {
        cursor += (160 - (cursor % 160)); // go to start of next line
    } else if (c == '\t') {
        for (int i = 0; i < 4; ++i) print_char(' '); // tabs is equal to 4 spaces
    } else {
        vga[cursor++] = c; // character byte
        vga[cursor++] = 0x0F; // Attribute byte: white text on black background
    }

    scroll();
    move_cursor();
}

// null-terminated string to screen gets printed
void print_str(const char* s) {
    while (*s) {
        print_char(*s++); // print each char in the string
    }
}

// prints an int in base 10
void print_int(int value) {
    char buffer[32]; // buffer to store chars of a number
    int i = 0;
    bool negative = false;

    if (value < 0) { // negative number handler
        negative = true;
        value = -value;
    }

    do { // convert numbers to char in reverse order

        buffer[i++] = '0' + (value % 10); // take lease significant digit
        value /= 10; // move to next digit

    } while (value);

    if (negative) buffer[i++] = '-'; // add neg sign if needed

    while (i--) {
        print_char(buffer[i]); // prints buffer in reverse to get correct order
    }
}

void print_hex(unsigned int value) { // prints an unsigned int in hexadecimal (ex. 0xAB12)

    char hex_chars[] = "0123456789ABCDEF"; // hex digit characters
    print_str("0x"); // print hex prefix

    for (int i = 28; i >= 0; i -= 4) { // print each nibble (4 bits) starting from highest
         print_char(hex_chars[(value >> i) & 0xF]); // extract 4-bit chunk
    }
}

// printf-style function to handle format strings and variable args
extern "C" void printf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt); // initialize variable argument list starting after fmt

    for (int i = 0; fmt[i]; ++i) {
        // check for format specifier
         if (fmt[i] == '%' && fmt[i +1]) {
            ++i; // move to specifier character
            switch (fmt[i]) {
                case 'd': print_int(va_arg(args, int)); break; // prints int
                case 'x': print_hex(va_arg(args, unsigned int)); break; // prints hexadecimal
                case 'c': print_char((char)va_arg(args, int)); break; // prints character
                case 's': print_str(va_arg(args, const char*)); break; // prints string
                default: print_char(fmt[i]); break; // unknown specifier
            }
        } else {
            print_char(fmt[i]); // Regular character, print directly
        }
    }
    va_end(args); // clean up the argument list
}

