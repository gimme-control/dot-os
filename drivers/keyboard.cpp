#include "../util.h"

extern "C" void add_to_cbuff(cbuffer_t* buf, char c, bool is_uppercase);
extern "C" char get_from_cbuffer(cbuffer_t* buf);
extern "C" void rem_from_cbuff(cbuffer_t* buf);

const char kbdus[128] = // the entire us keyboard, if we want to try support other keyboard layouts have to add those yourself
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};

enum scan_codes {
    leftArrowPressed = 0x4b,
    rightArrowPressed = 0x4d,
    downArrowPressed = 0x50,
    upArrowPressed = 0x48,
    leftShiftPressed= 0x2a,
    capsLockedPressed = 0x3a,
    returnPressed = 0x1c,
    F5Pressed = 0x3f,
    leftShiftReleased = 0x1d,
    ctrlPressed = 0x1d,
    ctrlReleased = 0x9d
};

cbuffer_t* keybuff;

void add_to_kbuff(char c, bool is_uppercase) {
    add_to_cbuff(keybuff, c, is_uppercase);
}

char get_from_kbuff() {
    return get_from_cbuffer(keybuff);
}

void rem_from_kbuff() {
    if(!keybuff) return;

    rem_from_cbuff(keybuff);
}


// Action buffer to store cursor movements we might add, mostly a skeleton rn


cbuffer_t* actionbuff;

void add_to_abuff(char c) {
    add_to_cbuff(actionbuff, c, false);
}

char get_from_abuff() {
    return get_from_cbuffer(actionbuff);
}

void rem_from_abuff() {
    rem_from_cbuff(actionbuff);
}

// here on out is practically copy and pasted from mellos building the keyboard handler
void keyboard_handler(struct regs *r) {
    static bool shift_pressed = false;
    static bool caps_lock = false;
    static bool ctrl_pressed = false;

    unsigned char scancode; // has to be dynamiclly resizable due to the nature of scan coes and not every one being the same
    scancode = inb(0x60);

    // if the top bit of the byte we read from the keyboard is et, thjat means that a key has been released
    if (scancode & 0x80) { // use this to
        // to check if user releases the special key
        switch(scancode) {
                case leftShiftReleased: shift_pressed = false; break;
            case ctrlReleased: ctrl_pressed = false; break;
        }
    }
    else {
        switch(scancode) {
                case leftArrowPressed:  add_to_abuff('L'); break;
                case rightArrowPressed: add_to_abuff('R'); break;
                case downArrowPressed:  add_to_abuff('D'); break;
                case upArrowPressed:    add_to_abuff('U'); break;
                case leftShiftPressed:  shift_pressed = true; break;
            case ctrlPressed:        ctrl_pressed = true; break;
                case capsLockedPressed:   caps_lock = !caps_lock; break;
                case returnPressed:      add_to_kbuff('\n', 0); break;

                case 0x56: add_to_kbuff(shift_pressed ? '>' : '<', 0); break;
            case 0xE: add_to_kbuff(8,0); break;
                default:
            if (ctrl_pressed)
                add_to_abuff(kbdus[scancode]);
            else {
                add_to_kbuff(kbdus[scancode], shift_pressed | caps_lock);
            }
        }
    }
}

extern "C" void irq_install_handler(int irq, void (*handler));

void kb_install()
{
    keybuff = kmalloc(sizeof(cbuffer_t));
    actionbuff = kmalloc(sizeof(cbuffer_t));

    keybuff -> size = 1000;
    keybuff -> top = 0;
    keybuff -> bot = 0;
    keybuff -> array = kmalloc(keybuff -> size);

    actionbuff -> size = 1000;
    actionbuff -> top = 0;
    actionbuff -> bot = 0;
    actionbuff -> array = kmalloc(actionbuff -> size);

    irq_install_handler(1, keyboard_handler);
}
