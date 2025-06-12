#include <stdint.h>

struct cbuffer_t {
    char* array;
    uint32_t top;
    uint32_t bot;
    uint32_t size;
};

// we point to the top of the array within the buffer first and then check what character was given and if it was uppercase or lowercase
// the logic after just handles an "on the fly" capitalization of the character if the is_uppercase flag was true
extern "C" void add_to_cbuff(cbuffer_t* buf, char c, bool is_uppercase) {
    buf -> array[buf -> top] = c - (is_uppercase && c <= 122 && c >= 97 ? 32 : 0);
    buf -> top = ((buf -> top + 1) % buf -> size);
}

char get_from_cbuffer(cbuffer_t * buf) {
    if (buf -> bot == buf -> top)
        return 0; // return 0 if buffer is empty

    char res = buf -> array[buf->bot];
    buf -> bot = ((buf -> bot +1) % buf -> size);

    return res;  // res represents whichever character we end up returning from the cbuffer normally a number representing some ascii value
}

void rem_from_cbuff(cbuffer_t* buf) {
    if (buf -> bot != buf -> top)
        buf -> bot = ((buf -> bot +1) % buf -> size);
} // as long as the cbuff isn't empty we can remove things by pointing to the bottom of the buffer
// and making whatever exists there equal to the next value, similar to a linked list
