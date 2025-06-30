#include <stdarg.h>
#include "util.h"

void update_cursor(int, int);
const int VGA_WIDTH = 80;
const int VGA_HEIGHT = 25;

// VGA text buffer
volatile unsigned short* vga = (volatile unsigned short*) 0xB8000; // (TODO) Value gets zero initialized - temp solution is vga_init() 

int cursor_pos = 0; // Position in terms of characters, not bytes

extern "C" void vga_init()
{
    vga = (volatile unsigned short*)0xB8000;
}

// Clear screen function
void clear_screen() {
    for (int i = 0; i < 80 * 25; i++) { // 80*25 -> cursor_pos
        vga[i] = (0x0F << 8) | ' '; // White on black space
    }
    cursor_pos = 0;
}

// Print a single character to screen
extern "C" void print_char(char c) {

    switch(c)
    {
        case '\n': 
        {
            cursor_pos = ((cursor_pos / 80) + 1) * 80; // Move to next line
        } break; 
        case '\r': 
        {
            cursor_pos = (cursor_pos / 80) * 80; // Move to start of current line
        } break; 
        case '\t': 
        {
            cursor_pos += 8 - (cursor_pos % 8); // Tab to next 8-char boundary
        } break; 
        case '\b': 
        {
            --cursor_pos;
            vga[cursor_pos] = (0x0F << 8) | ' ';
        } break; 
        default:
        {
            vga[cursor_pos] = (0x0F << 8) | c; // White on black
            cursor_pos++;
        } break; 
    }

     // Handle screen scrolling
     if (cursor_pos >= 80 * 25) {
         // Scroll up one line
         for (int i = 0; i < 80 * 24; i++) {
             vga[i] = vga[i + 80];
         }
         // Clear the last line
         for (int i = 80 * 24; i < 80 * 25; i++) {
             vga[i] = (0x0F << 8) | ' ';
         }
         cursor_pos = 80 * 24; // Move cursor to start of last line
    }

    update_cursor(cursor_pos % VGA_WIDTH, cursor_pos / VGA_WIDTH);
}

// Print a null-terminated string
extern "C" void print_str(const char* str) {
    int count = 0;
    if (!str) {
        return;
    }
    while (*str) {
        print_char(*str++);
    }
}

// Print an integer (base 10)
extern "C" void print_int(int num) {
    if (num == 0) {
        print_char('0');
        return;
    }

    if (num < 0) {
        print_char('-');
        num = -num;
    }

    char buf[12]; // Enough for 32-bit int
    int i = 0;

    while (num > 0) {
        buf[i++] = '0' + (num % 10);
        num /= 10;
    }

    // Print in reverse order
    while (i > 0) {
        print_char(buf[--i]);
    }
}

void print_hex(unsigned int val) {
    const char* hex_chars = "0123456789ABCDEF";
    print_str("0x");

    if (val == 0) {
        print_char('0');
        return;
    }

    // Find the highest non-zero nibble
    int start = -1;
    for (int i = 7; i >= 0; i--) {
        if ((val >> (i * 4)) & 0xF) {
            start = i;
            break;
        }
    }

    // Print from highest nibble down
    for (int i = start; i >= 0; i--) {
        print_char(hex_chars[(val >> (i * 4)) & 0xF]);
    }
}

extern "C" void kprintf(const char* fmt, ...) {
    if (!fmt) return;

    va_list args;
    va_start(args, fmt);

    for (int i = 0; fmt[i]; i++) {
        if (fmt[i] == '%' && fmt[i + 1]) {
            i++;
            switch (fmt[i]) {
                case 'd':
                case 'i':
                    print_int(va_arg(args, int));
                    break;
                case 'x':
                case 'X':
                    print_hex(va_arg(args, unsigned int));
                    break;
                case 'c':
                    print_char((char)va_arg(args, int));
                    break;
                case 's':
                    print_str(va_arg(args, const char*));
                    break;
                case '%':
                    print_char('%');
                    break;
                default:
                    print_char('%');
                    print_char(fmt[i]);
                    break;
            }
        } else {
            print_char(fmt[i]);
        }
    }

    va_end(args);
}


extern "C" void disable_cursor()
{
    outb(0x3D4, 0x0A);
    outb(0x3D5, 0x20);
}

extern "C" void enable_cursor(u8 cursor_start, u8 cursor_end)
{
    outb(0x3d4, 0x0A);
    outb(0x3d5, (inb(0x3D5) & 0xC0) | cursor_start);
    outb(0x3d4, 0x0B);
    outb(0x3d5, (inb(0x3D5) & 0xE0) | cursor_end);
}

void update_cursor(int x, int y)
{
    if (x < 0 || x >=80 || y < 0 || y >=25)
        return ;
	uint16_t pos = y * VGA_WIDTH + x;

	outb(0x3D4, 0x0F);
    outb(0x3D5, (u8) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (u8) ((pos >> 8) & 0xFF));
}

// Additional utility functions
extern "C" void kclear() {
    clear_screen();
}

extern "C" void kprint(const char* str) {
    print_str(str);
}
