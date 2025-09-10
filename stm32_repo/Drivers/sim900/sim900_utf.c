#include "sim900_utf.h"
#include <stdio.h>

size_t sim900_utf8_to_ucs2_hex(const char *utf8, char *hex, size_t hexsz)
{
    size_t out = 0;
    while (*utf8 && (out + 4) < hexsz) {
        unsigned c = (unsigned char)*utf8++;
        if (c < 0x80) {
            snprintf(&hex[out], 5, "%04X", c);
            out += 4;
        } else {
            // skip multibyte, output '?'
            snprintf(&hex[out], 5, "%04X", '?');
            out += 4;
            while ((*utf8 & 0xC0) == 0x80) utf8++;
        }
    }
    if (out < hexsz) hex[out] = '\0';
    return out;
}

size_t sim900_ucs2_hex_to_utf8(const char *hex, char *utf8, size_t utf8sz)
{
    size_t out = 0;
    while (hex[0] && hex[1] && hex[2] && hex[3] && (out + 1) < utf8sz) {
        unsigned v = 0;
        sscanf(hex, "%4x", &v);
        hex += 4;
        utf8[out++] = (char)v;
    }
    if (out < utf8sz) utf8[out] = '\0';
    return out;
}
