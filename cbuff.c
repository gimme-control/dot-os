#include "cbuff.h"
#include <stdbool.h>

void add_to_cbuff(cbuffer_t *buf, char c, bool is_uppercase) {
    buf -> array[ buf->top] = c - (is_uppercase && c <= 122 && c>=97 ? 32 : 0);
    buf -> top = ((buf->top+1) % buf -> size);
}

char get_from_cbuff(cbuffer_t* buf) {
    if (buf->bot == buf-> top)
        return 0;
    char res = buf -> array[buf-> bot];
    buf -> bot = ((buf -> bot +1) % buf -> size);

    return res;
}

void rem_from_cbuff(cbuffer_t *buf) {
    if (buf -> bot != buf-> top)
        buf -> bot = ((buf -> bot +1) % buf -> size);
}


