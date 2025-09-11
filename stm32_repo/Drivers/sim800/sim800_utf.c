#include "sim800_utf.h"
#include <string.h>

static uint32_t utf8_decode(const char **s) {
    const unsigned char *p = (const unsigned char *)*s;
    uint32_t cp = 0;
    if (p[0] < 0x80) { cp = p[0]; *s += 1; }
    else if ((p[0] & 0xE0) == 0xC0) { cp = ((p[0]&0x1F)<<6)|(p[1]&0x3F); *s += 2; }
    else if ((p[0] & 0xF0) == 0xE0) { cp = ((p[0]&0x0F)<<12)|((p[1]&0x3F)<<6)|(p[2]&0x3F); *s += 3; }
    else if ((p[0] & 0xF8) == 0xF0) { cp = ((p[0]&0x07)<<18)|((p[1]&0x3F)<<12)|((p[2]&0x3F)<<6)|(p[3]&0x3F); *s += 4; }
    else { *s += 1; }
    return cp;
}

static size_t utf8_encode(uint32_t cp, char *out) {
    if (cp < 0x80) { out[0] = (char)cp; return 1; }
    if (cp < 0x800) {
        out[0] = (char)(0xC0 | (cp >> 6));
        out[1] = (char)(0x80 | (cp & 0x3F));
        return 2;
    }
    if (cp < 0x10000) {
        out[0] = (char)(0xE0 | (cp >> 12));
        out[1] = (char)(0x80 | ((cp >> 6) & 0x3F));
        out[2] = (char)(0x80 | (cp & 0x3F));
        return 3;
    }
    out[0] = (char)(0xF0 | (cp >> 18));
    out[1] = (char)(0x80 | ((cp >> 12) & 0x3F));
    out[2] = (char)(0x80 | ((cp >> 6) & 0x3F));
    out[3] = (char)(0x80 | (cp & 0x3F));
    return 4;
}

bool sim800_utf8_to_ucs2_hex(const char *utf8, char *hex, size_t hexsz) {
    size_t pos = 0;
    const char *p = utf8;
    while (*p && pos + 4 < hexsz) {
        uint32_t cp = utf8_decode(&p);
        if (cp > 0xFFFF) cp = '?';
        hex[pos++] = "0123456789ABCDEF"[(cp >> 12) & 0xF];
        hex[pos++] = "0123456789ABCDEF"[(cp >> 8) & 0xF];
        hex[pos++] = "0123456789ABCDEF"[(cp >> 4) & 0xF];
        hex[pos++] = "0123456789ABCDEF"[cp & 0xF];
    }
    if (pos >= hexsz) return false;
    hex[pos] = '\0';
    return true;
}

static int hex_val(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return -1;
}

bool sim800_ucs2_hex_to_utf8(const char *hex, char *utf8, size_t utf8sz) {
    size_t pos = 0;
    const char *p = hex;
    while (p[0] && p[1]) {
        int h1 = hex_val(p[0]);
        int h2 = hex_val(p[1]);
        int h3 = hex_val(p[2]);
        int h4 = hex_val(p[3]);
        if (h1 < 0 || h2 < 0 || h3 < 0 || h4 < 0) return false;
        uint32_t cp = (uint32_t)((h1<<12)|(h2<<8)|(h3<<4)|h4);
        if (pos + 4 >= utf8sz) return false;
        pos += utf8_encode(cp, &utf8[pos]);
        p += 4;
    }
    if (pos >= utf8sz) return false;
    utf8[pos] = '\0';
    return true;
}
