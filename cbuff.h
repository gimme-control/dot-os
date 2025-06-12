#pragma once
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint32_t top;
    uint32_t bot;
    uint32_t size;
    char* array;
} cbuffer_t;

void add_to_cbuff(cbuffer_t* buf, char c, bool is_uppercase);
char get_from_cbuffer(cbuffer_t * buf);
void rem_from_cbuff(cbuffer_t* buf);
